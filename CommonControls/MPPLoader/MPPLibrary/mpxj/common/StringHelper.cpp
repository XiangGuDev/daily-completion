#include "stdafx.h"
#include "StringHelper.h"

int StringHelper::lastIndexOf(CString str, CString find)
{
	CString str2 = Reverse(str);
	CString find2 = Reverse(find);

	int index = str2.Find(find2);
	if (index >= 0)
		return str.GetLength() - index - 1;
	return index;
}

// 转换成单字节字符串
std::string StringHelper::w2String(LPCTSTR str)
{
	CW2A cstr(str);
	return (LPCSTR)cstr;
}

CString StringHelper::Reverse(LPCTSTR str)
{
	int len = _tcslen(str);
	//TCHAR* newstr = new TCHAR(len + 1);
	//for (int i = 0; i < len; i++)
	//	newstr[i] = str[len - i - 1];
	//newstr[len] = 0;
	//CString ret = newstr;
	//delete[] newstr;
	//return ret;

	CString ret;
	for (int i = 0; i < len; i++)
		ret += str[len - i - 1];
	return ret;
}

CString StringHelper::toString(int value)
{
	CString str;
	str.Format(L"%d", value);
	return str;
}

CString StringHelper::toString(double value)
{
	CString str;
	str.Format(L"%f", value);
	return str;
}