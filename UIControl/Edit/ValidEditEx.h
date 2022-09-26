//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           ValidEdit.H
//    @author         KG
//    @date           2015/3/7 10:41
//    @brief          校验编辑控件
//*****************************************************

#pragma once

namespace ControlUI
{
	/**
	*  @class    校验编辑控件
	*
	*  @brief
	*/
	class CONTROL_UI CValidEditEx : public CValidEdit
	{
	public:
		CValidEditEx();
		virtual ~CValidEditEx();

		/**
		*  @brief    设置最大最小输入值
		*
		*  @param	 double dblMax  最大值
		*  @param	 double dblMin  最小值
		*  @param	 bMaxEnabled 是否允许输入最大值
		*  @param	 bMinEnabled 是否允许输入最小值
		*/
		void SetMaxMinValueValidate(double dblMax, double dblMin, bool bMaxEnabled = false, bool bMinEnabled = false);

		bool ValidateMaxMinValue();

	protected:
		DECLARE_MESSAGE_MAP()

		/**
		*  @brief    失去焦点
		*
		*  @param    CWnd * pNewWnd
		*  @return   void
		*/
		virtual void OnKillFocus(CWnd* pNewWnd);

	private:
		double				_dMaxValue;		///< 最大值 
		double				_dMinValue;		///< 最小值
		bool				_bMaxEnabled;	///< 是否允许输入最大值
		bool				_bMinEnabled;	///< 是否允许输入最小值
	};
}