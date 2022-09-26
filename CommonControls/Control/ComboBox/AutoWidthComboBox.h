//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           AutoWidthComboBox.H
//    @author         wanghongchang
//    @date           2015\9\19 15:06
//    @brief          自定更新宽度下拉框
//*****************************************************
#pragma once

/**
*  @class    CAutoWidthComboBox
*
*  @brief    自动更新宽度下拉框
*/
class COMMONCONTROLS_API CAutoWidthComboBox : public CComboBox
{
public:
	/**
	*  @brief    根据下拉文本长度更新下拉框宽度
	*
	*  @param    CComboBox & combox
	*  @return   void
	*/
	static void ComboxAutoWidth(CComboBox &combox);

	static void ComboxAutoResize(CComboBox &combox);
	/*
	*  @brief    设置信息父窗口
	*
	*  @param    CWnd * pParent
	*/
	void SetMsgParent(CWnd* pParent);

public:

	virtual void MoveWindow(int x, int y, int nWidth, int nHeight,
		BOOL bRepaint = TRUE);

	virtual void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);

public:
	// manipulating listbox items
	/**
	*  @brief    添加下拉列表
	*
	*  @param    LPCTSTR lpszString 下拉列表记录名称
	*  @param    bool bAutoWidth 是否自动调整宽度，当传入false时，添加完后需要自己调用ComboxAutoWidth方法调整宽度
	*  @return   int 索引
	*/
	int AddString(LPCTSTR lpszString, bool bAutoWidth = true);
	int DeleteString(UINT nIndex);
	int InsertString(int nIndex, LPCTSTR lpszString);
	void ResetContent();


};