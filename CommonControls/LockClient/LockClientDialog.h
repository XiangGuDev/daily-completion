//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0H
//    @file           LockClientDialog.H
//    @author         KG
//    @date           2015/09/14 10:26
//    @brief          锁对话框
//*****************************************************

#pragma once

#include "LicenceAccess.h"
//#include "resource.h"
//#include "..\HyperLink\HyperLink.h"

// 完成粘贴序列号消息
#define WM_COMPLETE_PASTE	WM_USER + 22

/*
* 对话框退出操作
*/
enum ENotFoundMode
{
	eShowLic = 12,	// 显示许可证管理
	eNetLogin = 15,	// 进行网络登陆
	eRetryDet = 17,	// 重新检测锁
	eQuitLock = 19,	// 退出锁检测
	eQuitDown = 21,	// 退出完成
};

/**
*  @class    找不到许可提示框
*
*  @brief
*/
class CNoFoundLienceDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
public:
	enum { IDD = IDD_LOCK_TIP };
	CNoFoundLienceDlg(CWnd *pParent = NULL);

	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnStnClickedAddLic();
	afx_msg void OnBtnRetry();
	afx_msg void OnBtnNetLogin();
	afx_msg void OnCheckManualLogin();
public:
	/*
	*	初始化
	*/
	virtual BOOL OnInitDialog();

	/*
	*	获取对话框退出操作
	*/
	ENotFoundMode GetNoFoundMode();

	/*
	*	获取是否手动登陆
	*/
	BOOL IsManualLoginIn();

	/**
	*  @brief    消息预处理;
	*
	*/
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	CHyperLink _addLisLink; // 添加许可证
	ENotFoundMode _noFoundMode;	// 对话框退出操作
	CButton _checkManualLogin;
	BOOL _bIsManualLoginIn = FALSE;
public:
	afx_msg void OnClose();
};

/**
*  @class    软件许可证管理对话框
*
*  @brief
*/
class CServerLicenceMgrDlg : public CDialog
{
public:
	enum { IDD = IDD_LICENCEMGR };
	CServerLicenceMgrDlg(CWnd *pParent = NULL);
	~CServerLicenceMgrDlg();

	DECLARE_MESSAGE_MAP()

public:
	/*
	*	初始化
	*/
	BOOL OnInitDialog();

	/*
	*	自动添加许可证号
	*/
	void OnAutoAddLience();

	/*
	*	手动添加许可证号
	*/
	void OnAddLience();

	/*
	*	确认
	*/
	void OnOK();

	/*
	*	退出
	*/
	void OnCancel();

	/*
	*	选择锁
	*/
	void OnSelLockItem();

	/*
	*	添加许可证
	*/
	bool AddLicence(CString strSelLockLabel);

private:
	/*
	*	取得默认公司名称
	*/
	CString GetDefaultCorpName();

	/*
	*	根据标签查找设备ID
	*/
	bool FindLockByLabel(IBooLockClientServicePtr& service,
		LPCTSTR label, IBooReadLockInfoPtr& lock);

protected:
	CListCtrl		_licListCtrl;	// 许可正列表
};


/**
*  @class    注册号编辑控件
*
*  @brief
*/
class CLicenceEdit : public CEdit
{
	DECLARE_MESSAGE_MAP()
public:
	CLicenceEdit();
	~CLicenceEdit();

	/*
	*	处理粘贴注册号
	*/
	LRESULT OnPaste(WPARAM wParam, LPARAM lParam);

	/*
	*	从系统剪切板中获取数据
	*/
	bool GetDataFromClipboard(CString& strResult);

	/*
	*	解析数据格式是否符合完整粘贴要求(暂时不限制格式)
	*/
	bool IsCorrectFormat(LPCTSTR strLicence)
	{
		CString temp(strLicence);
		return temp.Find(_T('-')) > -1;
	}
};

/**
*  @class    添加许可证号对话框
*
*  @brief
*/
class CAddLienceDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()

public:
	enum { IDD = IDD_ADDLIENCE };
	CAddLienceDlg(CWnd *pWnd = NULL);
	afx_msg void OnOK();
	afx_msg void OnCancel();

public:
	/*
	*	初始化
	*/
	virtual BOOL OnInitDialog();

	/*
	*	当输入的数字超过限制时跳到下一个组件框
	*/
	void OnEditLienceChange1();

	/*
	*	当输入的数字超过限制时跳到下一个组件框
	*/
	void OnEditLienceChange2();

	/*
	*	当输入的数字超过限制时跳到下一个组件框
	*/
	void OnEditLienceChange3();

	/*
	*	当输入的数字超过限制时跳到下一个组件框
	*/
	void OnEditLienceChange4();

	/*
	*	完整粘贴处理
	*/
	LRESULT OnCompletePaste(WPARAM wParam, LPARAM lParam);

public:
	CString			_strCorpname;
	CString			_strLicence;
	CLicenceEdit	_licenceEdit;
};

/**
*  @class    网络锁登陆对话框
*
*  @brief    
*/
class CNetLoginDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
public:
	enum { IDD = IDD_DLG_NETLOGIN };
	CNetLoginDlg(CLicenceAccess *pLinc, CWnd *pParent = NULL);

public:

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	/*
	*	确认
	*/
	void OnOK();

	/*
	*	退出
	*/
	void OnCancel();

	/**
	*  @brief    判断是否登陆成功
	*
	*  @return   BOOL
	*/
	BOOL LoginSucceed();
private:
	/**
	*  @brief    修改AppStarter.ini内容
	*
	*  @param    const CString&  Service对应字段
	*  @param    const CString&  account对应字段
	*  @param    const CString&  password对应字段
	*  @return   BOOL
	*/
	BOOL SetNetLockInfo(const CString& strServer, const CString& strAccount, const CString& strPassword);

private:
	CEdit _netAddrEdit;		///<网络地址编辑框
	CEdit _adminEdit;		///<用户名编辑框
	CEdit _passEdit;		///<密码编辑框
	CLicenceAccess *_pLinc; ///<锁组件访问
	BOOL  _eSucc;	///<<是否成功登陆
};