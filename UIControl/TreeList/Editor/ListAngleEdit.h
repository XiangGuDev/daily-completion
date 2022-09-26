//*****************************************************
//    
//    @copyright      配网设计组;
//    @version        v3.0
//    @file           ListAngleEdit.H
//    @author         mahaidong;
//    @date           2015/4/10 15:45
//    @brief          列表角度编辑框;
//*****************************************************

#pragma once

#include "EditBase.h"

namespace ControlUI
{

	/**
	*  @class    CListAngleEdit
	*
	*  @brief    列表角度编辑框
	*/
	class CListAngleEdit : public CAngleEdit, public IEditBase
	{
		DECLARE_DYNAMIC(CListAngleEdit)

	public:
		CListAngleEdit();
		virtual ~CListAngleEdit();

	protected:
		DECLARE_MESSAGE_MAP()

	public:
		/**
		*  @brief    创建控件;
		*
		*  @param    CWnd * pParent 父窗口句柄;
		*  @param    CRect & rect 绘制区域;
		*  @param    LPCTSTR sInitText 弧度制角度;
		*  @return   bool 创建成功返回true，否则返回false;
		*/
		bool Create(CWnd* pParent, CRect& rect, LPCTSTR sInitText);

	protected:
		/**
		*  @brief    取消编辑
		*
		*/
		void CancelEdit();

		/**
		*  @brief    结束编辑
		*
		*/
		void EndEdit();

		/**
		*  @brief    失去焦点事件;
		*
		*/
		afx_msg void OnKillFocus(CWnd* pNewWnd);

		/**
		*  @brief    键盘按键事件;
		*
		*/
		afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	protected:
		bool				_bCancel;		///< 是否取消;
	};
}