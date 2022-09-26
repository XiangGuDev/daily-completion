//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           ValidEdit.H
//    @author         KG
//    @date           2015/3/7 10:41
//    @brief          校验编辑控件
//*****************************************************

namespace ControlUI
{
#pragma once

	/**
	*  @class    校验编辑控件
	*
	*  @brief
	*/
	class CONTROL_UI CValidEdit : public CEdit
	{
		DECLARE_DYNAMIC(CValidEdit)
	public:
		CValidEdit();
		virtual ~CValidEdit();

	protected:
		DECLARE_MESSAGE_MAP()

	public:
		/**
		*  @brief    检查输入是否合法(检测是否空输入)
		*
		*  @return	 bool 输入合法返回true
		*  @remark	 当设置eValidEditNoZero或eValidEditFileName时
		*			 需要在窗口关闭时调用进行检测，如果返回false则需要取消关闭窗口
		*/
		bool Validate();

		/**
		*  @brief    设置编辑标志

		*  @param	 int flag	标志
		*/
		void SetEditFlag(int nFlag);

		/**
		*  @brief    设置小数位数
		*
		*  @param	 int nDotNum  小数位数
		*/
		void SetDotNum(int nDotNum);

		/**
		*  @brief    气泡提示信息
		*
		*/
		void ShowBalloonTip(const CString& strTitle,
			const CString& strContent, int tipIcon = TTI_INFO, DWORD dwTimeOut = 0);

		/**
		*  @brief    气泡提示内容
		*
		*/
		void SetTitleAndContent(const CString& strTitle, const CString& strContent) 
		{ 
			_strTitle = strTitle;
			_strContent = strContent;
		};

		/**
		*  @brief    设置最大最小输入值
		*
		*  @param	 double dblMax  输入最大值
		*  @param	 double dblMin  输入最小值
		*  @param	 bool bShowLimitTip  是否显示范围提示信息
		*/
		void SetMaxMinValue(double dblMax, double dblMin, bool bShowLimitTip);

		/**
		*  @brief    设置约束条件
		*
		*  @param    const CString & strConstraint 约束条件(逻辑表达式)
		*  @param	 bool bShowLimitTip  是否显示范围提示信息
		*/
		void SetConstraint(const CString& strConstraint, bool bShowLimitTip);

		/**
		*  @brief    获取编辑标志

		*  @param	 int flag	标志
		*/
		int GetEditFlag() { return _nFlag; }

		/**
		*  @brief    禁用冒泡提示

		*  @param	 int flag	标志
		*/
		void DisableTip(bool bDisable) { 
			if (_bIsShowingTip)
			{
				_bIsShowingTip = false;
				Edit_HideBalloonTip(m_hWnd);
			}
			_bDisableTip = bDisable; 
		}

		/**
		*  @brief    是否正在显示提示

		*  @param	 int flag	标志
		*/
		bool IsShowingTip() { return _bIsShowingTip; }

	protected:
		/**
		*  @brief    与WM_PASTE消息对应函数
		*/
		LRESULT OnPaste(WPARAM wParam, LPARAM lParam);

		/**
		*  @brief    与WM_CHAR消息对应函数
		*/
		LRESULT OnChar(WPARAM wParam, LPARAM lParam);

		/**
		*  @brief    定时器
		*/
		void OnTimer(UINT_PTR timer);

		/*
		*  @brief    判断输入是否是数字
		*
		*  @param	 const CString& strText  检查文本
		*/
		virtual bool IsDigit(const CString& strText);

		/*
		*  @brief    判断输入是否正确
		*
		*  @param	 bool bRound  是否四舍五入
		*/
		virtual bool IsDigitValidate(bool bRound = false);

		/**
		*  @brief    判断输入的值是否在允许的范围内，不在则弹气泡提示
		*
		*  @return   bool 在范围内返回true，否则返回false
		*/
		virtual bool InRange();

		/*
		*  @brief    判断文件名是否合法
		*
		*/
		virtual bool IsFileNameValidate();

		/**
		*  @brief    检查输入是否正确
		*
		*  @param	 bool bRound  是否四舍五入
		*  @return	 bool 成功返回true
		*/
		bool CheckInvaildate(bool bRound = false);

	protected:
		bool				_bInPaste;		///< 是否正在粘贴
		int					_nFlag;			///< 控制标记
		int					_dotNum;		///< 允许输入小数位数
		double				_maxValue;		///< 允许输入最大值 
		double				_minValue;		///< 允许输入最小值
		bool				_bShowLimitTip;	///< 是否显示数值区域提示
		bool				_bDisableTip;	///< 禁用提示信息
		bool				_bIsShowingTip;	///< 是否正在显示提示
		CString				_strConstraint;	///< 约束条件
		CString				_strTitle;		///< 标题
		CString				_strContent;	///< 内容
	};
}