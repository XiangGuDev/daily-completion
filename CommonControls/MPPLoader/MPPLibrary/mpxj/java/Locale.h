#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////
// 本地化
//
class Locale
{
public:
	Locale(char const* _Locale);
	static Locale* getDefault();

	LPCTSTR getLanguageCode();

	LPCTSTR getNationCode();

private:
	CString _langCode;		// 语言代码
	CString _nationCode;	// 国家代码
	int _nationNumber;		// 国家编码

	static std::auto_ptr<Locale> _default;
};