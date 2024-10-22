﻿
// daily-completionDlg.h: 头文件
//

#pragma once
#include "YFramework/BindableProperty/BindableProperty.h"
#include "YFramework/Singleton/BaseSingleton.h"
#include "../Base/DCController.h"

class CTaskListCtrl;
class CSettingsDlg : public CDialogEx, public DCController
{
public:
	CSettingsDlg(CWnd* pParent = nullptr);	// 标准构造函数
	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAILYCOMPLETION_DIALOG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnClickAutoStart();
	afx_msg void OnClickChange();
private:
	CDateTimeCtrl _dateCtrl; // 日期
public:
	afx_msg void OnDtnDatetimechangeDate(NMHDR *pNMHDR, LRESULT *pResult);
};


