#include "StdAfx.h"
#include "AngleEdit.h"
#include <cassert>

#define MAX_TEXT_CHAR 12

IMPLEMENT_DYNAMIC(CAngleEdit, CEdit)

BEGIN_MESSAGE_MAP(CAngleEdit, CEdit)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(SWITCH_TO_NEXT_PART, OnSwitchToNextPart)
END_MESSAGE_MAP()

CAngleEdit::CAngleEdit()
{
}

CAngleEdit::~CAngleEdit()
{
}

CString CAngleEdit::GetSeparatorCharPos(INT &degreeCharPos, INT &minuteCharPos, INT &secondCharPos)
{
	degreeCharPos = minuteCharPos = secondCharPos = -1;

	TCHAR textBuf[MAX_TEXT_CHAR] = { 0 };
	GetWindowText(textBuf, MAX_TEXT_CHAR);

	for (INT i = 0; i < MAX_TEXT_CHAR; ++i)
	{
		if (textBuf[i] == DEGREE_CHAR)
			degreeCharPos = i;
		else if (textBuf[i] == MINUTE_CHAR)
			minuteCharPos = i;
		else if (textBuf[i] == SECOND_CHAR)
			secondCharPos = i;
	}

	return textBuf;
}

void CAngleEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// 使用按钮移动到下一个输入点
	if (nChar == VK_SPACE || nChar == VK_DECIMAL)
	{
		// 自动选中下一个输入框
		AutoSelectPart(TRUE);
		return;
	}

	//只能使用左右按键、←、空格、点键, 不允许Shift+箭头选择
	if (((nChar == VK_LEFT || nChar == VK_RIGHT) && !(GetKeyState(VK_SHIFT) & 0x8000)) || nChar == VK_BACK)
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

void CAngleEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bHandle = TRUE;
	if (nChar == 8 /*←*/)
	{
		// 处理删除字符
		bHandle = ProcessDelChart();
	}
	// 过滤输入字符,只能输入 0-9 与 -号
	else if ((nChar >= 48/*0*/ && nChar <= 57/*9*/) || nChar == 45 || /* -号*/ nChar == 8 /*←*/)
	{
		// 检测输入正确性
		bHandle = ValidateInput(nChar);
	}

	if (!bHandle)
		CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CAngleEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// 屏蔽鼠标选择
}

void CAngleEdit::SetEditFocus(void)
{
	SetFocus();

	//查找度字符位
	INT degreeCharPos, minuteCharPos, secondCharPos;
	GetSeparatorCharPos(degreeCharPos, minuteCharPos, secondCharPos);

	SetSel(0, degreeCharPos); /*选中度*/
}

// 处理双时选中范围
void CAngleEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// 在双击位置自动选中输入文本
	AutoSelectPart();
}

void CAngleEdit::AutoSelectPart(BOOL isSwitch)
{
	// 获取选中位置
	INT stPos, edPos;
	GetSel(stPos, edPos);

	INT ptCharPos = edPos;
	ATLASSERT(ptCharPos != -1);

	//查找度字符位
	INT degreeCharPos, minuteCharPos, secondCharPos;
	GetSeparatorCharPos(degreeCharPos, minuteCharPos, secondCharPos);

	ATLASSERT(degreeCharPos != -1 && minuteCharPos != -1 && secondCharPos != -1);

	// 判断光标在哪个数据范围内
	if (ptCharPos <= degreeCharPos)
	{
		isSwitch ? SetSel(degreeCharPos + 1, minuteCharPos) /*选中分*/ : SetSel(0, degreeCharPos); /*选中度*/

	}
	else if (ptCharPos > degreeCharPos && ptCharPos <= minuteCharPos)
	{
		isSwitch ? SetSel(minuteCharPos + 1, secondCharPos)/*选中秒*/ : SetSel(degreeCharPos + 1, minuteCharPos); /*选中分*/

	}
	else if (ptCharPos > minuteCharPos && ptCharPos <= secondCharPos)
	{
		isSwitch ? SetSel(0, degreeCharPos) /*选中度*/ : SetSel(minuteCharPos + 1, secondCharPos); /*选中秒*/

	}
	else if (ptCharPos >= secondCharPos)
	{
		SetSel(0, degreeCharPos); /*选中度*/
	}
}

BOOL CAngleEdit::ProcessDelChart()
{
	// 获取选中位置
	INT stPos, edPos;
	GetSel(stPos, edPos);

	INT ptCharPos = edPos;
	ATLASSERT(ptCharPos != -1);

	//查找度字符位
	INT degreeCharPos, minuteCharPos, secondCharPos;
	GetSeparatorCharPos(degreeCharPos, minuteCharPos, secondCharPos);

	//如果删除的字符为分隔符则只移动到前一个输入框中
	if (ptCharPos == (degreeCharPos + 1) || ptCharPos == (minuteCharPos + 1) || ptCharPos == (secondCharPos + 1))
	{
		SetSel(ptCharPos - 1, ptCharPos - 1);
		return TRUE;
	}

	//如果删除输入框中最后一字数字则把数字改成0并移动到前一个输入框中
	//判断光标在哪个数据范围内
	if (ptCharPos <= degreeCharPos && degreeCharPos - edPos + stPos <= 1)
	{
		SetSel(0, degreeCharPos);
		ReplaceSel(L"0");
		SetSel(0, 1);
		return TRUE;
	}
	else if (ptCharPos > degreeCharPos && ptCharPos <= minuteCharPos &&
		(minuteCharPos - degreeCharPos - edPos + stPos <= 2))
	{
		SetSel(degreeCharPos + 1, minuteCharPos);
		ReplaceSel(L"0");
		SetSel(degreeCharPos, degreeCharPos);
		return TRUE;
	}
	else if (ptCharPos > minuteCharPos && ptCharPos <= secondCharPos &&
		(secondCharPos - minuteCharPos - edPos + stPos <= 2))
	{
		SetSel(minuteCharPos + 1, secondCharPos);
		ReplaceSel(L"0");
		SetSel(minuteCharPos, minuteCharPos);
		return TRUE;
	}
	else if (ptCharPos > secondCharPos)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CAngleEdit::ValidateInput(TCHAR inChar)
{
	//获取文本并查找度字符位
	INT degreeCharPos, minuteCharPos, secondCharPos;
	CString strText = GetSeparatorCharPos(degreeCharPos, minuteCharPos, secondCharPos);
	ATLASSERT(degreeCharPos != -1 && minuteCharPos != -1 && secondCharPos != -1);

	// 获取选中位置
	INT stPos, edPos;
	GetSel(stPos, edPos);
	INT ptCharPos = edPos;

	//负号只在最开始输入
	if (inChar == 45 /*负号*/ && stPos != 0)
		return TRUE;

	// 在最后不能输入字符
	if (edPos >= strText.GetLength())
		return TRUE;

	// 删除选中字符并插入新输入字符, 模拟编缉框输入结果
	strText.Delete(stPos, edPos - stPos);
	strText.Insert(stPos, inChar);

	// 查找新输入位置
	INT degreePos = strText.Find(DEGREE_CHAR, 0);
	INT minutePos = strText.Find(MINUTE_CHAR, degreePos);
	INT secondPos = strText.Find(SECOND_CHAR, minutePos);

	// 检测度输入合法性
	CString strDegree = strText.Left(degreePos);
	INT degreeVal = _ttoi(strDegree);
	if (degreeVal >= 180 || degreeVal <= -180)
		return TRUE; // 输入不合法

	if (ptCharPos <= degreeCharPos)
	{
		// 输入字符不能起过4个
		if (strDegree.GetLength() > 4)
			return TRUE; // 输入不合法

		// 连续输入0自动覆盖
		if (strDegree.Find(L"-") != -1)
		{
			if (strDegree.GetLength() > 2 && degreeVal >= -9)
				SetSel(1, 2);
		}
		else
		{
			if (strDegree.GetLength() > 1 && degreeVal <= 9)
				SetSel(0, 1);
		}

		// 自动切换到下一输入框
		if (degreeVal > 99 || degreeVal < -99 || degreeVal >= 18 || degreeVal <= -18)
			PostMessage(SWITCH_TO_NEXT_PART, 0);

	}
	else if (ptCharPos > degreeCharPos && ptCharPos <= minuteCharPos)
	{
		// 检测分输入合法性
		CString strMinute = strText.Mid(degreePos + 1, minutePos - degreePos - 1);

		// 输入字符不能起过2个
		if (strMinute.GetLength() > 2)
			return TRUE; // 输入不合法

		INT minuteVal = _ttoi(strMinute);
		if (minuteVal > 59 || minuteVal < 0)
			return TRUE; // 输入不合法

		// 连续输入0自动覆盖
		if (strMinute.GetLength() > 1 && minuteVal <= 9)
			SetSel(degreeCharPos + 1, degreeCharPos + 2);

		// 自动切换到下一输入框
		if (minuteVal > 9 || (minuteVal >= 6 && strMinute.GetLength() == 1))
			PostMessage(SWITCH_TO_NEXT_PART, 0);

	}
	else if (ptCharPos > minuteCharPos && ptCharPos <= secondCharPos)
	{
		// 检测分秒入合法性
		CString strSecond = strText.Mid(minutePos + 1, secondPos - minutePos - 1);
		// 输入字符不能起过2个
		if (strSecond.GetLength() > 2)
			return TRUE; // 输入不合法

		INT secondVal = _ttoi(strSecond);
		if (secondVal > 59 || secondVal < 0)
			return TRUE; // 输入不合法

		// 连续输入0自动覆盖
		if (strSecond.GetLength() > 1 && secondVal <= 9)
			SetSel(minuteCharPos + 1, minuteCharPos + 2);

		// 自动切换到下一输入框
		if (secondVal > 9 || (secondVal >= 6 && strSecond.GetLength() == 1))
			PostMessage(SWITCH_TO_NEXT_PART, 1);
	}

	return FALSE;
}

CString CAngleEdit::GetAngleText(void)
{
	CString strRetVal;
	CString strAngle, strMin, strSecond;

	// 获取角度值
	GetAngleStrValue(strAngle, strMin, strSecond);

	if (strAngle.IsEmpty())
		strAngle = _T("0");

	if (strMin.IsEmpty())
		strMin = _T("0");

	if (strSecond.IsEmpty())
		strSecond = _T("0");

	strRetVal.Format(_T("%s°%s′%s″"), strAngle, strMin, strSecond);
	return strRetVal;
}

// 获取角度值
void CAngleEdit::GetAngleStrValue(CString& degreeVal, CString& minuteVal, CString& secondVal)
{
	//查找度字符位
	INT degreeCharPos, minuteCharPos, secondCharPos;
	GetSeparatorCharPos(degreeCharPos, minuteCharPos, secondCharPos);

	CString strText;
	GetWindowText(strText);
	degreeVal = strText.Left(degreeCharPos);
	minuteVal = strText.Left(minuteCharPos).Right(minuteCharPos - degreeCharPos - 1);
	secondVal = strText.Left(secondCharPos).Right(secondCharPos - minuteCharPos - 1);
}

LRESULT	CAngleEdit::OnSwitchToNextPart(WPARAM wParam, LPARAM lParam)
{
	if (!wParam)
	{
		AutoSelectPart(TRUE);
	}
	else
	{
		INT textLen = GetWindowTextLength();
		SetSel(textLen, textLen);
	}
	return TRUE;
}

double CAngleEdit::GetArcValue()
{
	// 获取角度值
	CString degreeVal, minuteVal, secondVal;
	GetAngleStrValue(degreeVal, minuteVal, secondVal);

	long nAngle = _ttol(degreeVal);
	long nMin = _ttol(minuteVal);
	long nSec = _ttol(secondVal);

	if (nAngle < 0)
	{
		nMin = -nMin;
		nSec = -nSec;
	}

	return M_PI * ((double)nAngle + (double)nMin / 60.0f + (double)nSec / 3600.0f) / 180.0f;
}

void CAngleEdit::SetArcValue(double dVal)
{
// 	ASSERT(FALSE);
// 	int nAngle,nMin,nSec;
// 	CGeometry::ArcDegree2AngleDegree(dVal, nAngle, nMin, nSec);
// 
// 	CString strValue;
// 	strValue.Format(_T("%d°%d′%d″"), nAngle, dVal > 0 ? nMin : -nMin, dVal > 0 ? nSec : -nSec);
// 	if (dVal < 0 && nAngle == 0)
// 	{
// 		strValue.Insert(-1, _T("-"));
// 	}
// 	SetWindowText(strValue);
}

double CAngleEdit::GetAngleValue()
{
	// 获取角度值
	CString degreeVal,minuteVal,secondVal;
	GetAngleStrValue(degreeVal, minuteVal, secondVal);

	long nAngle = _ttol(degreeVal);
	long nMin = _ttol(minuteVal);
	long nSec = _ttol(secondVal);

	if (nAngle < 0)
	{
		nMin = -nMin;
		nSec = -nSec;
	}

	return (double)nAngle + (double)nMin / 60.0f + (double)nSec / 3600.0f;
}

void CAngleEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	CString strDegree, strMinute, strSecond;
	GetAngleStrValue(strDegree, strMinute, strSecond);
	if (strDegree == _T("-"))
	{
		if ((strMinute == _T("0")) && (strSecond == _T("0")))
		{
			strDegree = _T("0");
		}
		else
		{
			strDegree = _T("-0");
		}
	}
	if (strDegree == _T("-0") && strMinute == _T("0") && strSecond == _T("0"))
	{
		strDegree = _T("0");
	}
	CString strValue;
	strValue.Format(_T("%s°%s′%s″"), strDegree, strMinute, strSecond);

	SetWindowText(strValue);
}
