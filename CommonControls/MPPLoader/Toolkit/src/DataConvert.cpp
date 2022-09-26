#include "StdAfx.h"
#include "..\include\DataConvert.h"

using namespace BwProj::toolkit;

#ifndef _CVTBUFSIZE
	#define _CVTBUFSIZE (309+40)
#endif

wchar_t* DataConvert::UTF8ToUnicode(const char* str)
{
	int    textlen = 0;
	wchar_t * result;
	textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	result = (wchar_t *)malloc((textlen + 1)*sizeof(wchar_t));
	memset(result, 0, (textlen + 1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
	return    result;
}

// 浮点数比较
bool DataConvert::FloatEqual (double var1, double var2, int dot)
{
	if (dot == -1)
	{
		return (fabs (var1 - var2) <= 0.000001);
	}
	else
	{
		//TCHAR fmtbuf1[_CVTBUFSIZE], fmtbuf2[_CVTBUFSIZE];
		//Float2Text (var1, fmtbuf1, dot, false);
		//Float2Text (var2, fmtbuf2, dot, false);

		//return !_tcscmp (fmtbuf1, fmtbuf2);
		ATLASSERT (dot < 10);

		//long nMultiple = 1;
		//while (dot > 0)
		//{
		//	nMultiple *= 10;

		//	dot--;
		//}

		//return (fabs(var1 - var2) * nMultiple < 1);

		ULONG DotPow = (ULONG) pow ((float) 10, dot + 1);
		LONGLONG u1 = (LONGLONG) (var1 * DotPow);
		LONGLONG u2 = (LONGLONG) (var2 * DotPow);

		u1 += 5; u1 /= 10;
		u2 += 5; u2 /= 10;

		return u1 == u2;
	}
}

// 浮点数四舍五入
double DataConvert::FloatRound (double num, int dot)
{
	if (dot < 0) return num;

	long dSign = 1;
	if (num < 0) dSign = -1;	
	long cs = (long) pow ((double) 10, dot);
	LONGLONG ullVal = (LONGLONG) (fabs (num) * cs + 0.5);

	return ((double)(ullVal * dSign)) / cs;
}

/** 
* __int64 转换成字符串
* @param [in] val		要格式化的数值
* @param [in] buff		输出缓存
* @return	返回__int64格式化后的字符串
*/
LPCTSTR DataConvert::I642Text (__int64 val, TCHAR* buff)
{
	// 实现64位整型转换
	errno_t err = _i64tot_s(val, buff, _CVTBUFSIZE, 10);
	if (err != 0)
		return _T("");

	//_sntprintf_s (buff, _T("%d"), val);
	return buff;
}

/** 
* __int64 转换成字符串
* @param [in] val		要格式化的数值
* @return	返回__int64格式化后的字符串
*/
CString DataConvert::I642Text (__int64 val)
{
	TCHAR buff [_CVTBUFSIZE];
	return I642Text (val, buff);
}

/** 
* bool 转换成字符串
* @param [in] val		要格式化的数值
* @return	返回bool格式化后的字符串
*/
LPCTSTR DataConvert::Bool2Text (BOOL val)
{
	return val ? _T("True") : _T("False");
}

/** 
* 字符串 转换成bool
* @param [in] val		要格式化的数值
* @return	返回字符串格式化后的bool
*/
bool DataConvert::Text2Bool (LPCTSTR val)
{
	CString str(val);
	str.MakeUpper();
	return str == _T("TRUE") || str == _T("T");
}

// 比较两个字符串是否相等，不区分大小写
bool DataConvert::StringEqual (LPCTSTR str1, LPCTSTR str2)
{
	if (str1 == str2)
		return true;
	if (str1 == NULL || str2 == NULL)
		return false;

	//TCHAR ch1[2], ch2[2];
	while (*str1 && *str2)
	{
		// 全变转换成小写（'a' > 'A'）
		TCHAR c1 = (*str1 >= _T('A') && *str1 <= _T('Z')) ? (*str1 + _T('a')-_T('A')) : * str1;
		TCHAR c2 = (*str2 >= _T('A') && *str2 <= _T('Z')) ? (*str2 + _T('a')-_T('A')) : * str2;

		if (c1 != c2)
			return false;

		str1 ++; str2 ++;
	}

	return (*str1 == NULL && *str2 == NULL);
}

// 浮点数四舍五入
//	num		= 要格式化的数值
//	dot		= 小数位数，为 -1 表示是不四舍五入
//	buffer	= 输出缓存
//	FullZero= 如果小数位不够，是否用 0 填充
LPCTSTR DataConvert::Float2Text(double num, TCHAR* buffer, int dot, bool FullZero)
{
	ATLASSERT(dot >= -1 && buffer != NULL);

	TCHAR fmt[_CVTBUFSIZE];

	if (dot >= 0)
	{
		_sntprintf_s(fmt, _CVTBUFSIZE,  _T("%%0.%df"), dot);
		_sntprintf_s(buffer, _CVTBUFSIZE, 1, fmt, FloatRound(num, dot));
	}
	else
	{
		_sntprintf_s(buffer, _CVTBUFSIZE, 1, _T("%f"), num);
	}

	if (FullZero == false || dot <= 0)
	{
		// 查找小数点位置
		int dotNum = -1;
		for (int i = 0; i < (int)_tcslen(buffer); i++)
		if (buffer[i] == _T('.'))
		{
			dotNum = i;
			break;
		}

		// 删除小数点后面的 0
		if (dotNum >= 0)
		{
			for (int i = (int)_tcslen(buffer) - 1; i >= dotNum; i--)
			if (buffer[i] == _T('0'))
				buffer[i] = NULL;
			else
				break;

			if (buffer[_tcslen(buffer) - 1] == _T('.'))
				buffer[_tcslen(buffer) - 1] = NULL;
		}
	}

	return buffer;
}

CString DataConvert::Float2Text(double num, int dot, bool FullZero)
{
	TCHAR fmtbuf[_CVTBUFSIZE];
	return Float2Text(num, fmtbuf, dot, FullZero);
}

/**
* long 转换成字符串
* @param [in] val		要格式化的数值
* @param [in] buff		输出缓存
* @return	返回long格式化后的字符串
*/
LPCTSTR DataConvert::Long2Text(long val, TCHAR* buff)
{
	_sntprintf_s(buff, _CVTBUFSIZE, 1, _T("%d"), val);
	return buff;
}

/**
* long 转换成字符串
* @param [in] val		要格式化的数值
* @return	返回long格式化后的字符串
*/
CString DataConvert::Long2Text(long val)
{
	TCHAR buff[_CVTBUFSIZE];
	return Long2Text(val, buff);
}