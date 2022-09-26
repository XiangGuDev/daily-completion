////*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           EditXpVer.H
//    @author         LOL
//    @date           2015/12/14 9:15
//    @brief          针对XP不支持CueBanner的替代用Edit控件
//*****************************************************

#pragma once

namespace ControlUI
{
	/**
	*  @class    XP用替代CEdit
	*
	*  @brief    针对XP不支持CueBanner的替代用Edit控件
	*/
	class CONTROL_UI CEditXpVer : public CEdit
	{
		DECLARE_DYNAMIC(CEditXpVer)
	public:
		CEditXpVer();
		virtual ~CEditXpVer();

		/**
		*  @brief    设置提示信息
		*
		*  @param    const CString & 提示文字
		*  @return   void
		*/
		void SetBannerText(const CString& str);

		CString GetBannerText();

		void SetWindowText(LPCTSTR lpszString);
		void GetInputText(CString& strTemp);

	protected:
		DECLARE_MESSAGE_MAP()

	protected:
		afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
		afx_msg void OnKillFocus(CWnd* pNewWnd);
		afx_msg void OnSetFocus(CWnd* pOldWnd);

	private:
		void WhenOnFocus();
		void WhenKillFocus();
	private:
		TCHAR   _chPass;				///< 密码提示符
		COLORREF m_crText;				///< 提示字体的颜色
		CString _strBannerText;			///< 提示文字
	};
}