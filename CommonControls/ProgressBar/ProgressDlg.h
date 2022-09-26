//*****************************************************
//    
//    @copyright      三维设计组
//    @version        v1.0
//    @file           ProgressDlg.h
//    @author         ZSX
//    @data           2018/9/3 18:15
//    @brief          进度条对话框
//*****************************************************

#pragma once
#include "afxcmn.h"
#include "afxdialogex.h"

#define USER_PROGRESSMSGBOX WM_USER + 1201

class CProgressDlg : public CDialogEx, public IProgressListener
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // 标准构造函数
	CProgressDlg(ModalProgressInfo* pProInfo, bool bCancel = true, CProgressAgentStopEvent* pEvent = nullptr);
	virtual ~CProgressDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROGRESS_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_ProgressBar;
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnDisposeUserMsg(WPARAM wParam, LPARAM lParam);
		
	/**
	*  函数介绍 设置标题
	*
	*  函数参数 const CString& sText
	*  返回值   void
	*/
	void SetTitle(const CString& sText);

	/**
	*  函数介绍 设置文字
	*
	*  函数参数 const CString& sTitle
	*  返回值   void
	*/
	void SetText(const CString& sTitle);

	/**
	*  函数介绍 设置进度
	*
	*  函数参数 int nStep
	*  返回值   void
	*/
	virtual void OnStep(int nStep);

	/**
	*  函数介绍 获取当前进度
	*
	*  返回值   int
	*/
	int GetCurPos();

protected:
	/**
	*  函数介绍 模态框更新进度
	*
	*  返回值   void
	*/
	void ModalUpdate();


private:
	CFont _ssFont;		// 字体
	CStatic _stShow;	// 显示文字控件

	ModalProgressInfo* _pProInfo;	// 模态进度信息
	HANDLE _pThreadHandle;			// 模态时线程句柄
	bool	_bCanStop = false;//是否可以停止
	bool    _bIsStopping = false;//是否已启动停止

	CProgressAgentStopEvent* _pEvent = nullptr;//停止处理事件
};
