//*****************************************************
//    
//    @copyright      软件二部
//    @version        v1.0
//    @file           BatchOperDlg.H
//    @author         LC
//    @data           2018/10/17 8:54
//    @brief          批量操作类
//****************************************************/

#pragma once

class CBatchOperationDlg;

/**
*  @class 	批量操作对话框对外接口类
*
*  @brief	用于创建（批量替换/覆盖/删除）对话框
*/
class COMMONCONTROLS_API CBatchOperDlg
{
public:
	CBatchOperDlg(LPCTSTR strMess, LPCTSTR strTitle, CWnd* pParent = NULL);
	~CBatchOperDlg();
	
	/**
	*  @brief    模态窗口
	*
	*  @return   int
	*/
	int DoModal();

protected:
	
	/**
	*  @brief    初始化数据
	*
	*  @param    LPCTSTR strMess		提示信息
	*  @param    LPCTSTR strTitle		对话框标题
	*  @param    CWnd * pParent			父窗口
	*  @return   void
	*/
	void InitData(LPCTSTR strMess, LPCTSTR strTitle, CWnd* pParent = NULL);

private:
	CBatchOperationDlg*		_pDlg;
};

