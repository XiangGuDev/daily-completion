#include "stdafx.h"
#include "StringBuilder.h"
#include "..\common\StringHelper.h"
#include "..\..\..\Toolkit\include\MemoryDataStream2.h"

StringBuilder::StringBuilder(LPCSTR head)
{
	append(head);
}

void StringBuilder::append(LPCTSTR value)
{
	CW2A text(value);
	append((LPCSTR) text);
}

void StringBuilder::append(LPCSTR value)
{
	m_ms.Write(value, strlen(value));
}

void StringBuilder::append(TCHAR value)
{
	CString text(value);
	append(text);
}

void StringBuilder::append(int value)
{
	CString text = StringHelper::toString(value);
	append(text);
}

CString StringBuilder::toString()
{
	if (m_ms.GetLength() == 0)
		return L"";

	BwProj::toolkit::c_buffer_ptr buffer = BwProj::toolkit::c_buffer_ptr::alloc(m_ms.GetLength() + 1);
	m_ms.CopyBuffer(buffer.get());
	buffer.get()[m_ms.GetLength()] = '\0';

	LPCSTR str = (LPCSTR) buffer.get();
	CA2W wstr(str);
	return (LPCTSTR) wstr;
}