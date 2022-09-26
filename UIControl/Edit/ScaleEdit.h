//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           ScaleEdit.H
//    @author         zxw
//    @date           2015/10/15 17:50
//    @brief          带比例编辑框控件(含有：)
//*****************************************************

#pragma once

namespace ControlUI
{
	/**
	*  @class    比例编辑框控件类
	*
	*  @brief
	*/
	class CONTROL_UI CScaleEdit : public CEdit
	{
		DECLARE_DYNAMIC(CScaleEdit)
	public:
		CScaleEdit();
		virtual ~CScaleEdit();

	protected:
		DECLARE_MESSAGE_MAP()
	private:
		/**
		*  @brief    处理删除字符逻辑
		*
		*  @return   BOOL 成功返回TRUE
		*/
		BOOL ProcessDelChart();

		/**
		*  @brief    检测输入正确性
		*
		*  @param    TCHAR inChar 当前输入的字符
		*  @return   BOOL 输入有效返回TRUE
		*/
		BOOL ValidateInput();

		/**
		*  @brief    检查输入是否正确
		*
		*  @return	 bool 成功返回true
		*/
		bool CheckInvaildate();

		/*
		*  @brief    判断输入是否正确
		*
		*/
		virtual bool IsDigitValidate(CString strTex);

		/*
		*  @brief    判断输入是否是数字
		*
		*  @param	 const CString& strText  检查文本
		*/
		virtual bool IsDigit(const CString& strText);

	public:
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	};
}