//*****************************************************
//    
//    @copyright      软件二部
//    @version        v1.0
//    @file           BatchOperationDlg.H
//    @author         LC
//    @data           2018/10/16 16:59
//    @brief          批量操作对话框(可用于批量替换/覆盖/删除等场景)
//****************************************************/

#pragma once

/**
*  @class 	批量操作对话框
*
*  @brief
*/
class CBatchOperationDlg : public CDialog
{
	enum { IDD = IDD_DLG_BATCHOPERATION };

public:
	CBatchOperationDlg(LPCTSTR strMess, LPCTSTR strTitle, CWnd* pParent = NULL);
	~CBatchOperationDlg();

	DECLARE_MESSAGE_MAP()

protected:
	/**
	*  @brief    数据交换
	*
	*/
	virtual void DoDataExchange(CDataExchange* pDX);

	// 初始化对话框时，创建页面
	afx_msg BOOL OnInitDialog();

	afx_msg void OnOK();
	afx_msg void OnCancel();

private:
	CStatic			_staticMess;		///< 提示信息控件
	CButton			_checkBox;			///< 勾选框

	CString			_strMess;			///< 提示信息
	CString			_strTitle;			///< 标题名
};

