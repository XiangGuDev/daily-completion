/*****************************************************
*    文件名称：    BaseDialog.H
*    功能描述：	   基础对话框
*    创建标识：    淦海强  2018/11/14 17:42
*
*    修改标识：
*    修改描述：
*****************************************************/

#pragma once
#include "afxdialogex.h"

class COMMONCONTROLS_API CBaseDialog : public CDialogEx, public CMSRegDlg
{
public:
	/**
	*  函数介绍：   基类对话框
	*  函数输入：  UINT nIDTemplate 对话框ID
	*  函数输入：  bool bIsMutex   true为互斥对话框，flase为共存对话框
	*  函数输入：  CWnd * pParentWnd
	*  输出参数：    
	*  返回值：     
	*/
	CBaseDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL, bool bIsMutex = true, const CString& strDialogName = L"");

	/**
	*  函数介绍:    初始化对话框
	*  返回值:   	BOOL
	*/
	virtual BOOL OnInitDialog();

	virtual void CloseDialog() = 0;

	// 销毁对话框
	virtual BOOL DestroyWindow();

	virtual afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};

