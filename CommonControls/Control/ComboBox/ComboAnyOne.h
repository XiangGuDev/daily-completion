#pragma once
#include "afxwin.h"
#ifndef WM_END_EDIT
#define WM_END_EDIT (WM_USER+1)
#endif

// 控件基类
class IPopupCtrl
{
public:

	IPopupCtrl()
	{
		m_pOwner = NULL;
		m_pNotifyWnd = NULL;
	}

	virtual ~IPopupCtrl() {}

public:
	// 返回窗口
	virtual CWnd* GetWnd(CWnd *pOnwer) = 0;

	// 返回需要在Combo上显示的内容
	virtual CString GetCtrlTextForShow() = 0;

	// 通知控件显示
	virtual void OnPreShowCtrl(LPCTSTR curComboTxt) = 0;

	// 返回控件不显示滚动条的尺寸
	virtual CSize GetBestSize() = 0;

	// 设置所有者
	void SetOwner(CWnd *pCombo)
	{
		m_pOwner = pCombo;
	}

	// 设置消息通知接收窗口，一般为WM_NOTIFY、WM_COMMAND消息的处理者
	void SetNotifyWnd(CWnd *pWnd)
	{
		m_pNotifyWnd = pWnd;
	}

protected:
	void UpdateOwner()
	{
		m_pOwner->Invalidate();
	}

protected:
	CWnd *m_pOwner;
	CWnd *m_pNotifyWnd;
};

// COMBO下拉面板，负责下拉面板
class COMMONCONTROLS_API CComboPanel : public CWnd
{
public:
	// 下拉面板
	CComboPanel(IPopupCtrl *subWnd);

	// 析构下拉面板
	~CComboPanel();

	// 弹出下拉面板
	void PopupComboPanel();

	// 结束编辑
	void EndEdit();

	// 创建窗口
	void CreateWnd(CWnd *pParent);

	DECLARE_MESSAGE_MAP()

protected:
	// 返回安全的子窗口指针，如果子窗口不存在，则返回NULL
	CWnd* GetSafeSubWnd();

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

protected:
	IPopupCtrl *m_pubWnd;
	static CString s_ClassName;
};

// 下拉任意控件
class COMMONCONTROLS_API CComboAnyOne : public CWnd
{
public:
	CComboAnyOne(IPopupCtrl *subWnd);
	~CComboAnyOne();

public:
	// 创建窗口
	void CreateWnd(CWnd *pParent, LPRECT rt, UINT uId = 0);

	// 设置通知窗口，默认是父窗口,一般为WM_NOTIFY、WM_COMMAND消息的处理者
	void SetNotifyWnd(CWnd *pNewWnd);

	// 显示文本转换函数
	virtual CString TranslateSelection(LPCTSTR szDefTxt);

protected:
	// 计算必要尺寸
	void UpdateWndSize();

	// 下拉弹出子控件
	void DropDownSubCtrl();

	// 返回文本区域
	CRect GetTxtRect();

	// 重载消息预处理函数
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	CString GetSelectionText();
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
protected:
	CComboPanel  m_Panel;				// 面板
	CToolTipCtrl m_TipCtrl;				// 提示信息
	IPopupCtrl  *m_pSubWnd;				// 下拉控件内容
	HWND		 m_NotifyWnd;			// 通知窗口
	CRect		 m_BtnRect;				// 按钮大小
	CRect		 m_DropListRect;		// 下拉框位置
	CFont		 m_Font;				// 文字样式
	int			 m_ComboStatus;			// 下拉框状态
	int			 m_nDropDownListHeight;	// 下拉框高度
	static const int m_BtnSize = 16;
	static const int m_nMarin = 4;
	static const int m_nDropBtPos = 3;
	static const int m_nTipId = 1;
	static CString s_ClassName;
	CImageList m_Images;
};

