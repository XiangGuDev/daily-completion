//*****************************************************
//    
//    @copyright      配网设计组;
//    @version        v3.0
//    @file           SplitCtrl.H
//    @author         mahaidong;
//    @date           2015/4/2 17:33
//    @brief          分隔条控件;
//*****************************************************

#pragma once

// Splitter panes constants
#define SPLIT_PANE_LEFT			 0
#define SPLIT_PANE_RIGHT		 1
#define SPLIT_PANE_TOP			 SPLIT_PANE_LEFT
#define SPLIT_PANE_BOTTOM		 SPLIT_PANE_RIGHT
#define SPLIT_PANE_NONE			-1

// Splitter extended styles
#define SPLIT_PROPORTIONAL		0x00000001
#define SPLIT_NONINTERACTIVE	0x00000002
#define SPLIT_RIGHTALIGNED		0x00000004
#define SPLIT_BOTTOMALIGNED		SPLIT_RIGHTALIGNED

enum 
{ 
	m_nPanesCount = 2, 
	m_nPropMax = 10000 
};

/**
*  @class CSplitCtrl;
*
*  @brief 分隔条控件;
*/
class COMMONCONTROLS_API CSplitCtrl : public CWnd
{
	DECLARE_DYNAMIC(CSplitCtrl)

protected:
	DECLARE_MESSAGE_MAP()

public:
	/**
	*  @brief    构造函数;
	*
	*/
	CSplitCtrl();

public:
	/**
	*  @brief    创建控件;
	*
	*  @param    const RECT & rect 窗口区域;
	*  @param    CWnd * pParentWnd 父窗口;
	*  @param    UINT nID 控件ID;
	*  @param    bool bVertical true: 水平分隔条，false: 垂直分隔条;
	*  @param    DWORD dwStyle 样式;
	*  @return   bool 成功返回true;
	*/
	bool Create(const RECT& rect, CWnd* pParentWnd, UINT nID, bool bVertical = true, DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VISIBLE);

	void SetSplitterRect(LPRECT lpRect = NULL, bool bUpdate = true);

	void GetSplitterRect(LPRECT lpRect) const;

	bool SetSplitterPos(int xyPos = -1, bool bUpdate = true);

	void SetSplitterPosPct(int nPct, bool bUpdate = true);

	int GetSplitterPos() const;

	void SetFixed(bool bFixed);
		
	LRESULT OnMouseWheelEx(WPARAM /*wParam*/, LPARAM /*lParam*/);
	bool SetSinglePaneMode(int nPane = SPLIT_PANE_NONE);

	int GetSinglePaneMode() const;

	DWORD GetSplitterExtendedStyle() const;

	DWORD SetSplitterExtendedStyle(DWORD dwExtendedStyle, DWORD dwMask = 0);

	// Splitter operations
	void SetSplitterPanes(HWND hWndLeftTop, HWND hWndRightBottom, bool bUpdate = true);

	bool SetSplitterPane(int nPane, HWND hWnd, bool bUpdate = true);

	HWND GetSplitterPane(int nPane) const;

	bool SetActivePane(int nPane);

	int GetActivePane() const;

	bool ActivateNextPane(bool bNext = true);

	bool SetDefaultActivePane(int nPane);

	bool SetDefaultActivePane(HWND hWnd);

	int GetDefaultActivePane() const;

	void DrawSplitter(CDC &dc);

	// Overrideables
	void DrawSplitterBar(CDC &dc);

	// called only if pane is empty
	void DrawSplitterPane(CDC &dc, int nPane);

	afx_msg LRESULT OnCreate(WPARAM /*wParam*/, LPARAM /*lParam*/);

	afx_msg LRESULT OnPaint(WPARAM /*wParam*/, LPARAM /*lParam*/);

	afx_msg LRESULT OnSetCursor(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnLButtonDown(WPARAM /*wParam*/, LPARAM lParam);

	afx_msg LRESULT OnLButtonUp(WPARAM /*wParam*/, LPARAM /*lParam*/);

	LRESULT OnLButtonDoubleClick(WPARAM /*wParam*/, LPARAM /*lParam*/);

	LRESULT OnCaptureChanged(WPARAM /*wParam*/, LPARAM /*lParam*/);

	LRESULT OnSetFocus(WPARAM /*wParam*/, LPARAM);

	LRESULT OnMouseActivate(WPARAM wParam, LPARAM lParam);

	LRESULT OnSettingChange(WPARAM /*wParam*/, LPARAM /*lParam*/);

	LRESULT OnEraseBackground(WPARAM /*wParam*/, LPARAM /*lParam*/);

	LRESULT OnSize(WPARAM wParam, LPARAM /*lParam*/);

protected:
	// Implementation - internal helpers
	void UpdateSplitterLayout();

	bool GetSplitterBarRect(LPRECT lpRect) const;

	bool GetSplitterPaneRect(int nPane, LPRECT lpRect) const;

	bool IsOverSplitterRect(int x, int y) const;

	bool IsOverSplitterBar(int x, int y) const;

	void DrawGhostBar();

	void GetSystemSettings(bool bUpdate);

	bool IsProportional() const;

	void StoreProportionalPos();

	void UpdateProportionalPos();

	bool IsRightAligned() const;

	void StoreRightAlignPos();

	void UpdateRightAlignPos();

	bool IsInteractive() const;

	/**
	*  @brief    注册控件类;
	*
	*  @return   bool 成功返回true;
	*/
	bool RegisterTreeList();
protected:
	HWND				m_hWndPane[m_nPanesCount];
	RECT				m_rcSplitter;
	int					m_xySplitterPos;
	int					m_nDefActivePane;
	int					m_cxySplitBar;              // splitter bar width/height
	HCURSOR				m_hCursor;					// 鼠标状态;
	int					m_cxyMin;                   // minimum pane size
	int					m_cxyBarEdge;              	// splitter bar edge
	bool				m_bFullDrag;
	int					m_cxyDragOffset;
	int					m_nProportionalPos;
	bool				m_bUpdateProportionalPos;
	DWORD				m_dwExtendedStyle;			// splitter specific extended styles
	int					m_nSinglePane;				// single pane mode
	bool				t_bVertical;				// 水平分隔还是垂直分隔;
	bool				_bFixed;					//< 是否固定不允许拖动	
};