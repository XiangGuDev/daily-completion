#include "StdAfx.h"
#include "ValidEditEx.h"

namespace ControlUI
{

	BEGIN_MESSAGE_MAP(CValidEditEx, CValidEdit)
		ON_WM_KILLFOCUS()
	END_MESSAGE_MAP()

	CValidEditEx::CValidEditEx():
		_dMaxValue(100000000000000),
		_dMinValue(-100000000000000),
		_bMaxEnabled(false),
		_bMinEnabled(false)
	{
	}

	CValidEditEx::~CValidEditEx()
	{
	}

	void CValidEditEx::SetMaxMinValueValidate(double dblMax, double dblMin, bool bMaxEnabled/* = false*/, bool bMinEnabled/* = false*/)
	{
		_dMaxValue = dblMax;
		_dMinValue = dblMin;
		_bMaxEnabled = bMaxEnabled;
		_bMinEnabled = bMinEnabled;
	}

	bool CValidEditEx::ValidateMaxMinValue()
	{
		CString strText;
		GetWindowText(strText);

		double dblValue = _wtof(strText);
		if (dblValue > _dMaxValue || (!_bMaxEnabled && abs(dblValue- _dMaxValue)<1e-6) || dblValue < _dMinValue || (!_bMinEnabled && abs(dblValue - _dMinValue)<1e-6))
		{
			double minValue = _dMinValue;

			if (!(_nFlag & eVeNegative) && minValue < 0)
			{
				minValue = 0;
			}

			CString strLimit, strFormat;

			strFormat.Format(L"您只能在此键入 %%.%df 到 %%.%df 之间的数值。", _dotNum, _dotNum);
			strLimit.Format(strFormat, minValue, _dMaxValue);

			ShowBalloonTip(L"不能接受的数值范围", strLimit);

			return false;
		}
		return true;
	}

	void CValidEditEx::OnKillFocus(CWnd* pNewWnd)
	{
		// 检查合法性
		if (false == Validate())
			return;

		if (!ValidateMaxMinValue())
		{
			return;
		}

		CEdit::OnKillFocus(pNewWnd);
	}
}