#include "StdAfx.h"
#include <math.h>
#include "StringTool.h"
#include "Convert.h"
#include <shellapi.h>

namespace CommBase
{
	CString CStringTool::TrimNumberString(CString NumStr)
	{
		NumStr.TrimRight(L'0');
		NumStr.TrimRight(L'.');
		if (NumStr.IsEmpty())
			NumStr = L'0';

		return NumStr;
	}

	int CStringTool::CountWord(CString sString, TCHAR sParChr)
	{
		int cnt, idx, sLen;
		TCHAR PrevChr;

		cnt = 0; PrevChr = sParChr;

		sLen = sString.GetLength();
		for (idx = 0; idx < sLen; idx++)
		{
			if ((sString.GetAt(idx) != sParChr) && (PrevChr == sParChr))
				cnt++;
			PrevChr = sString.GetAt(idx);
		}
		return cnt;
	}

	CString CStringTool::ReadWord(CString sString, int WordIdx, TCHAR sParChr)
	{
		int cnt, idx, sLen, idx2;
		TCHAR PrevChr;
		CString NewWord;

		cnt = 0; PrevChr = sParChr;
		sLen = sString.GetLength();
		for (idx = 0; idx < sLen; idx++)
		{
			if ((sString.GetAt(idx) != sParChr) && (PrevChr == sParChr))
			{
				cnt++;
				if (cnt == WordIdx)
				{
					idx2 = idx;
					while (idx2 < sString.GetLength() && sString.GetAt(idx2) != sParChr)
						idx2++;
					if (idx2 >= sString.GetLength())
						return sString.Mid(idx);
					else
						return sString.Mid(idx, idx2 - idx);
				}
			}
			PrevChr = sString.GetAt(idx);
		}
		return _T("");
	}

	int CStringTool::FindWord(CString SourStr, CString Word, bool RigorFind,
		int StartPos, TCHAR sParChr)
	{
		int i, cnt;
		CString s1, f1;
		cnt = CountWord(SourStr, sParChr);
		if (!cnt || cnt < StartPos) return 0;

		if (RigorFind == false)
		{
			s1 = SourStr.Trim(); f1 = Word.Trim();
			s1.MakeUpper(); f1.MakeUpper();
			// 将分割字符转为大写 SHL
			if (sParChr >= _T('a') && sParChr <= _T('z'))
				sParChr -= 32;
		}
		else
		{
			s1 = SourStr; f1 = Word;
		}

		for (i = StartPos; i <= cnt; i++)
		{
			if (ReadWord(s1, i, sParChr) == f1)
				return i;
		}
		return 0;
	}

	CString CStringTool::DoubleToStr(double value)
	{
#ifdef _UNICODE
		wchar_t valstr[255];
		_swprintf(valstr, L"%f", value);
#else
		char valstr[255];
		sprintf(valstr, "%f", value);
#endif
		return TrimNumberString(valstr);
	}

	CString CStringTool::IntToStr(long value)
	{
		TCHAR  buf[128] = { 0 };
		_ltot_s(value, buf, 128, 10);
		return buf;
	}

	long CStringTool::StrToInt(CString str)
	{
		return _ttol(str);
	}

	// Str To Double 字符串必须是 "1234.696" 形式
	double CStringTool::StrToDouble(const CString& str)
	{
		return _tstof(str);
	}

	// 比较两个版本号大小
	int CStringTool::CompareVersion(CString version1, CString version2, int nIndex)
	{
		int nCount1 = CStringTool::CountWord(version1, _T('.'));
		int nCount2 = CStringTool::CountWord(version2, _T('.'));

		int nCount = nCount1;
		if (nIndex > 0)
		{
			nCount = (nCount1 < nIndex) ? nCount1 : nIndex;
		}

		for (int i = 1; i <= nCount; ++i)
		{
			if (i > nCount2) return 1;

			if (CStringTool::ReadWord(version1, i, _T('.')) == CStringTool::ReadWord(version2, i, _T('.')))
				continue;
			else if (_tstol(CStringTool::ReadWord(version1, i, _T('.'))) < _tstol(CStringTool::ReadWord(version2, i, _T('.'))))
				return -1;
			else
				return 1;
		}

		if (nCount1 > nCount2)
			return 1;
		else if (nCount1 < nCount2)
			return -1;

		return 0;
	}

	void CStringTool::SplitString(const CString& strNeedSplit,
		StringArray& strSplitStrings, CString strSep)
	{
		if (strNeedSplit.IsEmpty())
			return;

		int nSepLen = strSep.GetLength();

		// 1、获取分隔数据的起始位置与结束位置
		int nTextStart = 0;
		int nTextEnd = strNeedSplit.Find(strSep, nTextStart);
		while (-1 != nTextEnd)
		{
			// 2、将解析好的数据添加到返回值中
			CString strText = strNeedSplit.Mid(nTextStart, nTextEnd - nTextStart);
			strSplitStrings.push_back(strText);

			// 3、获取下一个分隔数据的起始位置与结束位置
			nTextStart = nTextEnd + nSepLen;
			nTextEnd = strNeedSplit.Find(strSep, nTextStart);
		}

		// 4、获取最后一个分隔数据并添加到返回值中
		CString strLastText = strNeedSplit.Mid(nTextStart, strNeedSplit.GetLength() - nTextStart);
		if (!strLastText.IsEmpty())
			strSplitStrings.push_back(strLastText);
	}

	CString CStringTool::BuildSplitString(const StringArray& strSplitStrings, CString strSep)
	{
		CString strSplitString;
		size_t nSize = strSplitStrings.size();

		for (size_t i = 0; i < nSize; ++i)
		{
			if (i != 0)
			{
				strSplitString += strSep;
			}

			strSplitString += strSplitStrings[i];
		}

		return strSplitString;
	}

	CString CStringTool::GenerateChineseNumIn100(int num)
	{
		CString strNum;
		int num1 = num % 10;  //个位
		int num2 = num / 10;  //十位
		switch (num2)
		{
		case 1: strNum = L"十"; break;
		case 2: strNum = L"二十"; break;
		case 3: strNum = L"三十"; break;
		case 4: strNum = L"四十"; break;
		case 5: strNum = L"五十"; break;
		case 6: strNum = L"六十"; break;
		case 7: strNum = L"七十"; break;
		case 8: strNum = L"八十"; break;
		case 9: strNum = L"九十"; break;
		default: break;
		}

		switch (num1)
		{
		case 1: strNum += L"一"; break;
		case 2: strNum += L"二"; break;
		case 3: strNum += L"三"; break;
		case 4: strNum += L"四"; break;
		case 5: strNum += L"五"; break;
		case 6: strNum += L"六"; break;
		case 7: strNum += L"七"; break;
		case 8: strNum += L"八"; break;
		case 9: strNum += L"九"; break;
		default: break;
		}

		if (strNum.IsEmpty())
		{
			strNum += L"零";
		}

		return strNum;
	}

	bool CStringTool::IsDigit(const CString& strText)
	{
		bool bFoundNum = false;
		if (strText.IsEmpty())
			return true;

		if (strText.GetLength() == 1 && strText == _T("-"))
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
				// 不在第一位
				if (i != 0) return false;
				continue;
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

	CString CStringTool::MCharToWChar(const CStringA& s, int nCodePage)
	{
		CString wstr;
		int len = MultiByteToWideChar(nCodePage, 0, s.GetString(), -1, 0, 0);
		LPWSTR lpszBuf = (LPWSTR)wstr.GetBufferSetLength(len);

		int nResult = MultiByteToWideChar(nCodePage, 0, s.GetString(), s.GetLength() + 1, lpszBuf, len);

		wstr.ReleaseBuffer();

		if (nResult == 0)
			wstr.Empty();

		return wstr;
	}

	CStringA CStringTool::WCharToMChar(const CString& s, int nCodePage)
	{
		CStringA str;
		int len = WideCharToMultiByte(nCodePage, 0, s.GetString(), -1, 0, 0, 0, 0);
		LPSTR lpszBuf = (LPSTR)str.GetBufferSetLength(len);

		int nResult = WideCharToMultiByte(nCodePage, 0,
			s.GetString(), s.GetLength() + 1, lpszBuf, len, NULL, NULL);

		str.ReleaseBuffer();

		if (nResult == 0)
			str.Empty();

		return str;
	}

	CString CStringTool::GetNextCode(const TCHAR *lpszCurCode)
	{
		// 递增字符
		CString strNextCode;

		// 1、按照数字进行递增
		int nLength = (int)_tcslen(lpszCurCode);
		for (int nCharIdx = nLength - 1; nCharIdx >= 0; --nCharIdx)
		{
			TCHAR curChar = lpszCurCode[nCharIdx];
			// 数字递增
			if (_T('0') <= curChar && curChar <= _T('9'))
			{
				// 找到末尾的连串数字
				int nPrevNumCharIdx = nCharIdx - 1;
				for (; nPrevNumCharIdx >= 0; --nPrevNumCharIdx)
				{
					TCHAR numChar = lpszCurCode[nPrevNumCharIdx];
					if (numChar < _T('0')
						|| numChar > _T('9'))
					{
						break;
					}
				}

				// 排除前缀为0的字符
				for (; nPrevNumCharIdx < nCharIdx && nPrevNumCharIdx != nLength - 2; ++nPrevNumCharIdx)
				{
					TCHAR numChar = lpszCurCode[nPrevNumCharIdx + 1];
					if (numChar != _T('0'))
					{
						break;
					}
				}

				// 递增字符前缀
				CString strPrevfix;
				// 递增字符后缀
				CString strSuffix;
				// 递增字符
				CString strIncNum;
				for (int nRpCharIdx = 0; nRpCharIdx < nLength; ++nRpCharIdx)
				{
					TCHAR rpChar = lpszCurCode[nRpCharIdx];
					if (nRpCharIdx <= nPrevNumCharIdx)
					{
						strPrevfix.AppendChar(rpChar);
					}
					else if (nRpCharIdx <= nCharIdx)
					{
						strIncNum.AppendChar(rpChar);
					}
					else
					{
						strSuffix.AppendChar(rpChar);
					}
				}
				int nIncNum = _ttoi(strIncNum);
				++nIncNum;
				strIncNum = CStringTool::IntToStr(nIncNum);
				strNextCode = (strPrevfix + strIncNum + strSuffix);

				return strNextCode;
			}
		}

		// 数字、字母都没有找到，则末尾补1
		strNextCode.Format(L"%s1", lpszCurCode);

		return strNextCode;
	}

	CString CStringTool::GetPrevCode(const TCHAR *lpszCurCode)
	{
		// 递减字符
		CString strPrevCode;

		// 1、按照数字进行递减
		int nLength = (int)_tcslen(lpszCurCode);
		for (int nCharIdx = nLength - 1; nCharIdx >= 0; --nCharIdx)
		{
			TCHAR curChar = lpszCurCode[nCharIdx];
			// 数字递增
			if (_T('0') <= curChar && curChar <= _T('9'))
			{
				// 找到末尾的连串数字
				int nPrevNumCharIdx = nCharIdx - 1;
				for (; nPrevNumCharIdx >= 0; --nPrevNumCharIdx)
				{
					TCHAR numChar = lpszCurCode[nPrevNumCharIdx];
					if (numChar < _T('0')
						|| numChar > _T('9'))
					{
						break;
					}
				}

				// 排除前缀为0的字符
				for (; nPrevNumCharIdx < nCharIdx && nPrevNumCharIdx != nLength - 2; ++nPrevNumCharIdx)
				{
					TCHAR numChar = lpszCurCode[nPrevNumCharIdx + 1];
					if (numChar != _T('0'))
					{
						break;
					}
				}

				// 递增字符前缀
				CString strPrevfix;
				// 递增字符后缀
				CString strSuffix;
				// 递增字符
				CString strIncNum;
				for (int nRpCharIdx = 0; nRpCharIdx < nLength; ++nRpCharIdx)
				{
					TCHAR rpChar = lpszCurCode[nRpCharIdx];
					if (nRpCharIdx <= nPrevNumCharIdx)
					{
						strPrevfix.AppendChar(rpChar);
					}
					else if (nRpCharIdx <= nCharIdx)
					{
						strIncNum.AppendChar(rpChar);
					}
					else
					{
						strSuffix.AppendChar(rpChar);
					}
				}
				int nIncNum = _ttoi(strIncNum);
				--nIncNum;
				if (nIncNum < 0) // 不小于0
				{
					nIncNum = 0;
				}
				strIncNum = CStringTool::IntToStr(nIncNum);
				strPrevCode = (strPrevfix + strIncNum + strSuffix);

				return strPrevCode;
			}
		}

		// 数字、字母都没有找到，则末尾补1
		strPrevCode.Format(L"%s1", lpszCurCode);

		return strPrevCode;
	}
}