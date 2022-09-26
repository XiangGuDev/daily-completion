//*****************************************************
//
//    @copyright	  配网设计组
//	  @version		  v1.0
//	  @file			  ColorComboBox.H
//	  @author		  lezhihao
//	  @date			  2019/01/09 10:04
//    @brief          颜色组合框 
//*****************************************************

#pragma once

namespace ControlUI
{
	/*
	*   @class CColorComboBox
	*
	*   @brief 颜色组合下拉框
	*/
	class CONTROL_UI CColorComboBox :public CComboBox
	{
		DECLARE_DYNAMIC(CColorComboBox)

	public:
		CColorComboBox();
		virtual ~CColorComboBox();

	public:
		int AddItem(LPCTSTR lpszText, COLORREF clrValue);
		CString GetCurText();
		COLORREF GetCurColor();
		virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

		int GetColorItemDataIndex(const COLORREF & color);
	protected:
		DECLARE_MESSAGE_MAP()
	};
}
