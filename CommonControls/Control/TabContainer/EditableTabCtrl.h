#pragma once
#include "InputEdit.h"
#include <vector>



class COMMONCONTROLS_API CEditableTabCtrl : public CWnd
{
	enum ButtonStatus { eDefStatus, eHightLightStatus, ePushDownSatus };
public:
	CEditableTabCtrl(DWORD dwStyle = TCS_ENABLEEDIT | TCS_SHOWADDBTN | TCS_SHOWCLOSEBTN);
	~CEditableTabCtrl();
	DECLARE_MESSAGE_MAP()
	
public:
	// 设置样式
	void SetDisplayStyle(DWORD dwStyle);

	void SetNotifyWnd(HWND hWnd);

	// 返回当前样式
	DWORD GetDisplayStyle();

	// 设置自定义编辑框
	void SetCustomEdit(CInputEdit *pEdit);

	// 创建窗口
	void CreateWnd(CWnd *pParent, LPRECT rt, UINT uId = 0);

	// 插入页签
	int InsertItem(LPCTSTR labelName, CWnd *pChildren, int nIndex = -1);
	
	// 移除页签
	void RemoveItem(int nIndex);
	
	// 获得当前选中项
	int GetCurSel();

	// 返回页签大小
	bool GetItemRect(int nIndex, LPRECT retRect);

	// 设置页签项文本
	void SetItemText(int nIndex, LPCTSTR newTxt);

	// 获得页签项文本
	CString GetItemText(int nIndex);

	// 选择项
	void SelectItem(int nIndex);

	int GetItemCount();

	// 清除所有Tab
	void ClearAll();

protected:
	CInputEdit* GetInputEditor();
	void UpdateItemRect();
	void UpdateTipInfo();

	CRect GetAddButtonPos();
	CRect GetContainerPos();
	CRect GetTabContainerPos(bool bIncludeBtn);

	// 返回tab页签填充内容
	void GetTabCorner(const TabInfo& tabInfo, CPoint& leftTop, CPoint& rightTop, CPoint &leftBottom, CPoint &rightBottom);

	// 绘制tab页签外轮廓效果
	void DrawTabItemFrame(CDC *pDC, const TabInfo& info);
	void DrawTabItemText(CDC *pDC, const TabInfo& info);
	void DrawTabCloseButton(CDC *pDC, const TabInfo& info);

	// 绘制tab页签效果
	void DrawTabItem(CDC *pDC, const TabInfo& info);

	// 绘制添加按钮
	void DrawAddButton(CDC *pDC, ButtonStatus btInfo);

	// 测试是否点中了tab页签，如果没有点中按钮，则返回-1;
	int HitTest(CPoint pt);

	// 测试点中了那个tab页签的关闭按钮，如果没有点中任何关闭按钮，返回-1;
	int HitCloseBtTest(CPoint pt);
	void UpdateCloseButtonStatus(CPoint msPt);
	bool CheckMouseInAddButtonArea(CPoint msPt);

	// 重载
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnEndEdit(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	CInputEdit  m_DefInputEditor;		// tab名称编辑框
	CInputEdit  *m_pInputEditor;		// 自定义tab名称编辑框
	TabInfoList m_TabList;				// 所有的tab记录
	int			m_CurSel;				// 当前选中项
	int			m_nTabHeight;			// Tab高度
	CRect		m_TabRect;				// 当前tab区域大小
	int			m_nMinTabWidth;			// Tab最小宽度
	int			m_nMaxTabWidth;			// Tab最大宽度
	int			m_nTabPoDu;				// Tab页签的坡度
	int			m_nButtonWidth;			// 添加按钮的宽度
	int			m_nButtonSpan;			// 添加按钮与其他按钮的预留宽度
	int			m_nTabToContainer;		// Tab到容器之间的距离
	COLORREF	m_crTabBkColor;			// Tab页签的背景颜色
	COLORREF    m_crTabHightBkColor;	// Tab页签高亮显颜色
	COLORREF	m_crTabFrameColor;		// Tab外边框颜色
	COLORREF	m_crContainerColor;		// 填充区颜色
	COLORREF	m_crTabContainerColor;	// Tab区域填充颜色
	int			m_nCloseBtSize;			// 关闭按钮大小
	int			m_nIconSize;			// 图标大小
	int			m_nIconOffset;			// 图标与最左边的偏移
	static CString s_ClassName;			// 窗口类名
	CImageList	m_Images;				// 图像列表
	CImageList	m_AddButtonImgs;		// 按钮样式
	ButtonStatus m_ButtonStatus;		// 按钮状态信息
	CToolTipCtrl m_TipCtrl;				// 提示信息
	DWORD		 m_nDisStyle;			// 显示样式
	HWND		m_NotifyWnd;			// 通知窗口
	CFont		m_TabFnt;				// Tab绘制字体
};

