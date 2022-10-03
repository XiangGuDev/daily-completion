#pragma once

#define WM_SHOWTASK WM_USER+666

class CBaseTaskDlg : public CDialogEx
{
public:
	CBaseTaskDlg(UINT nIDTemplate, CWnd *pParent = nullptr);
	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAILYCOMPLETION_DIALOG };
#endif
public:
	// 是否隐藏窗口到托盘
	void Hide2Taskbar(bool bHide = true);
	void SetTaskbarTip(const CString &strTip);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
protected:
	// 生成的消息映射函数
	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg LRESULT onShowTask(WPARAM wParam, LPARAM lParam);
	void ToTray();
protected:
	NOTIFYICONDATA m_nid;
	bool _bHideWnd;
	CString _strTaskbarTip;
};


