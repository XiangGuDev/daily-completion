//*****************************************************
//    
//    @copyright      配网设计组;
//    @version        v3.0
//    @file           ComboBox.H
//    @author         mahaidong;
//    @date           2015/4/9 14:18
//    @brief          树型组合框控件;
//*****************************************************

#pragma once

#include "EditBase.h"
#include "EditContainer.h"

/**
*  @class    CListTreeComboBox
*
*  @brief    树型组合框控件
*/
class COMMONCONTROLS_API CListTreeComboBox : public CTreeComboBox, public IEditBase
{
	DECLARE_DYNAMIC(CListTreeComboBox)

public:
	CListTreeComboBox();
	virtual ~CListTreeComboBox();

protected:
	DECLARE_MESSAGE_MAP()

public:
	/**
	*  @brief    创建控件;
	*
	*  @param    CWnd * pParent 父窗口句柄;
	*  @param    CRect & rect 绘制区域;
	*  @param    LPCTSTR lpszInit 默认显示文本;
	*  @param    LPCTSTR lpszData 默认显示扩展文本;
	*  @param    bool bDropList 是否不可编辑
	*  @return   bool 创建成功返回true，否则返回false;
	*/
	bool CreateCtrl(CWnd* pParent, CRect& rect, LPCTSTR lpszInit, LPCTSTR lpszData, bool bDropList);

	/**
	*  @brief    控件创建之后再加载XML分类数据
	*
	*  @param    const CString& xmlFile 下拉树Xml配置文件（或XML内容字符串）
	*  @param    bool bNoLimitOrSelAll true自动添加不限或全选
	*  @return   bool 成功返回true
	*/
	bool LoadXmlData(const CString& xmlFile, bool bNoLimitOrSelAll = false);

	/**
	*  @brief    加载字符串数组
	*
	*  @param    CStringVector & listData 字符串数组
	*  @param    bool bNoLimitOrSelAll true自动添加不限或全选
	*/
	void LoadStringVector(CStringVector& listData, bool bNoLimitOrSelAll = false);

	/**
	*  @brief    取消编辑
	*
	*/
	void CancelEdit();

	/**
	*  @brief    失去焦点的处理
	*
	*  @param    CWnd * pWnd 句柄
	*/
	void DoKillFocus(CWnd* pWnd);

	/**
	*  @brief    键盘按下的消息处理
	*
	*  @param    UINT nChar
	*  @param    UINT nRepCnt
	*  @param    UINT nFlags
	*  @return   void
	*/
	
	void ProcessKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

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
	afx_msg void OnEditChange();

public:
	WNDPROC			_pEditProc;  ///< 编辑框消息

protected:
	bool			_bCandel;	  ///< 是否取消
	COLORREF		_crNormal;	  ///< 正常颜色
	COLORREF		_crSelected;  ///< 选择颜色
	COLORREF		_crText;      ///< 文本颜色
	CString			_strInit;	  ///< 默认值
	CString			_strData;	  ///< 默认值扩展
	CEditContainer	_container;   ///< 边框容器

	bool			_bDropList;   ///< 无输入框模式
	bool			_bIsSelect;
};