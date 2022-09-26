//*****************************************************
//    
//    @copyright      配网设计组;
//    @version        v3.0
//    @file           ListDataTime.H
//    @author         mahaidong;
//    @date           2015/4/10 15:45
//    @brief          列表日期编辑框;
//*****************************************************

#pragma once

#include "EditBase.h"
#include "EditContainer.h"

namespace ControlUI
{
	/**
	*  @class    CListDataTime
	*
	*  @brief    列表日期编辑框
	*/
	class CListDataTime : public CDateTimeCtrl, public IEditBase
	{
		DECLARE_DYNAMIC(CListDataTime)

	public:
		CListDataTime();
		virtual ~CListDataTime();

	protected:
		DECLARE_MESSAGE_MAP()

	public:
		/**
		*  @brief    创建控件;
		*
		*  @param    CWnd * pParent 父窗口句柄;
		*  @param    CRect & rect 绘制区域;
		*  @param    LPCTSTR sInitText 默认显示文本;
		*  @param	 bool bAdd    创建新的样式  支持 年度增加	  需要添加传true
		*  @return   bool 创建成功返回true，否则返回false;
		*/
		bool Create(CWnd* pParent, CRect& rect, LPCTSTR sInitText, bool bAdd = false);

		/**
		*  @brief    取消编辑
		*
		*/
		void CancelEdit();

	public:
		afx_msg void OnKillFocus(CWnd* pNewWnd);
		afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	protected:
		bool			_bCandel;	  ///< 是否取消
		CEditContainer	_container;   ///< 边框容器
	};
}