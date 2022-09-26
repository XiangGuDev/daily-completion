////*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           EditValue.H
//    @author         JCC
//    @date           2018/9/5 18:23
//    @brief		  可以获取上一次输入的编辑框
//*****************************************************

#pragma once

namespace ControlUI
{
	class CONTROL_UI CEditValue : public CEdit
	{
	public:
		CEditValue();
		~CEditValue();

		CString GetOldValue();


		// 实现  
	protected:
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags); //声明消息  
		DECLARE_MESSAGE_MAP()

	private:
		CString m_strOldValue; // 编辑框非空前的一次值
	};

}