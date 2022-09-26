////*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           NumEdit.H
//    @author         JCC
//    @date           2018/9/5 17:11
//    @brief		  限制数字输入的编辑框
//*****************************************************
#pragma once

namespace ControlUI
{
	class CONTROL_UI CNumEdit : public CEdit
	{
	public:
		CNumEdit();
		~CNumEdit();

		// 设置小数位数
		int SetDecimalNum(unsigned int iNum);

		// 设置是否只能是正数
		int SetIsPositiveNumber(bool bIsOrNot);

		// 设置是否只能是非负数数
		void SetIsCanZero(bool bIsOrNot);

		// 设置是否只能是整数
		int SetIsInteger(bool bIsOrNot);

		// 实现  
	protected:
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags); //声明消息
		afx_msg void OnKillFocus(CWnd* pNewWnd); // 失去焦点
		afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
		DECLARE_MESSAGE_MAP()

		/**
		*  @brief    整理字符
		*
		*  @param    const CString & strText
		*  @return   CString
		*/
		CString TrimString(const CString& strText);

	private:
		unsigned int m_decimalNum; // 小数位数
		bool m_bIsPositiveNumber; // 是否只能是正数
		bool m_bIsCanZero; // 是否能为0
		bool m_bIsInteger; // 是否只能是整数
	};
}