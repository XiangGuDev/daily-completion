//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           AngleEdit.H
//    @author         KG
//    @date           2015/3/8 10:12
//    @brief          角度编辑框控件(含有°’”)
//*****************************************************

#pragma once

// 度，分，秒切换消息(内置)
#define SWITCH_TO_NEXT_PART (WM_USER+140)

/**
*  @class    角度编辑框控件类
*
*  @brief
*/
class COMMONCONTROLS_API CAngleEdit : public CEdit
{
	DECLARE_DYNAMIC(CAngleEdit)
public:
	CAngleEdit();
	virtual ~CAngleEdit();

protected:
	DECLARE_MESSAGE_MAP()

public:
	/**
	*  @brief    获取角度分量字符串值
	*
	*  @param    CString & degreeVal 度
	*  @param    CString & minuteVal 分
	*  @param    CString & secondVal 秒
	*/
	void GetAngleStrValue(CString& degreeVal,
		CString& minuteVal, CString& secondVal);

	/**
	*  @brief    获得角度文本
	*
	*  @return   CString 返回完整角度字符串
	*/
	CString GetAngleText();

	/**
	*  @brief    获得弧度值
	*
	*  @return   double 返回弧度值
	*/
	double GetArcValue();

	/**
	*  @brief    设置弧度值
	*
	*  @param    double dVal 弧度值
	*/
	void SetArcValue(double dVal);

	/**
	*  @brief    获得角度值
	*
	*  @return   double 返回角度值
	*/
	double GetAngleValue();

	/**
	*  @brief    设置焦点
	*
	*/
	void SetEditFocus();

private:
	/**
	*  @brief    获取分隔符字符位置
	*
	*  @param    INT & degreeCharPos 度位置
	*  @param    INT & minuteCharPos 分位置
	*  @param    INT & secondCharPos 秒位置
	*  @return   CString 返回格式化后的文本
	*/
	CString GetSeparatorCharPos(INT &degreeCharPos,
		INT &minuteCharPos, INT &secondCharPos);

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
	BOOL ValidateInput(TCHAR inChar);

	/**
	*  @brief    根据当前光标位置自动选中文字输入部份
	*
	*  @param    BOOL isSwitch 是否选中下一个输入部份
	*/
	void AutoSelectPart(BOOL isSwitch = FALSE);

public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT	OnSwitchToNextPart(WPARAM wParam, LPARAM lParam);
	/**
	*  @brief    失去焦点事件;
	*
	*/
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};