#include "StdAfx.h"
#include "ValidEdit.h"

// 关闭提示框定时器
#define TM_BALLOONTIP 2015

IMPLEMENT_DYNAMIC(CValidEdit, CBCGPEdit)

CValidEdit::CValidEdit()
{
	_digitNum = 8;
	_dotNum = 8;
	_bInPaste = false;
	_nFlag = eNormalEdit;
	_maxValue = 1000000000000000;
	_minValue = -1000000000000000;
	_bShowLimitTip = false;
}

CValidEdit::~CValidEdit()
{
}

BEGIN_MESSAGE_MAP(CValidEdit, CBCGPEdit)
	ON_MESSAGE(WM_CHAR, OnChar)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_WM_TIMER()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

void CValidEdit::SetEditFlag(int nFlag)
{
	_nFlag = nFlag;
}

void CValidEdit::SetDotNum(int nDotNum)
{
	_dotNum = nDotNum;
}

void CValidEdit::SetDigitNum(int nDigitNum)
{
	_digitNum = nDigitNum;
}

bool CValidEdit::Validate()
{
	CString strText;
	GetWindowText(strText);
	strText.Trim();

	if ((_nFlag & eFileNameEdit) && strText.IsEmpty())
	{
		ShowBalloonTip(L"文件名不能为空", L"请输入正确的文件名。");
		return false;
	}
	if ((_nFlag & eVeNoEmpty) && strText.IsEmpty())
	{
		ShowBalloonTip(L"数值不能为空", L"请输入正确的数值。");
		return false;
	}

	double dText = CStringToolkit::StrToDouble((LPCTSTR)strText);
	if ((_nFlag & eVeNoZero) && FLOAT_EQUAL8(dText, 0.0))
	{
		ShowBalloonTip(L"数值不能为零", L"请输入正确的数值。");
		return false;
	}

	if (!(_nFlag & eFileNameEdit) && dText < _minValue)
	{
		double minValue = _minValue;

		if (!(_nFlag & eVeNegative) && minValue < 0)
		{
			minValue = 0;
		}

		CString strLimit, strFormat;

		strFormat.Format(L"您只能在此键入 %%.%df 到 %%.%df 范围的数值。", _dotNum, _dotNum);
		strLimit.Format(strFormat, minValue, _maxValue);

		if (_bShowLimitTip)
			ShowBalloonTip(L"不能接受的数值范围", strLimit);

		return false;
	}

	return true;
}

LRESULT CValidEdit::OnPaste(WPARAM wParam, LPARAM lParam)
{
	// 普通编辑框
	if (_nFlag & eNormalEdit)
	{
		Default();
		return 0;
	}

	// 检查输入合法性
	if ( !CheckInvaildate(true) )
	{
		_bInPaste = true;

		if (_nFlag & eFileNameEdit)
		{
			ShowBalloonTip(L"文件名不能包含下列任何字符:", L"		\\ / : * ? \" < > |");
		}
		else
		{
			ShowBalloonTip(L"不能接受的格式", L"您只能在此粘贴正确的数值。");
		}
	}

	return 0;
}

LRESULT CValidEdit::OnChar(WPARAM wParam, LPARAM lParam)
{
	// 普通编辑框
	if (_nFlag & eNormalEdit)
	{
		Default();
		return 0;
	}

	_bInPaste = false;

	if (wParam == L'。')
		wParam = L'.';

	// 不符合条件字符(空格与TAB不允许输入)
	if (!(_nFlag & eFileNameEdit) 
			&& (wParam == L' ' || wParam > 127) )
	{
		ShowBalloonTip(L"不能接受的字符", L"您只能在此键入数值。");
		return 0;
	}

	// 检查输入合法性
	CheckInvaildate();

	return 0;
}

bool CValidEdit::CheckInvaildate(bool bRound)
{
	bool bResult = false;

	CString strText;
	GetWindowText(strText);
	DWORD curSel = GetSel();

	SetRedraw(FALSE);
	Default();

	// 条件不符合还原
	if ((_nFlag & eFileNameEdit) ?
		!IsFileNameValidate() : !IsDigitValidate(bRound) )
	{
		SetWindowText(strText);
		SetSel(curSel);
	}
	else
	{
		bResult = true;

		// 隐藏气泡提示,不在粘贴时
		if (!_bInPaste)
			Edit_HideBalloonTip(m_hWnd);
	}

	SetRedraw(TRUE);
	Invalidate();

	return bResult;
}

bool CValidEdit::IsFileNameValidate()
{
	CString strText;
	GetWindowText(strText);

	TCHAR* pStrInv = L"\\/:*?\"<>|";
	bool bResult = (-1 == strText.FindOneOf(pStrInv));

	if ( !bResult )
	{
		ShowBalloonTip(L"文件名不能包含下列任何字符:", L"		\\ / : * ? \" < > |");
	}

	return bResult;
}

bool CValidEdit::IsDigit(const CString& strText)
{
	bool bFoundNum = false;
	if ( strText.IsEmpty() )
		return true; 

	if ( strText.GetLength() == 1 && strText == _T("-") )
		return true;

	if ( strText.Left(1) == _T("."))
		return false;

	int dotCt = 0; // 点号计数器，有多个点认为无效
	UINT ct = strText.GetLength();
	for( UINT i = 0; i < ct; ++i )
	{
		const TCHAR chr = strText.GetAt(i);
		// 当前字符为符号
		if (chr == 45/*"-"*/|| chr == 43 /*"+"*/)
		{
			// 不在第一位
			if ( i != 0 ) return false;
			continue;
		}
		// 点号计数器
		if ( chr == 46 /*"."*/ ) 
		{
			dotCt++;
			// 超过两个点
			if ( dotCt > 1 ) return false;
			continue;
		}
		// 数字范围
		if ( chr < 48 || chr > 57 )
			return false;

		bFoundNum = true;
	}
	return bFoundNum;
}

bool CValidEdit::FloatEqual(double dOriValue, CString strTag, int nDec)
{
	CString strOri = CStringToolkitEx::DblToStr(dOriValue);
	if (strOri == strTag)
		return true;

	int nIndex = strOri.Find(L".");
	int nIndexTag = strTag.Find(L".");

	CString strOriNew, strTagNew;
	strOriNew = nIndex == -1 ? strOri : strOri.Left(nIndex + nDec + 1);
	strTagNew = nIndexTag == -1 ? strTag : strTag.Left(nIndexTag + nDec + 1);
	strOriNew.TrimRight(L".");
	strTagNew.TrimRight(L".");
	bool bRet = (strOriNew == strTagNew);
	return (strOriNew == strTagNew);
}

bool CValidEdit::IsDigitValidate(bool bRound)
{	
	CString strText;
	GetWindowText(strText);

	if ( strText.IsEmpty() )
		return true;

	// 空格与TAB不允许输入
	if ( strText.Find(L" ") != -1 || strText.Find(L"\t") != -1 )
		return false;

	int nChDot = strText.Replace(L"。", L".");

	// 不是数字
	if ( !IsDigit(strText) )
	{
		ShowBalloonTip(L"不能接受的字符", L"您只能在此键入数值。");
		return false;
	}

	// 正数检查
	if ((strText.Left(1) != L"-") && (strText.Left(1) != L"0") && (_nFlag & eVeNotPositive))
	{
		ShowBalloonTip(L"不能接受正数", L"您只能在此键入负数。");
		return false;
	}

	// 负数检查
	if ( (strText.Left(1) == L"-") && !(_nFlag & eVeNegative) )
	{
		ShowBalloonTip(L"不能接受负数", L"您只能在此键入正数。");
		return false;
	}

	// 限制范围
	if ((_ttof(strText) > 100 || _ttof(strText) < 0)
		&& (_nFlag & eVeLimitRange))
	{
		ShowBalloonTip(L"数值超出范围", L"您只能在此键入0-100之间的数。");
		return false;
	}

	if (!(_nFlag & eVePreZero))
	{
		// 整数前置0检查
		if (!(_nFlag & eVeDecimal))
		{
			// 不允许输入0时检测
			if (((strText.Left(1) == L"0" || strText.Left(2) == L"-0")
				&& (_nFlag & eVeNoZero))
				|| ((strText.Left(2) == L"-0" || (strText.GetLength() > 1
				&& strText.Left(1) == L"0")) && !(_nFlag & eVeNoZero))) // 允许输入0时检测
			{
				ShowBalloonTip(L"不能接受的格式", L"您只能在此键入正确的整值。");
				return false;
			}
		}
		// 小数前置0检查
		else if ((strText.Left(2) == L"-.")
			|| ((strText.Left(1) == L"0") &&
			(strText.GetLength() > 1 && strText.Mid(1, 1) != L"."))	//0x 情况
			|| ((strText.Left(2) == L"-0") &&
			(strText.GetLength() > 2 && strText.Mid(2, 1) != L".")))  //-0x 情况
		{
			ShowBalloonTip(L"不能接受的格式", L"您只能在此键入正确的小数。");
			return false;
		}
	}

	// 获取小数位数
	int dotPos = strText.Find(L".");
	int dotNum = (dotPos!=-1)? strText.GetLength()-dotPos-1:0;

	// 获取当前位数
	int digitNum = strText.GetLength();

	// 最大位数检查
	if (digitNum > _digitNum && (_nFlag & eVeDigitLimit))
	{
		CString strDigitNum;
		strDigitNum.Format(L"您只能在此键入%d位。", _digitNum);
		ShowBalloonTip(L"不能接受的最大位数", strDigitNum);
		return false;
	}

	// 整数检查
	if ((dotNum > 0 || dotNum == 0 && dotPos != -1) && !(_nFlag & eVeDecimal))
	{
		ShowBalloonTip(L"不能接受的字符", L"您只能在此键入整数。");
		return false;
	}

	// 小数位置检查
	if ( dotNum > _dotNum && (_nFlag & eVeDotNum) )
	{
		if ( !bRound )
		{
			CString strDotNum;
			strDotNum.Format(L"您只能在此键入%d位小数。",_dotNum);
			ShowBalloonTip(L"不能接受的小数位数", strDotNum);
			return false;
		}

		// 进行四舍五入
		CString strFormat;
		strFormat.Format(L"%%.%df", _dotNum);

		double val = _wtof(strText);
		CString strValue;
		strValue.Format(strFormat, val);
		SetWindowText(strValue);

		return true;
	}
	
	double dblValue = _wtof(strText);
	int nIndex = strText.Find(L".");
	int nDot = nIndex == -1 ? 0 : strText.GetLength() - nIndex - 1;
	if (dblValue > _maxValue || (!DataConvert::FloatEqual(DBL_MIN, _minValue, nDot) && !FloatEqual(_minValue, strText, nDot) && dblValue < _minValue))
	{
		double minValue = _minValue;

		if (!(_nFlag & eVeNegative) && minValue < 0)
		{			
			minValue = 0;
		}

		CString strLimit,strFormat;

		strFormat.Format(L"您只能在此键入 %%.%df 到 %%.%df 范围的数值。", _dotNum, _dotNum);
		strLimit.Format(strFormat, minValue, _maxValue);

		if (_bShowLimitTip)
			ShowBalloonTip(L"不能接受的数值范围", strLimit);

		return false;
	}

	if (nChDot == 1)
	{
		SetWindowText(strText);
		int nTextLength = strText.GetLength();
		SetSel(nTextLength, nTextLength, TRUE);
	}
	
	return true;
}

void CValidEdit::OnTimer(UINT_PTR timer)
{
	if (timer == TM_BALLOONTIP)
	{
		KillTimer(TM_BALLOONTIP);
		Edit_HideBalloonTip(m_hWnd);
	}
}

void CValidEdit::ShowBalloonTip(const CString& strTitle,
	const CString& strContent, int tipIcon, DWORD dwTimeOut)
{
	EDITBALLOONTIP info = {0};
	info.cbStruct = sizeof(EDITBALLOONTIP);
	info.pszTitle = strTitle;
	info.pszText = strContent;
	info.ttiIcon = tipIcon;

	// 显示气泡提示
	Edit_ShowBalloonTip(m_hWnd, &info);

	// 开启关闭提示
	if (dwTimeOut != 0)
	{
		SetTimer(TM_BALLOONTIP, dwTimeOut, NULL);
	}
}

void CValidEdit::SetMaxMinValue(double dblMax, double dblMin, bool bShowLimitTip)
{
	_maxValue = dblMax;
	_minValue = dblMin;
	_bShowLimitTip = bShowLimitTip;
}
