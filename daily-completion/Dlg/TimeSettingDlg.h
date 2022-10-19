
// daily-completionDlg.h: 头文件
//

#pragma once
#include "../Base/DCController.h"

class CTimeSettingDlg : public CDialogEx, public DCController
{
public:
	CTimeSettingDlg(CWnd* pParent = nullptr);	// 标准构造函数
	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAILYCOMPLETION_DIALOG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
protected:
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnClickReset();
	virtual void OnOK();
private:
	CComboBox _combTime;
	CComboBox _combName;
	CComboBox _combShow;
	std::vector<CString> _nameCache;
};


