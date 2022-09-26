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

/**
*  @class    校验编辑控件
*
*  @brief    
*/
class COMMONCONTROLS_API CValidEdit : public CBCGPEdit
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
	*  @brief    设置最大位数
	*
	*  @param	 int nDigitNum  最大位数
	*/
	void CValidEdit::SetDigitNum(int nDigitNum);

	/**
	*  @brief    气泡提示信息
	*
	*/
	void ShowBalloonTip(const CString& strTitle,
		const CString& strContent, int tipIcon = TTI_INFO, DWORD dwTimeOut = 0);

	/**
	*  @brief    设置最大最小输入值
	*
	*  @param	 double dblMax  输入最大值
	*  @param	 double dblMin  输入最小值
	*  @param	 bool bShowLimitTip  是否显示范围提示信息
	*/
	void SetMaxMinValue(double dblMax, double dblMin, bool bShowLimitTip);

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

	/**
	*  函数介绍:    检测相同位数的小数是否相等
	*  输入参数:	double dOriValue
	*  输入参数:	CString strTag
	*  输入参数:	int nDec
	*  返回值:   	bool
	*/
	bool FloatEqual(double dOriValue, CString strTag, int nDec);

protected:
	bool				_bInPaste;		///< 是否正在粘贴
	int					_nFlag;			///< 控制标记
	int					_digitNum;		///< 限制最大位数
	int					_dotNum;		///< 允许输入小数位数
	double				_maxValue;		///< 允许输入最大值 
	double				_minValue;		///< 允许输入最小值
	bool				_bShowLimitTip;	///< 是否显示数值区域提示
};