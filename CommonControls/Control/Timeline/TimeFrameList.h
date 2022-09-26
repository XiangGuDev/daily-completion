/*---------------------------------------------------------------------
*文件名称：TimeFrameListCtrl.h
*功能描述：时间帧控件
*创建标识：陶子龙2021.9.18.
*
*修改标识：
*修改描述：
----------------------------------------------------------------------*/
#pragma once

//时间帧区域通知结构
struct CTimeFrameNotify
{
	NMHDR hdrNotify;										// 通知消息结构
	int row;												// 所在行
	int col;												// 所在列
	ITimeLineCtrlDataSource *timeLineCtrlDataSource;		// 所属的数据源
};

// 定义颜色列表
typedef std::vector<COLORREF> VecColor;

// 定义收缩的行数
typedef std::set<int> SetCollapseRow;
typedef SetCollapseRow::iterator SetCollapseRowIter;

class COMMONCONTROLS_API CTimeFrameListCtrl :public CWnd
{
public:
	CTimeFrameListCtrl(void);
	virtual ~CTimeFrameListCtrl(void);

protected:
	DECLARE_DYNAMIC(CTimeFrameListCtrl)
	DECLARE_MESSAGE_MAP()

public:
	/* 
	* 函数介绍：创建时间帧列表控件
	* 输入参数：const RECT& rect 控件区域
	*			CWnd* pParentWnd 父窗口
	*			CTimeLineCtrl* timeLineCtrl 所在的时间轴控件
	*			UINT nID(0xfff0) 控件ID
	*			DWORD dwStyle(WS_BORDER | WS_CHILD | WS_VISIBLE) 控件风格
	* 输出参数：void
	* 返回值  ：bool 是否创建成功
	*/
	bool Create(const CRect &rect, CWnd *pParentWnd, CTimeLineCtrl *pTimeLineCtrl, UINT nID = NULL,
		DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VISIBLE);

	/*
	* 函数介绍：初始化控件配置
	* 输入参数：const CStirng & strCfg 配置文件路径
	* 输出参数：void
	* 返回值  ：void
	*/
	void LoadConfig(const CString& strCfg);

	/*
	* 函数介绍：更新控件
	* 输入参数：void
	* 输出参数：void
	* 返回值  ：void
	*/
	void UpdateCtrl(void);

	/*
	* 函数介绍：设置事件通知父窗口
	* 输入参数：void
	* 输出参数：CWnd * pParent	父窗口句柄
	* 返回值  ：void
	*/
	void SetNofityWnd(HWND hNotifyWnd);

	/*
	* 函数介绍：设置鼠标响应判断接口
	* 输入参数：IMouseResponseJudgment *pMouseResponseJudgement 鼠标响应判断接口
	* 输出参数：void
	* 返回值  ：void
	*/
	void SetMouseResponseJudgment(IMouseResponseJudgment *pMouseResponseJudgement);

	/*
	* 函数介绍：设置时间帧刻度的长度
	* 输入参数：int nTimeFrameLength 时间帧刻度的长度
	* 输出参数：void
	* 返回值  ：void
	*/
	void SetTimeFrameLength(int nTimeFrameLength);

	/*
	* 函数介绍：获取时间帧刻度的长度
	* 输入参数：void
	* 输出参数：void
	* 返回值  ：int 时间帧刻度的长度
	*/
	int GetTimeFrameLength(void);

	/*
	* 函数介绍：设置时间帧区的行数
	* 输入参数：int nItemCount 时间帧区的行数
	* 输出参数：void
	* 返回值  ：void
	*/
	void SetItemCount(int nItemCount);

	/*
	* 函数介绍：获取单元格高度
	* 输入参数：void
	* 输出参数：void
	* 返回值  ：int 单元格高度
	*/
	int GetCellHeight(void);
	/*
	* 函数介绍：获取表头的高度
	* 输入参数：void
	* 输出参数：void
	* 返回值  ：int 表头的高度
	*/
	int GetHeadHeight(void);
	/*
	* 函数介绍：获取表头和单元格的宽度
	* 输入参数：void
	* 输出参数：void
	* 返回值  ：int 表头和单元格的宽度
	*/
	int GetWidth(void);

	/*
	* 函数介绍：获取单元格的行数
	* 输入参数：void
	* 输出参数：void
	* 返回值  ：int 单元格的行数
	*/
	int GetItemCount(void);

public:
	/*
	* 函数介绍：设置关键帧
	* 输入参数：int nRow 单元格行号
	*			TimeFrameRowInfoMap &mapTimeFrameInfo 行号对应的时间帧信息列表
	* 输出参数：void
	* 返回值  ：void
	*/
	void SetKeyFrame(int nRow, TimeFrameRowInfoMap &mapTimeFrameInfo, CMemoryDC& dcMemory);
	/*
	* 函数介绍：设置关键帧
	* 输入参数：int nRow 单元格行号
	*			int nCol 单元格列号
	* 输出参数：void
	* 返回值  ：void
	*/
	void SetKeyFrame(int nRow, int nCol, CMemoryDC& dcMemory);
	/*
	* 函数介绍：设置补间动画
	* 输入参数：int nRow 单元格行号
	*			TimeFrameRowInfoMap &mapTimeFrameInfo 行号对应的时间帧信息列表
	* 输出参数：void
	* 返回值  ：void
	*/
	void SetTweenAnimation(int nRow, TimeFrameRowInfoMap &mapTimeFrameInfo, CMemoryDC& dcMemory);
	/*
	* 函数介绍：设置补间动画
	* 输入参数：int nRow 单元格行号
	*			int nCol 单元格列号
	* 输出参数：void
	* 返回值  ：void
	*/
	void SetTweenAnimation(int nRow, int nCol, CMemoryDC& dcMemory);

	CTimeLineCtrl* GetTimeLineCtrl() { return _timeLineCtrl; }

protected:
	/*
	* 函数介绍：获取单元格区域(根据行号和列号)
	* 输入参数：int nRow 单元格行号
	*			int nCol 单元格列号
	* 输出参数：void
	* 返回值  ：void
	*/
	CRect GetCellRect(int nRow, int nCol);
	/*
	* 函数介绍：获取单元格区域中心点(根据行号和列号)
	* 输入参数：int nRow 单元格行号
	*			int nCol 单元格列号
	* 输出参数：void
	* 返回值  ：void
	*/
	CPoint GetCellPoint(int nRow, int nCol);
	/*
	* 函数介绍：获取单元格区域中心点(根据单元格区域)
	* 输入参数：CRect rect 单元格区域
	* 输出参数：void
	* 返回值  ：void
	*/
	CPoint GetCellPoint(CRect rect);

	/*
	* 函数介绍：绘制点标识(设置为关键帧)
	* 输入参数：CPoint centerPoint 标识的中心位置
	*			COLORREF &colorSpot 点的颜色
	*			CMemoryDC &dcMemory 双缓存dc
	* 输出参数：void
	* 返回值  ：void
	*/
	void DrawSpot(CPoint centerPoint, COLORREF &colorSpot, CMemoryDC &dcMemory);

	/*
	* 函数介绍：绘制时间帧
	* 输入参数：CMemoryDC &dcMemory 双缓存dc
	* 输出参数：void
	* 返回值  ：void
	*/
	void DrawTimeFrame(CMemoryDC& dcMemory);

protected:
	/*
	* 函数介绍：向父级发送Notify通知消息
	* 输入参数：UINT nMessage 消息ID
	*			int &nRow(-1) 单元格所在行
	*			int &nCol(-1) 单元格所在列
	*			ITimeLineCtrlDataSource *timeLineCtrlDataSource(NULL) 单元格所属数据源
	* 输出参数：void
	* 返回值  ：LRESULT 事件通知结果
	*/
	LRESULT NotifyParent(UINT nMessage, int nRow = -1, int nCol = -1,
		ITimeLineCtrlDataSource *timeLineCtrlDataSource = NULL);

	/*
	* 函数介绍：获取当前鼠标指向的位置
	* 输入参数：CPoint* pPtMs 当前鼠标客户坐标
	* 输出参数：int &nRow 鼠标指向的行(-1表示在表头)
	*			int &nCol 鼠标指向的列
	* 返回值  ：bool 焦点在控件范围内则返回true，否则返回false
	*/
	bool GetFocusPos(int &nRow, int &nCol, CPoint* ptMs = NULL);

	/*
	* 函数介绍：获取当前鼠标指向位置对应的数据源
	* 输入参数：int nRow 单元格所在行
	* 输出参数：void
	* 返回值  ：ITimeLineCtrlDataSource* 当前鼠标指向位置对应的数据源
	*/
	ITimeLineCtrlDataSource* GetFocusDataSource(int nRow);

	/*
	* 函数介绍：获取收缩后显示对应的真实的行数
	* 输入参数：int nRow 收缩后显示的行数
	* 输出参数：void
	* 返回值  ：int 收缩后显示对应的真实的行数
	*/
	int GetRowAfterCollapse(int nRow);

	/*
	* 函数介绍：获取收缩后显示的行数
	* 输入参数：int nRow 收缩后显示对应的真实的行数
	* 输出参数：void
	* 返回值  ：int 收缩后显示的行数
	*/
	int GetCollapseDisplayRow(int nRow);

	/*
	* 函数介绍：移动当前帧
	* 输入参数：CPoint ptMs 鼠标的位置
	* 输出参数：void
	* 返回值  ：void
	*/
	void MoveCurrentFrame(CPoint ptMs);

protected:
	/*
	* 函数介绍：填充表头背景色
	* 输入参数：CMemoryDC &dcMemory 双缓存dc
	*			CRect rcHead 填充的区域
	* 输出参数：void
	* 返回值  ：void
	*/
	void FillHeadBkColor(CMemoryDC &dcMemory, CRect rcHead);
	/*
	* 函数介绍：绘制时间帧列表列头刻度
	* 输入参数：CMemoryDC &dcMemory 双缓存dc
	*			CRect rcHead 绘制的区域
	*			bool bFirst 区域内是否包含第一格
	*			bool bLast 区域内是否包含最后一格
	* 输出参数：void
	* 返回值  ：void
	*/
	void DrawTimeFrameHeadLine(CMemoryDC &dcMemory, CRect rcHead, bool bFirst, bool bLast);
	/*
	* 函数介绍：绘制时间帧列表列头文本
	* 输入参数：CMemoryDC &dcMemory 双缓存dc
	*			CRect rcHeadText 绘制文本的区域
	*			int nText 绘制文本的内容
	* 输出参数：void
	* 返回值  ：void
	*/
	void DrawTimeFrameHeadText(CMemoryDC &dcMemory, CRect rcHeadText, int nText);
	/*
	* 函数介绍：绘制时间帧列表列头
	* 输入参数：CMemoryDC &dcMemory 双缓存dc
	* 输出参数：void
	* 返回值  ：void
	*/
	void DrawTimeFrameHead(CMemoryDC &dcMemory);

	/*
	* 函数介绍：绘制时间帧列表单元格
	* 输入参数：CMemoryDC &dcMemory 双缓存dc
	* 输出参数：void
	* 返回值  ：void
	*/
	void DrawTimeFrameCell(CMemoryDC &dcMemory);

	/*
	* 函数介绍：绘制当前帧
	* 输入参数：CMemoryDC &dcMemory 双缓存dc
	* 输出参数：void
	* 返回值  ：void
	*/
	void DrawCurrentFrame(CMemoryDC &dcMemory);

	/*
	* 函数介绍：绘制选中的单元格
	* 输入参数：CMemoryDC &dcMemory 双缓存dc
	* 输出参数：void
	* 返回值  ：void
	*/
	void DrawSelectCell(CMemoryDC &dcMemory);

	/*
	* 函数介绍：更新滚动条
	* 输入参数：void
	* 输出参数：void
	* 返回值  ：void
	*/
	void UpdateScrollBar(void);

protected:
	/*
	* 函数介绍：获取字符串颜色
	* 输入参数：const CString & strColor 字符串颜色
	* 输出参数：void
	* 返回值  ：COLORREF 字符串颜色
	*/
	COLORREF GetStringColor(const CString& strColor);
	/*
	* 函数介绍：获取字符串颜色列表
	* 输入参数：const CString & strColor 字符串颜色列表
	* 输出参数：void
	* 返回值  ：VecColor 字符串颜色列表
	*/
	VecColor GetStringColorVec(const CString& strColor);

protected:
	/*
	* 函数介绍：注册控件类
	* 输入参数：void
	* 输出参数：void
	* 返回值  ：bool 是否成功注册
	*/
	virtual bool RegisterTimeFrameList(void);

protected:
	//自画响应事件
	afx_msg void OnPaint(void);
	//擦除绘制的背景色
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//鼠标移动响应事件
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//鼠标左键按下响应事件
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//鼠标左键弹起响应事件
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//鼠标左键双击响应事件
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//鼠标右键按下响应事件
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//横向滚动条响应事件
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//窗口尺寸大小变化响应事件
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	CTimeLineCtrl*				_timeLineCtrl;					// 对应的时间轴控件

protected:
	HWND						_hNofigyWnd;					// 事件通知窗口

	int							_nLength;						// 时间帧刻度的长度
	int							_nItemCount;					// 单元格的行数
	int							_nWidth;						// 表头和单元格的宽度
	int							_nHeadHeight;					// 表头的高度
	int							_nCellHeight;					// 单元格的高度
	int							_nKeyFrameSpotSizePercent;		// 关键帧点大小的百分比

	int							_nCurrentFrameCol;				// 当前帧所在的列
	bool						_bPressDownCurrentFrame;		// 是否按住当前帧

	IMouseResponseJudgment*		_pMouseResponseJudgment;		// 鼠标响应判断
	int							_nSelRow;						// 鼠标响应判断的选中行
	bool						_bResponseMoveKeyFrame;			// 是否响应移动关键帧

	COLORREF					_colorHead;						// 表头的颜色
	COLORREF					_colorTimePoint;				// 时刻点的颜色
	COLORREF					_colorCurrentFrame;				// 当前帧框架的颜色
	COLORREF					_colorCurrentFrameBack;			// 当前帧填充的颜色
	VecColor					_vecColorKeyFrame;				// 关键帧的颜色列表
	VecColor					_vecColorTweenAnimation;		// 补间动画的颜色列表

	CRect						_rcHeadText;					// 表头时刻点区域
	int							_nHeadText;						// 表头显示区域的开始时刻点文本

	int							_arrSelCellRowAndCol[2];		// 选中单元格的行(0)和列(1)

	SetCollapseRow				_setCollapseRow;				// 收缩的行数
};