#include "stdafx.h" // #include "stdafx.h" // #include "pch.h" // 预编译头
#include "ScreenRecorder.h"
#include<queue>
#include "CaptureScreen.h"
#include "MergeMusic.h"

extern "C"
{
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libavutil\time.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libavdevice\avdevice.h>
}
#include <ipp.h>
#include <chrono>


unsigned char clip_value(unsigned char x, unsigned char min_val, unsigned char  max_val) {
	if (x > max_val) {
		return max_val;
	}
	else if (x < min_val) {
		return min_val;
	}
	else {
		return x;
	}
}

//RGB to YUV420
bool RGB24_TO_YUV420(unsigned char *RgbBuf, int w, int h, unsigned char *yuvBuf)
{
	unsigned char*ptrY, *ptrU, *ptrV, *ptrRGB;
	memset(yuvBuf, 0, w*h * 3 / 2);
	ptrY = yuvBuf;
	ptrU = yuvBuf + w * h;
	ptrV = ptrU + (w*h * 1 / 4);
	unsigned char y, u, v, r, g, b;
	for (int j = h - 1; j >= 0; --j) {
		auto wLineLen = ((w * 24 + 31) & 0xffffffe0) / 8;
		ptrRGB = RgbBuf + wLineLen * j ;
		for (int i = 0; i < w; ++i) {
			b = *(ptrRGB++);
			g = *(ptrRGB++);
			r = *(ptrRGB++);

			y = (unsigned char)((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
			*(ptrY++) = clip_value(y, 0, 255);
			if (j % 2 == 0 && i % 2 == 0) {
				u = (unsigned char)((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
				*(ptrU++) = clip_value(u, 0, 255);
			}
			else {
				if (i % 2 == 0) {
					v = (unsigned char)((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;
					*(ptrV++) = clip_value(v, 0, 255);
				}
			}
		}
	}
	return true;
}

CScreenRecorder::CScreenRecorder()
{
	_capThread = NULL;
	_wrtThread = NULL;
	InitData();
}

CScreenRecorder::CScreenRecorder(const CString & strSaveFileName, const CRect & rect)
{
	_capThread = NULL;
	_wrtThread = NULL;
	_strSaveFileName = strSaveFileName;
	InitData();
}

CScreenRecorder::~CScreenRecorder()
{
	DeleteCriticalSection(&_Section);
}

void CScreenRecorder::AddBGMusic(const CString &strVideoFilePath, const CString &strMusicFilePath, const CString &strTargetFilePath)
{
	Merge merge;
	// 视频文件路径
	string szVideoPath = CW2A(strVideoFilePath.GetString());
	// 音频文件路径
	string szAudioPath = CW2A(strMusicFilePath.GetString());
	// 合成文件路径
	string szDstPath = CW2A(strTargetFilePath.GetString());

	merge.addMusic(szVideoPath, szAudioPath, szDstPath, "00:00:00");
}

CString CScreenRecorder::GetLastVideoPath()
{
	return _strLastVideoPath;
}

void CScreenRecorder::StartRecording()
{
	StopRecording();

	_bStop = false;

	// 初始化数据
	if (!InitRecording())
	{
		::MessageBox(::GetActiveWindow(), L"FFMPEG初始化失败！", L"提示", MB_OK | MB_ICONWARNING);
		return;
	}

	// 开启线程
	_wrtThread = (HANDLE)_beginthreadex(NULL, 0, &CScreenRecorder::WriteThread, (LPVOID)this, 0, NULL);
	_capThread = (HANDLE)_beginthreadex(NULL, 0, &CScreenRecorder::CaptureThread, (LPVOID)this, 0, NULL);
}

void CScreenRecorder::StopRecording()
{
	if (_bStop)
		return;
	_bStop = true;

	WaitForSingleObject(_capThread, INFINITE);
	WaitForSingleObject(_wrtThread, INFINITE);
	CloseHandle(_capThread);
	CloseHandle(_wrtThread);
	_capThread = NULL;
	_wrtThread = NULL;

	AfterRecording();

	CString strInfo;
	strInfo.Format(L"缓冲区占用：%d/100",_ImageQueue.size());
	// OutputDebugString(strInfo);
}

void CScreenRecorder::SetScreenRect(const CRect & rect)
{
	_rect = rect;

	int nWidth = _rect.Width();
	int nHeight = _rect.Height();

	// 补位为4的倍数
	if (nWidth % 4 != 0 || nHeight % 4 != 0)
	{
		nWidth = nWidth - nWidth % 4;
		nHeight = nHeight - nHeight % 4;
	}

	_rect.right = _rect.left + nWidth;
	_rect.bottom = _rect.top + nHeight;

	_bFullScreen = false;
}

void CScreenRecorder::SetScreenRect(CWnd * pWnd)
{
	if (!pWnd)
		return;
	//// 设置录屏区域
	CRect rcDlg;
	pWnd->GetClientRect(&rcDlg);
	//pWnd->ClientToScreen(&rcDlg);
	SetScreenRect(rcDlg);
	_pCurWnd = pWnd;
}

void CScreenRecorder::SetSaveFileName(const CString & strSaveFileName)
{
	_strSaveFileName = strSaveFileName;
}

void CScreenRecorder::SetFPS(int nFPS)
{
	_nDefFps = nFPS;
	_nCurFps = _nDefFps;
}

int CScreenRecorder::GetFPS()
{
	return _nDefFps;
}

void CScreenRecorder::SetFullScreen(bool bFullScreen)
{
	_bFullScreen = bFullScreen;
}

void CScreenRecorder::ShowMouse(bool bShow)
{
	_bShowMouse = bShow;
}
bool CScreenRecorder::InitCapture()
{
	if (_avframe == nullptr)
		return false;

	int ret = av_image_fill_arrays(_avframe->data,
		_avframe->linesize,
		_outbuffer,
		AV_PIX_FMT_YUV420P,
		_Width,
		_Height, 1);

	if (ret < 0)
	{
		printf("av_image_fill_arrays is faild! \n");
		return false;
	}
	_packet = av_packet_alloc();
	av_init_packet(_packet);
	if (!_packet)
	{
		printf("packet is faild! \n");
		return false;
	}

	_frameNumber = 0;
	return true;
}

bool CScreenRecorder::Capture()
{
	_nLastTime = GetTickCount();

	BYTE* frameimage = nullptr;
	if (_bFullScreen)
	{
		frameimage = _CaptureScreen->CaptureImage();
	}
	else
	{
		frameimage = _CaptureScreen->CaptureImage(_rect.left, _rect.top, _Width, _Height);
	}
	if (!frameimage)
		return false;

	long curframeTime = GetTickCount() - _nLastTime;
	_nTotaltime = GetTickCount() - _nStartTime;

	double dAverframeTime = (_nTotaltime * 1.0) / (_frameNumber + 1); // 当前平均每帧时间
	_nCurFps = (int)(1000.0 / dAverframeTime); // 更新帧率

	// 动态帧率打印
	CString str;
	str.Format(_T("nTotaltime：%lldms\r\n_frameNumber：%d帧\r\n当前帧率：%dFPS\r\n"), _nTotaltime, _frameNumber, _nCurFps);
	// OutputDebugString(str);

	// 防止帧率过快
	if (curframeTime < _dDefOneFrameTime)
	{
		Sleep(_dDefOneFrameTime - curframeTime);
	}
	EnterCriticalSection(&_Section);
	int leaveSize = _ImageQueue.size();
	LeaveCriticalSection(&_Section);
	// 防止缓冲区占用过快
	if (leaveSize > 75)
	{
		Sleep(50);
		TRACE(L"\r\n > 75");
	}
	else if (leaveSize > 50)
	{
		Sleep(30);
		TRACE(L"\r\n > 50");
	}

	// 帧队列临界区
	if (leaveSize < 100)
	{
		EnterCriticalSection(&_Section);
		_ImageQueue.push(frameimage);
		LeaveCriticalSection(&_Section);
	}
	else
	{
		TRACE(L"\r\n 缓冲区已满...");
	}

	if (_bStop)
		return false;

	return true;
}

unsigned __stdcall CScreenRecorder::CaptureThread(void * pParam)
{
	CScreenRecorder *pSR = (CScreenRecorder *)pParam;
	if (pSR == nullptr || !pSR->InitCapture())
		return 0;

	while(true)
	{
		if (!pSR->Capture())
			break;
	}

	return 0;
}

bool CScreenRecorder::WriteFrame()
{

	BYTE* pframeimage = NULL;

	// 帧队列临界区
	EnterCriticalSection(&_Section);
	if (_ImageQueue.size() == 0)
	{
		LeaveCriticalSection(&_Section);
		if (_bStop)
		{
			return false;
		}
		return true;
	}
	pframeimage = _ImageQueue.front();
	_ImageQueue.pop();
	if (pframeimage == NULL)
	{
		LeaveCriticalSection(&_Section);
		return 0;
	}
	LeaveCriticalSection(&_Section);


	RGB24_TO_YUV420(pframeimage, _Width, _Height, _outbuffer);

	free(pframeimage);

	// 动态帧率修改
	_avCodecCtx_Out->time_base.den = (_nCurFps == 0) ? 1 : _nCurFps;
	_avframe->pkt_dts = _avframe->pts = _frameNumber * _avCodecCtx_Out->time_base.num * _avStream->time_base.den / (_avCodecCtx_Out->time_base.den * _avStream->time_base.num);
	//_avframe->pkt_dts = _avframe->pts = av_rescale_q_rnd(_frameNumber, avCodecCtx_Out->time_base, avStream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));

	_avframe->pkt_duration = 0;
	_avframe->pkt_pos = -1;

	int ret = avcodec_send_frame(_avCodecCtx_Out, _avframe);
	if (ret < 0)
		return false;

	ret = avcodec_receive_packet(_avCodecCtx_Out, _packet);
	if (ret < 0)
		return false;

	if (_packet->size > 0)
	{
		//av_packet_rescale_ts(packet, avCodecCtx_Out->time_base, avStream->time_base);

		/*if (av_write_frame(_avFormCtx_Out, _packet) < 0)
		{
			_avFormCtx_Out = NULL;
			toolkit::commkit::CLog::WriteLog(_T("av_write_frame is fail\r\n"));
			return false;
		}*/
		av_write_frame(_avFormCtx_Out, _packet);
		_frameNumber++;
		TRACE(L"录入第%d帧....\n", _frameNumber);
	}
	return true;

}

unsigned __stdcall CScreenRecorder::WriteThread(void * pParam)
{
	CScreenRecorder *pSR = (CScreenRecorder *)pParam;
	if (pSR == nullptr)
		return 0;
	while(true)
	{
		if (!pSR->WriteFrame())
			break;
	}

	return 0;
}

bool CScreenRecorder::InitRecording()
{
	// 初始化代码
	avformat_network_init();
	avdevice_register_all();

	_CaptureScreen = std::shared_ptr<CCaptureScreen>(_bFullScreen ? new CCaptureScreen(this) : new CCaptureScreen(this, _rect.Width(), _rect.Height()));
	_CaptureScreen->ShowMouse(_bShowMouse);

	_Width = 0;
	_Height = 0;

	_CaptureScreen->GetSize(_Width, _Height);

	CreateDirectory((LPCWSTR)CFileToolkit::GetFileDirectory(_strSaveFileName), NULL);//创建文件目录

	int ret = avformat_alloc_output_context2(&_avFormCtx_Out, NULL, NULL, CT2A((LPCWSTR)_strSaveFileName));
	if (ret < 0)
	{
		printf("Init avformat object is faild! \n");
		return false;
	}

	_avCodec = avcodec_find_encoder(_avFormCtx_Out->oformat->video_codec);
	if (!_avCodec)
	{
		printf("Init avCodec object is faild! \n");
		return false;
	}

	_avCodecCtx_Out = avcodec_alloc_context3(_avCodec);
	if (!_avCodecCtx_Out)
	{
		printf("Init avCodecCtx_Out object is faild! \n");
		return false;
	}
	_avStream = avformat_new_stream(_avFormCtx_Out, _avCodec);
	if (!_avStream)
	{
		printf("Init avStream object is faild! \n");
		return false;
	}

	_avCodecCtx_Out->flags |= AV_CODEC_FLAG_QSCALE;
	_avCodecCtx_Out->bit_rate = 4000 * 4000 * 4;
	//_avCodecCtx_Out->rc_min_rate = 4000000;
	//_avCodecCtx_Out->rc_max_rate = 4000000;
	//_avCodecCtx_Out->bit_rate_tolerance = 4000000;
	_avCodecCtx_Out->time_base.den = _nDefFps;
	_avCodecCtx_Out->time_base.num = 1;

	_avCodecCtx_Out->width = _Width;
	_avCodecCtx_Out->height = _Height;

	//pH264Encoder->pCodecCtx->_frame_number = 1;
	_avCodecCtx_Out->gop_size = 1;
	_avCodecCtx_Out->max_b_frames = 0;
	_avCodecCtx_Out->thread_count = 4;
	_avCodecCtx_Out->pix_fmt = AV_PIX_FMT_YUV420P;
	_avCodecCtx_Out->codec_id = AV_CODEC_ID_H264;
	_avCodecCtx_Out->codec_type = AVMEDIA_TYPE_VIDEO;

	av_opt_set(_avCodecCtx_Out->priv_data, "b-pyramid", "none", 0);
	av_opt_set(_avCodecCtx_Out->priv_data, "preset", "superfast", 0);
	av_opt_set(_avCodecCtx_Out->priv_data, "tune", "zerolatency", 0);

	if (_avFormCtx_Out->oformat->flags & AVFMT_GLOBALHEADER)
		_avCodecCtx_Out->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

	ret = avcodec_open2(_avCodecCtx_Out, _avCodec, NULL);
	if (ret < 0)
	{
		printf("Open avcodec is faild! \n");
		return false;
	}

	avcodec_parameters_from_context(_avStream->codecpar, _avCodecCtx_Out);
	if (!(_avFormCtx_Out->oformat->flags & AVFMT_NOFILE))
	{
		ret = avio_open(&_avFormCtx_Out->pb, CT2A((LPCWSTR)_strSaveFileName), AVIO_FLAG_WRITE);
		if (ret < 0)
		{
			printf("Open file is faild! \n");
			return false;
		}
	}

	ret = avformat_write_header(_avFormCtx_Out, NULL);
	if (ret < 0)
	{
		printf("write header is faild! \n");
		return false;
	}

	_avframe = av_frame_alloc();
	if (!_avframe)
	{
		printf("Init _avframe is faild! \n");
		return false;
	}
	_avframe->format = AV_PIX_FMT_YUV420P;
	_avframe->width = _Width;
	_avframe->height = _Height;

	LONG64 frameSize = 0;
	frameSize = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, _Width, _Height, 1);
	if (_outbuffer != nullptr)
	{
		delete[] _outbuffer;
		_outbuffer = nullptr;
	}
	_outbuffer = new BYTE[frameSize];

	// 初始化参数
	_dDefOneFrameTime = 1000.0 / _nDefFps; // 默认每帧时间
	_nTotaltime = 0; // 总时间
	_nStartTime = GetTickCount(); // 开始时间
	_nLastTime = _nStartTime;
	return true;
}

void CScreenRecorder::AfterRecording()
{
	EnterCriticalSection(&_Section);
	if (_avFormCtx_Out && _avFormCtx_Out->pb)
	{
		av_write_trailer(_avFormCtx_Out);
		avformat_free_context(_avFormCtx_Out);
		avcodec_close(_avCodecCtx_Out);
		avcodec_free_context(&_avCodecCtx_Out);
		av_free(_avCodec);
		av_packet_free(&_packet);
		av_frame_free(&_avframe);
		// 解除文件占用
		try
		{
			avio_close(_avFormCtx_Out->pb);
		}
		catch (...)
		{
			::MessageBox(::GetActiveWindow(), _T("视频中间文件被占用！"), _T("提示"), MB_OK | MB_ICONWARNING);
		}
		
	}
	else
	{
		::MessageBox(::GetActiveWindow(), _T("发生异常，视频录制失败！"), _T("提示"), MB_OK | MB_ICONWARNING);
	}
	LeaveCriticalSection(&_Section);

	if (_outbuffer != nullptr)
	{
		delete[] _outbuffer;
		_outbuffer = nullptr;
	}

	while (!_ImageQueue.empty())
	{
		auto pframeimage = _ImageQueue.front();
		_ImageQueue.pop();
		free(pframeimage);
	}
	// 录屏相关属性
	_Width = 0;
	_Height = 0;
	_avFormCtx_Out = nullptr;
	_avCodecCtx_Out = nullptr;
	_avCodec = nullptr;
	_avStream = nullptr;
	_avframe = nullptr;
	_packet = nullptr;
	_outbuffer = nullptr;
	_CaptureScreen = nullptr;
	_frameNumber = 0;
	// 帧率计算相关
	_dDefOneFrameTime = 0; // 默认每帧时间
	_nTotaltime = 0; // 总时间
	_nStartTime = 0; // 开始时间
	_nLastTime = 0; // 上次时间
}

void CScreenRecorder::InitData()
{
	_nDefFps = 30;
	_nCurFps = _nDefFps;
	_pCurWnd = NULL;
	InitializeCriticalSection(&_Section);
}

CWnd * CScreenRecorder::GetCurWnd()
{
	return _pCurWnd;
}

CSize CScreenRecorder::GetTextPos()
{
	return _txtPos;
}

void CScreenRecorder::SetTextPos(const CSize & pos)
{
	_txtPos = pos;
}

const CString & CScreenRecorder::GetText()
{
	return _strText;
}

void CScreenRecorder::SetText(const CString & strText)
{
	_strText = strText;
}
