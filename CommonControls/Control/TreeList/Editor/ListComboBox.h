//*****************************************************
//    
//    @copyright      配网设计组;
//    @version        v3.0
//    @file           ComboBox.H
//    @author         mahaidong;
//    @date           2015/4/9 14:18
//    @brief          列表普通组合框控件;
//*****************************************************

#pragma once

#include "EditBase.h"
#include "EditContainer.h"

/**
*  @class    CListComboBox
*
*  @brief    列表普通组合框控件
*/
class COMMONCONTROLS_API CListComboBox : public CComboBox, public IEditBase
{
	DECLARE_DYNAMIC(CListComboBox)

public:
	CListComboBox();
	virtual ~CListComboBox();

protected:
	DECLARE_MESSAGE_MAP()

public:
	/**
	*  @brief    创建控件;
	*
	*  @param    CWnd * pParent 父窗口句柄;
	*  @param    CRect & rect 绘制区域;
	*  @param    bool bDropList 是否不可编辑;
	*  @param    LPCTSTR sInitText 默认显示文本;
	*  @return   bool 创建成功返回true，否则返回false;
	*/
	virtual bool Create(CWnd* pParent, CRect& rect, bool bDropList, LPCTSTR sInitText);

	/**
	*  @brief    设置焦点行及选中;
	*
	*/
	void SetSelAndFocus();

	/**
	*  @brief    加载下拉数据
	*
	*  @param    const CString& strXmlFile 下拉数据文件
	*  @return   bool 加载成功返回true
	*/
	bool LoadXmlData(const CString& strXmlFile);

	/**
	*  @brief    加载字符串数组
	*
	*  @param    CStringVector & listData 字符串数组
	*/
	void LoadStringVector(CStringVector& listData);

	/**
	*  @brief    失去焦点的处理
	*
	*  @param    CWnd * pWnd 句柄
	*/
	virtual void DoKillFocus(CWnd* pWnd);

	/**
	*  @brief    取消编辑
	*
	*/
	void CancelEdit();

private:
	/**
	*  @brief    自动选择默认项
	*
	*/
	void AutoSelectItem();

	/**
	*  @brief    绘制组合框列表项;
	*
	*/
	void DrawItem(HDC hDC, const CRect& rcItem, int iItem, int iState);

	/**
	*  @brief    获取下拉项高度
	*
	*/
	virtual void MeasureItem(LPMEASUREITEMSTRUCT pMItem);

	/**
	*  @brief    绘制下拉项
	*
	*/
	virtual void DrawItem(LPDRAWITEMSTRUCT pDIStruct);

public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void ProcessKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public:
	WNDPROC			_pEditProc;  ///< 编辑框消息

protected:
	CString			_strInitText; ///< 初始显示值
	bool			_bDropList;   ///< 无输入框模式
	bool			_bCandel;	  ///< 是否取消
	COLORREF		_crNormal;	  ///< 正常颜色
	COLORREF		_crSelected;  ///< 选择颜色
	COLORREF		_crText;      ///< 文本颜色
	CEditContainer	_container;   ///< 边框容器
};