#pragma once
#include "../../../Toolkit/include/MemoryDataStream2.h"

class StringBuilder
{
public:
	StringBuilder(LPCSTR head = "");

	void append(LPCTSTR value);
	void append(LPCSTR value);
	void append(TCHAR value);
	void append(int value);

	CString toString();

private:
	BwProj::toolkit::CMemoryDataWriteStream2 m_ms;
};