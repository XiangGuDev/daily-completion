//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           IMSRegDlg.H
//    @author         zhuyu
//    @data           2018/3/28 10:37
//    @brief          平台注册对话框
//*****************************************************

#pragma once

#define BUSATTSDLG L"业务属性对话框"

/**
*  @class    用于统一更新，关闭对话框
*
*  @brief
*/
class COMMONCONTROLS_API CMSRegDlg
{
public:
	/**
	*  @brief    构造
	*
	*  @param    bool bIsMutex	是否为互斥对话框
	*  @return   
	*/
	CMSRegDlg(bool bIsMutex, const CString& strDlgName = L"");
	virtual ~CMSRegDlg();

public:
	/**
	*  @brief    更新对话框
	*
	*  @return
	*/
	virtual void UpdateDialog();

	/**
	*  @brief    关闭对话框
	*
	*  @return   
	*/
	virtual void CloseDialog() = 0;

protected:
	/**
	*  @brief    注册对话框
	*
	*  @param    IMSRegDlg * pDlg
	*  @param    bool bIsMutex	是否为互斥对话框
	*  @return   void
	*/
	void RegistDialog(CMSRegDlg* pDlg, bool bIsMutex);

	/**
	*  @brief    卸载对话框
	*
	*  @param    CMSRegDlg * pDlg
	*  @return   void
	*/
	void UnRegistDialog(CMSRegDlg* pDlg);
	
	/**
	*  @brief    在map中删除关闭的对话框指针
	*
	*  @param    CMSRegDlg * pDlg
	*  @return   void
	*/
	void DeleteDialogInMap(CMSRegDlg* pDlg);
};



/**
*  @class    用于统一更新，关闭对话框
*
*  @brief
*/
class COMMONCONTROLS_API CDlgMgrTool
{
public:

	/**
	*  @brief    更新对话框
	*
	*  @return   void
	*/
	static void UpdateDialog();

	/**
	*  @brief    关闭对话框
	*
	*  @return   void
	*/
	static void CloseDialog();

	/**
	*  @brief    获取对话框指针
	*
	*  @param    const CString&
	*  @return   CMSRegDlg*
	*/
	static CMSRegDlg* GetDlgPter(const CString& strName);

protected:

};