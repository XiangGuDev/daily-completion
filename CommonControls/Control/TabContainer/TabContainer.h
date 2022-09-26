//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v3.0
//    @file           TabContainerCtrl.H
//    @author         mahaidong
//    @date           2015/4/2 17:38
//    @brief          tab选项卡控件;
//*****************************************************

#pragma once

// TAB正在选择消息，返回TRUE则阻止选择
#define TAB_SELECTING (WM_USER+2016)

// TAB选择消息
#define TAB_SELECT (WM_USER+2017)

// TAB消息响应事件
#define TAB_TOOLTIPS (WM_USER + 2018)

// 创建
// m_tab.Create(WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE|TCS_SINGLELINE|TCS_TABS|TCS_RIGHTJUSTIFY, rc, this, id);
// 头文件加入 afx_msg LRESULT OnTabSelect(WPARAM, LPARAM);
// CPP中BEGIN_MESSAGE_MAP加入消息响应 ON_MESSAGE(TAB_SELECT, OnTabSelect)
// 其中WPARAM 为当前TAB窗口句柄，做为多个TAB控件判断，LPARAM 当前选择的TAB索引

// 标签控件消息传递结构体
struct CTabContainerNotify
{
	NMHDR hdrNotify;			///< 通知消息结构
	int nSelItem;				///< 被选中的标签页序号
};


class COMMONCONTROLS_API CTabContainer : public CTabCtrl
{
public:
	/**
	*  @brief    构造函数;
	*
	*/
	CTabContainer();

	/**
	*  @brief    析构函数;
	*
	*/
	virtual ~CTabContainer();

public:
	/**
	*  @brief    根据索引设置选中的tab选项卡;
	*
	*  @param    int nItem 需要选中的tab选项卡索引;
	*  @return   int
	*/
	int SetCurSel(int nItem);

	/**
	*  @brief    插入选项卡
	*
	*  @param    int nItem 插入索引;
	*  @param    LPCTSTR lpszItem 选项卡显示名称;
	*  @param    HWND hWnd 窗口句柄;
	*  @return   LONG
	*/
	LONG InsertItem(int nItem, LPCTSTR lpszItem, HWND hWnd);

	/**
	*  @brief    插入选项卡;
	*
	*  @param    int nItem 插入索引;
	*  @param    LPCTSTR lpszItem 选项卡显示名称;
	*  @param    HWND hWnd 窗口句柄;
	*  @param    int nImage 显示图标索引;
	*  @return   LONG 
	*/
	LONG InsertItem(int nItem, LPCTSTR lpszItem, HWND hWnd, int nImage);

	/**
	*  @brief    根据索引删除选项卡;
	*
	*  @param    int nItem 要删除选项卡的索引;
	*/
	void DeleteItem(int nItem);

	/**
	*  @brief    删除全部选项卡;
	*
	*/
	void DeleteAllItems();

	/**
	*  @brief    设置接受消息的窗口句柄;
	*
	*  @param    CWnd * pParent 父窗口句柄;
	*/
	void SetMsgParent(CWnd* pParent);

	/**
	*  @brief    设置标签显示文本;
	*
	*  @param    int nItem 标签索引;
	*  @param    LPCTSTR lpStrText 标签显示文本;
	*/
	void SetTabText(int nItem, LPCTSTR lpStrText);

	/**
	*  @brief    获得标签文本;
	*
	*  @param    int nItem 标签索引;
	*  @return   CString 标签显示文本;
	*/
	CString GetTabText(int nItem);

	/**
	*  @brief    设置是否显示关闭按钮，按钮点击后，只发送相关事件，不关闭标签项
	*
	*  @param    bool bNeedCloseButton	是否显示
	*/
	void SetItemCloseButton(bool bNeedCloseButton = true);

protected:
	/**
	*  @brief    function_description
	*
	*/
	void InitFont();
	/**
	*  @brief    function_description
	*
	*  @param    int nItem
	*/
	void MakeVisible(int nItem);

	/**
	*  @brief    function_description
	*
	*/
	void AdjustChildPos();

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelChanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNMClickTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawMainBorder(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItemBorder(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual COLORREF GetTabColor(BOOL bSelected = FALSE);
	virtual COLORREF GetTabTextColor(BOOL bSelected = FALSE);
	virtual void PreSubclassWindow();

protected:
	CWnd*	_pParent;
	COLORREF m_crBack;
	COLORREF m_tabColor;
	COLORREF m_tabTextColor;
	CFont	 m_fontTab;
	int _nLastCurItem;			///< 上一次选中标签
	CString _strCloseSign;		///< 关闭字符占位
	CRect _rcCloseSign;			///< 关闭字符坐标
	bool _bNeedCloseButton;		///< 是否显示关闭按钮
	bool _bTabItemChange;		///< 切换了标签页标识
};