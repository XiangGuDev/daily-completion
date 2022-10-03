#pragma once

#define WM_SHOWTASK WM_USER+666
#define WM_HIDETOTASKBAR  WM_USER+667  
#define WM_SHOWFROMTASKBAR  WM_USER+668  
class CBaseTaskDlg : public CDialogEx
{
public:
	CBaseTaskDlg(UINT nIDTemplate, CWnd *pParent = nullptr);
	void HideToTaskbar();
	void ShowFromTaskbar();
	void SetTaskbarTip(const CString &strTip);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
protected:
	// 生成的消息映射函数
	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnHideToTaskbar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowFromTaskbar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT onShowTask(WPARAM wParam, LPARAM lParam);
	void ToTray();
protected:
	NOTIFYICONDATA m_nid;
	CString _strTaskbarTip;
	CRect _tmpRect;
	bool _bVisible;
};


