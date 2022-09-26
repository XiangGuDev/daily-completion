#pragma once

class StringHelper
{
public:
	// ×Ö·û´®²éÕÒ
	static int lastIndexOf(CString str, CString find);

	// ×ª»»³Éµ¥×Ö½Ú×Ö·û´®
	static std::string w2String(LPCTSTR str);

	static CString Reverse(LPCTSTR str);

	static CString toString(int value);
	static CString toString(double value);
};