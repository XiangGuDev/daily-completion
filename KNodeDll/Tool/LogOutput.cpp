#include "StdAfx.h"
#include "LogOutput.h"

#pragma warning(disable:4091)
#include <shlobj.h>
#pragma comment(lib, "shlwapi")

namespace CommBase
{
	/*
	* 定义静态变量
	*/
	static CAtlFile s_logFile;
	static CString s_strLogPath;	///< 记录日志路径
	static bool s_bOpenLog = false;		///< 是否打开了日志
#ifdef SAFE_LOGOUTPUT
	static CRITICAL_SECTION s_cs = { 0 }; ///< 线程同步临界区
#endif
	static int s_nLogType = (eLogError | eLogWarning | eLogInfo | eLogSystem); //日志记录类型

	bool CLog::InitLog(LPCWSTR lpLogPath, int nLogType)
	{
		s_bOpenLog = false;
		s_nLogType = nLogType;
		s_strLogPath = lpLogPath;

		// 确保路径存在
		SHCreateDirectoryEx(NULL, s_strLogPath, NULL);

		//初始化临界区变量
#ifdef SAFE_LOGOUTPUT
		InitializeCriticalSection(&s_cs);
#endif

		//创建日志文件名
		return GenerateLogFile();
	}

	void CLog::UninitLog()
	{
#ifdef SAFE_LOGOUTPUT
		DeleteCriticalSection(&s_cs);
#endif

		s_logFile.Close();
		s_bOpenLog = false;
		s_strLogPath.Empty();
	}

	void CLog::LogErrorV(LPCTSTR pszLog, ...)
	{
		if (!(s_nLogType&eLogError))
			return;

		CString strFormatLog;
		va_list argList;
		va_start(argList, pszLog);
		strFormatLog.FormatV(pszLog, argList);
		va_end(argList);

		// 获取信息时间
		SYSTEMTIME st = { 0 };
		GetLocalTime(&st);
		CString strLogMsg;
		strLogMsg.Format(L"(%d:%d:%d)", st.wHour, st.wMinute, st.wSecond);

		LogToFile(L"错误信息" + strLogMsg + L"---" + strFormatLog + L"\r\n");
	}

	void CLog::LogError(LPCWSTR lpLog)
	{
		if (!(s_nLogType&eLogError))
			return;

		// 获取信息时间
		SYSTEMTIME st = { 0 };
		GetLocalTime(&st);
		CString strLogMsg;
		strLogMsg.Format(L"错误信息(%d:%d:%d)---%s\r\n", st.wHour, st.wMinute, st.wSecond, lpLog);

		LogToFile(strLogMsg);
	}

	void CLog::LogWarningV(LPCTSTR pszLog, ...)
	{
		if (!(s_nLogType&eLogWarning))
			return;

		CString strFormatLog;
		va_list argList;
		va_start(argList, pszLog);
		strFormatLog.FormatV(pszLog, argList);
		va_end(argList);

		// 获取信息时间
		SYSTEMTIME st = { 0 };
		GetLocalTime(&st);
		CString strLogMsg;
		strLogMsg.Format(L"(%d:%d:%d)", st.wHour, st.wMinute, st.wSecond);

		LogToFile(L"警告信息" + strLogMsg + L"---" + strFormatLog + L"\r\n");
	}

	void CLog::LogWarning(LPCWSTR lpLog)
	{
		if (!(s_nLogType&eLogWarning))
			return;

		// 获取信息时间
		SYSTEMTIME st = { 0 };
		GetLocalTime(&st);
		CString strLogMsg;
		strLogMsg.Format(L"警告信息(%d:%d:%d)---%s\r\n", st.wHour, st.wMinute, st.wSecond, lpLog);

		LogToFile(strLogMsg);
	}

	void CLog::LogInfoV(LPCTSTR pszLog, ...)
	{
		if (!(s_nLogType&eLogInfo))
			return;

		CString strFormatLog;
		va_list argList;
		va_start(argList, pszLog);
		strFormatLog.FormatV(pszLog, argList);
		va_end(argList);

		// 获取信息时间
		SYSTEMTIME st = { 0 };
		GetLocalTime(&st);
		CString strLogMsg;
		strLogMsg.Format(L"(%d:%d:%d)", st.wHour, st.wMinute, st.wSecond);

		LogToFile(L"一般信息" + strLogMsg + L"---" + strFormatLog + L"\r\n");
	}

	void CLog::LogInfo(LPCWSTR lpLog)
	{
		if (!(s_nLogType&eLogInfo))
			return;

		// 获取信息时间
		SYSTEMTIME st = { 0 };
		GetLocalTime(&st);
		CString strLogMsg;
		strLogMsg.Format(L"一般信息(%d:%d:%d)---%s\r\n", st.wHour, st.wMinute, st.wSecond, lpLog);

		LogToFile(strLogMsg);
	}

	void CLog::LogSystemV(LPCTSTR pszLog, ...)
	{
		if (!(s_nLogType&eLogSystem))
			return;

		CString strFormatLog;
		va_list argList;
		va_start(argList, pszLog);
		strFormatLog.FormatV(pszLog, argList);
		va_end(argList);

		// 获取信息时间
		SYSTEMTIME st = { 0 };
		GetLocalTime(&st);
		CString strLogMsg;
		strLogMsg.Format(L"(%d:%d:%d)", st.wHour, st.wMinute, st.wSecond);

		LogToFile(L"系统信息" + strLogMsg + L"---" + strFormatLog + L"\r\n");
	}

	void CLog::LogSystem(LPCWSTR lpLog)
	{
		if (!(s_nLogType&eLogSystem))
			return;

		// 获取信息时间
		SYSTEMTIME st = { 0 };
		GetLocalTime(&st);
		CString strLogMsg;
		strLogMsg.Format(L"系统信息(%d:%d:%d)---%s\r\n", st.wHour, st.wMinute, st.wSecond, lpLog);

		LogToFile(strLogMsg);
	}

	void CLog::SetLogType(int nLogType)
	{
		s_nLogType = nLogType;
	}

	void CLog::LogToFile(LPCWSTR lpInfo)
	{
		if (NULL == lpInfo || !s_bOpenLog)
			return;

		try
		{
			//进入临界区
#ifdef SAFE_LOGOUTPUT
			EnterCriticalSection(&s_cs);
#endif

			// 写入日志信息
			s_logFile.Write(lpInfo, lstrlen(lpInfo) * sizeof(TCHAR));
			s_logFile.Flush();

			//离开临界区
#ifdef SAFE_LOGOUTPUT
			LeaveCriticalSection(&s_cs);
#endif
		}
		catch (...)
		{
#ifdef SAFE_LOGOUTPUT
			LeaveCriticalSection(&s_cs);
#endif
		}
	}

	bool CLog::GenerateLogFile()
	{
		if (s_bOpenLog)
			return false;

		// 利用时间格式化成日志文件名
		SYSTEMTIME st = { 0 };
		GetLocalTime(&st);
		CString strLogFile;
		strLogFile.Format(L"%d-%d-%d.log", st.wYear, st.wMonth, st.wDay);

		// 打开日志文件
		if (s_logFile.Create(s_strLogPath + strLogFile, GENERIC_READ | GENERIC_WRITE,
			0, OPEN_ALWAYS) != S_OK)
		{
			return false;
		}

		// 新建文件写入UNICODE头部
		ULONGLONG nLen = 0;
		s_logFile.GetSize(nLen);
		if (!nLen)
		{
			WORD unicodeHead = 0xFEFF;
			s_logFile.Write(&unicodeHead, sizeof(WORD));
		}

		s_bOpenLog = true;

		// 追加日志
		s_logFile.Seek(0, FILE_END);

		// 写入日志信息头
		LogToFile(L"---------------------日志记录---------------------\r\n");

		return true;
	}

	void CLog::LogPerfermance(bool bDone)
	{
		static LARGE_INTEGER frq, tm1, tm2;
		if (!bDone)
		{
			// 获取CPU 始终周期
			::QueryPerformanceFrequency(&frq);
			// 获取CPU计数
			::QueryPerformanceCounter(&tm1);
		}
		else
		{
			// 获取CPU计数
			::QueryPerformanceCounter(&tm2);
			double dTime = (double)(tm2.QuadPart - tm1.QuadPart) / (double)frq.QuadPart;
			CString msg;
			msg.Format(_T("\n代码用时 %f ms\n"), dTime * 1000);
			OutputDebugString(msg);
			LogInfo(msg);
		}
	}
}