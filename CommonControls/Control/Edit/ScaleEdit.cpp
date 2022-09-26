#include "StdAfx.h"
#include "ScaleEdit.h"

#define DOT_NUM 3

IMPLEMENT_DYNAMIC(CScaleEdit, CEdit)

BEGIN_MESSAGE_MAP(CScaleEdit, CEdit)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
END_MESSAGE_MAP()

CScaleEdit::CScaleEdit()
{
}

CScaleEdit::~CScaleEdit()
{
}

void CScaleEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// 使用按钮移动到下一个输入点
	if (nChar == VK_SPACE || nChar == VK_DECIMAL)
		return;

	//只能使用左右按键、←、空格、点键, 不允许Shift+箭头选择
	if (((nChar == VK_LEFT || nChar == VK_RIGHT) && !(GetKeyState(VK_SHIFT) & 0x8000)) || nChar == VK_BACK)
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

void CScaleEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bHandle = TRUE;
	DWORD curSel = GetSel();
	if (nChar == 8 /*←*/)
	{
		// 处理删除字符
		bHandle = ProcessDelChart();
	}
	
	CheckInvaildate();
}

bool CScaleEdit::CheckInvaildate()
{
	bool bResult = false;	DWORD curBeforeSel = GetSel();

	CString strChangeBeforeText;
	GetWindowText(strChangeBeforeText);
	// 获取选中位置
	INT stPos, edPos;
	GetSel(stPos, edPos);

	SetRedraw(FALSE);
	Default();

	CString strText;
	GetWindowText(strText);
	DWORD curSel = GetSel();

	INT scaleCharPos = strText.Find(SCALE_CHAR);
	INT endCharPos = strText.GetLength();

	if (scaleCharPos ==-1 )
	{
		SetWindowText(strChangeBeforeText);
		SetSel(edPos, edPos);
	}

	// 条件不符合还原
	if (!IsDigitValidate(strText.Left(scaleCharPos))
		|| !IsDigitValidate(strText.Right(endCharPos-scaleCharPos-1)))
	{
		SetWindowText(strChangeBeforeText);
		SetSel(curBeforeSel);
	}

 	else
	{
		bResult = true;
	}

	SetRedraw(TRUE);
	Invalidate();

	return bResult;
}

BOOL CScaleEdit::ProcessDelChart()
{
	CString strText;
	GetWindowText(strText);

	//字符串长度
	int nLeng = strText.GetLength();

	// 获取选中位置
	INT stPos, edPos;
	GetSel(stPos, edPos);

	INT ptCharPos = edPos;
	ATLASSERT(ptCharPos != -1);

	//查找度字符位
	INT scaleCharPos = strText.Find(SCALE_CHAR);
	INT endCharPos = strText.GetLength();

	//如果删除的字符为分隔符则只移动到前一个输入框中
	if (ptCharPos == (scaleCharPos + 1) || ptCharPos == (endCharPos + 1))
	{
		SetSel(ptCharPos - 1, ptCharPos - 1);
		return TRUE;
	}
	

	else if ((stPos == 0) && (edPos == nLeng))
	{
		SetSel(ptCharPos, ptCharPos);
		return TRUE;
	}
	
	return FALSE;
}

bool CScaleEdit::IsDigit(const CString& strText)
{
	bool bFoundNum = false;
	if (strText.IsEmpty())
		return true;

	if (strText.Left(1) == _T("."))
		return false;

	int dotCt = 0; // 点号计数器，有多个点认为无效
	UINT ct = strText.GetLength();
	for (UINT i = 0; i < ct; ++i)
	{
		const TCHAR chr = strText.GetAt(i);
		// 当前字符为符号
		if (chr == 45/*"-"*/ || chr == 43 /*"+"*/)
		{
			// 不能存在符号
			return false;
		}
		// 点号计数器
		if (chr == 46 /*"."*/)
		{
			dotCt++;
			// 超过两个点
			if (dotCt > 1) return false;
			continue;
		}
		// 数字范围
		if (chr < 48 || chr > 57)
			return false;

		bFoundNum = true;
	}
	return bFoundNum;
}

bool CScaleEdit::IsDigitValidate(CString strText)
{
	// 空格与TAB不允许输入
	if ( strText.Find(L" ") != -1 || strText.Find(L"\t") != -1 )
		return false;

	// 不是数字
	if (!IsDigit(strText))
		return false;

	// 小数前置0检查
	if ((strText.Left(2) == L"-.")
		|| ((strText.Left(1) == L"0") &&
		(strText.GetLength() > 4 && strText.Mid(1, 1) != L"."))	//0x 情况
		|| ((strText.Left(2) == L"-0") &&
		(strText.GetLength() > 2 && strText.Mid(2, 1) != L".")))  //-0x 情况
	{
		return false;
	}

	// 获取小数位数
	int dotPos = strText.Find(L".");
	int dotNum = (dotPos != -1) ? strText.GetLength() - dotPos: 0;

	// 小数位置检查
	if (dotNum > DOT_NUM)
		return false;

	// 检查输入整数部分最大不超过12位
	CString strDotLeftText = (dotPos != -1) ? strText.Left(dotPos) : strText;
	strDotLeftText.Trim(L'-');
	if (strDotLeftText.GetLength() > 16)
		return false;

	return true;
}