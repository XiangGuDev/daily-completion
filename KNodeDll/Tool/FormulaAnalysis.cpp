#include "stdafx.h"
#include <Math.h>
#include "FormulaAnalysis.h"

namespace CommBase
{
	CAnalysisFormula::CAnalysisFormula(LPCTSTR formula)
	{
		_pos = 0;
		_MulDivReslut = _AddSubReslut = 0;
		_Formula = formula;
		_Formula.Replace(_T("AND"), _T("&"));
		_Formula.Replace(_T("OR"), _T("|"));
		_bOp1 = true; _bResult = true;
	}

	//产生式“A->T&A | T|A | T”
	bool CAnalysisFormula::A_AndOr(void)
	{
		_bOp1 = B_Operator();
		bool bResult = _bOp1;

		// 过滤掉空格
		while (_pos < _Formula.GetLength() && _Formula.GetAt(_pos) == ' ')
			_pos++;

		if (_pos < _Formula.GetLength())
		{
			// 过滤掉空格
			while (_pos < _Formula.GetLength() && _Formula.GetAt(_pos) == ' ')
				_pos++;

			wchar_t ch = _Formula.GetAt(_pos);

			while (ch == '&' || ch == '|')
			{
				_pos++;
				bool bOp2 = B_Operator();
				if (ch == '&')
				{
					if (_bOp1 == true && bOp2 == true && bResult == true)
					{
						_bOp1 = true; bOp2 = true; bResult = true; _bResult = true;
					}
					else
					{
						_bOp1 = false; bOp2 = false; bResult = false; _bResult = false;
					}
				}
				else if (ch == '|')
				{
					if (_bOp1 == false && bOp2 == false && bResult == false)
					{
						_bOp1 = false; bOp2 = false; bResult = false; _bResult = false;
					}
					else
					{
						bResult = true; _bOp1 = true; bOp2 = true; _bResult = true;
					}
				}

				if (_pos >= _Formula.GetLength()) break;

				ch = _Formula.GetAt(_pos);
			}
		}

		return bResult;
	}

	//产生式“B->T>=B | T<=B | T>B | T<B | T”
	bool CAnalysisFormula::B_Operator(void)
	{
		// 运算符左边的值
		double dAddSub1 = C_AddSub();
		bool bResult = true;

		// 过滤掉空格
		while (_pos < _Formula.GetLength() && _Formula.GetAt(_pos) == ' ')
			_pos++;

		if (_pos < _Formula.GetLength())
		{
			wchar_t ch = _Formula.GetAt(_pos);
			while (ch == _T('>') || ch == _T('<') || ch == _T('≤') ||
				ch == _T('≥') || ch == _T('≠') || ch == '=' || ch == _T('∈'))
			{
				_DesValue = _SrcValue;
				_SrcValue = "";

				// 运算符右边的值
				_pos++;
				double dAddSub2 = C_AddSub();

				if (ch == '>')
				{
					if (bResult && (dAddSub1 > dAddSub2))
					{
						bResult = _bResult = true;
					}
					else
					{
						bResult = _bResult = false;
					}
					dAddSub1 = dAddSub2;
				}
				else if (ch == '<')
				{
					if (bResult && (dAddSub1 < dAddSub2))
					{
						bResult = _bResult = true;
					}
					else
					{
						bResult = _bResult = false;
					}
					dAddSub1 = dAddSub2;
				}
				else if (ch == _T('≤'))
				{
					if (bResult && (dAddSub1 <= dAddSub2))
					{
						bResult = _bResult = true;
					}
					else
					{
						bResult = _bResult = false;
					}
					dAddSub1 = dAddSub2;
				}
				else if (ch == _T('≥'))
				{
					if (bResult && (dAddSub1 >= dAddSub2))
					{
						bResult = _bResult = true;
					}
					else
					{
						bResult = _bResult = false;
					}
					dAddSub1 = dAddSub2;
				}
				else if (ch == _T('≠'))
				{
					if (_DesValue == "" && _SrcValue == "")
						_bResult = ((dAddSub1) != (dAddSub2) ? true : false);
					else
					{
						_bResult = _tcscmp(_SrcValue, _DesValue) ? true : false;
						_SrcValue = "";
						_DesValue = "";
					}
				}
				else if (ch == _T('='))
				{
					if (_DesValue == "" && _SrcValue == "")
						_bResult = ((dAddSub1) == (dAddSub2) ? true : false);
					else
					{
						_bResult = (!_tcscmp(_SrcValue, _DesValue) ? true : false);
						_SrcValue = ""; _DesValue = "";
					}
				}
				else if (ch == _T('∈'))
				{
					_bResult = (_DesValue.Find(_SrcValue) > -1) ? true : false;
					_SrcValue = ""; _DesValue = "";
				}
				else if (ch == _T('∉'))
				{
					// 增加不包含于处理逻辑，参照‘∈’规则 zengjiangyou 20170222
					_bResult = (_DesValue.Find(_SrcValue) > -1) ? false : true;
					_SrcValue = ""; _DesValue = "";
				}

				if (_pos >= _Formula.GetLength()) break;

				ch = _Formula.GetAt(_pos);
			}
		}
		return _bResult;
	}


	//产生式“C -> T+C | T-C | T”的函数，用来分析加减算术表达式
	double CAnalysisFormula::C_AddSub(void)
	{
		// 计算加减算术表达式的左元
		double dResult1 = D_MulDiv();

		// 过滤掉空格
		while (_pos < _Formula.GetLength() && _Formula.GetAt(_pos) == ' ')
			_pos++;

		if (_pos < _Formula.GetLength())
		{
			wchar_t ch = _Formula.GetAt(_pos);
			while ((ch == '+' || ch == '-') && _SrcValue == "")
			{
				_pos++;
				double dResult2 = D_MulDiv();       //计算加减算术表达式的右元

				//计算求值
				if (ch == '+')						//如果是"+"号
					dResult1 += dResult2;           //则用加法计算
				else                                //否则（是"-"号）
					dResult1 -= dResult2;           //用减法计算

				if (_pos >= _Formula.GetLength()) break;

				ch = _Formula.GetAt(_pos);
			}
		}

		return dResult1;
	}

	double CAnalysisFormula::D_MulDiv(void)
	{
		// 计算乘除算术表达式的左元
		double dResult1 = E_Number();

		// 过滤掉空格
		while (_pos < _Formula.GetLength() && _Formula.GetAt(_pos) == ' ')
			_pos++;

		if (_pos < _Formula.GetLength())
		{
			wchar_t ch = _Formula.GetAt(_pos);
			while ((ch == '*' || ch == '/') && _SrcValue == "")
			{
				_pos++;
				double dResult2 = E_Number();	//计算乘除算术表达式的右元

				//计算求值
				if (ch == '*')                  //如果是"*"号
					dResult1 *= dResult2;      //则用乘法计算
				else                           //否则（是"\"号）
					dResult1 /= dResult2;      //用除法计算

				if (_pos >= _Formula.GetLength()) break;

				ch = _Formula.GetAt(_pos);
			}
		}

		return dResult1;
	}

	double CAnalysisFormula::E_Number(void)
	{
		// 声明存储返回值的变量
		double dResult = 0;

		// 过滤掉空格
		while (_pos < _Formula.GetLength() && _Formula.GetAt(_pos) == ' ')
			_pos++;

		if (_pos < _Formula.GetLength())
		{
			wchar_t ch = _Formula.GetAt(_pos);
			if (ch == '(')						//如果字符缓冲区当前位置的符号是"("
			{
				_pos++;							//则指示器加一指向下一个符号
				dResult = A_AndOr();			//调用产生式“E -> T+E | T-E | T”的分析函数
				_pos++;

				return dResult;
			}

			// 如果字符缓冲区中当前位置的字符为数字
			ch = _Formula.GetAt(_pos);
			int cValue = ch;

			if ((cValue < 256 && ::isdigit(ch)) || ch == _T('.'))
			{
				//则用产生式F -> i推导
				//把字符缓冲区中当前位置的字符串转换为整数
				CString strChar;

				//改变指示器的值，跳过字符缓冲区的数字部分，找到下一个输入字符。
				ch = _Formula.GetAt(_pos);
				cValue = ch;
				while (((cValue < 256 && ::isdigit(ch)) || ch == _T('.')) && _pos < _Formula.GetLength())
				{
					CString tmpChar(strChar);
					strChar.Format(_T("%s%c"), tmpChar, _Formula.GetAt(_pos));
					_pos++;
					ch = _Formula.GetAt(_pos);
					cValue = ch;
				}
				dResult = _tstof(strChar);
			}
			else if (ch == _T('“'))
			{
				_pos++;
				while (_pos < _Formula.GetLength() && _Formula.GetAt(_pos) != _T('”'))
					_SrcValue += _Formula.GetAt(_pos++);

				_pos++;
			}
		}

		return dResult;
	}
}