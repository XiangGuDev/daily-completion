#include "StdAfx.h"
#include "Guid.h"
#include <xstddef>

namespace CommBase
{
	size_t HashGuid::operator()(const GUID& guid) const
	{
		return ((guid.Data1 ^ ((guid.Data2 << 0x10) | (guid.Data3))) ^ 
				((guid.Data4[2] << 0x18) | guid.Data4[7]));
	}

	CGuid::CGuid()
	{
		ChangeNew();
	}

	CGuid::CGuid(const GUID& guid)
	{
		_guid = guid;
	}

	CGuid::CGuid(LPCWSTR strId)
	{
		if (lstrlen(strId) != 32)
		{
			Clear();
			return;
		}

		TCHAR lpGuid[33] = { 0 };
		lstrcpyn(lpGuid, strId, 33);

		_guid.Data4[7] = (unsigned char)_tcstoul(lpGuid + 30, NULL, 16);
		lpGuid[30] = L'\0';
		_guid.Data4[6] = (unsigned char)_tcstoul(lpGuid + 28, NULL, 16);
		lpGuid[28] = L'\0';
		_guid.Data4[5] = (unsigned char)_tcstoul(lpGuid + 26, NULL, 16);
		lpGuid[26] = L'\0';
		_guid.Data4[4] = (unsigned char)_tcstoul(lpGuid + 24, NULL, 16);
		lpGuid[24] = L'\0';
		_guid.Data4[3] = (unsigned char)_tcstoul(lpGuid + 22, NULL, 16);
		lpGuid[22] = L'\0';
		_guid.Data4[2] = (unsigned char)_tcstoul(lpGuid + 20, NULL, 16);
		lpGuid[20] = L'\0';
		_guid.Data4[1] = (unsigned char)_tcstoul(lpGuid + 18, NULL, 16);
		lpGuid[18] = L'\0';
		_guid.Data4[0] = (unsigned char)_tcstoul(lpGuid + 16, NULL, 16);
		lpGuid[16] = L'\0';
		_guid.Data3 =    (unsigned short)_tcstoul(lpGuid + 12, NULL, 16);
		lpGuid[12] = L'\0';
		_guid.Data2 =    (unsigned short)_tcstoul(lpGuid + 8, NULL, 16);
		lpGuid[8] = L'\0';
		_guid.Data1 =    (unsigned long)_tcstoul(lpGuid, NULL, 16);
	}

	void CGuid::ChangeNew()
	{
		::CoCreateGuid(&_guid);
	}

	CString CGuid::ToString(bool bStand/* = false*/) const
	{
		TCHAR buf[64] = { 0 };

		if (bStand)
		{
			_snwprintf(buf, sizeof(buf),
				_T("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"),
				_guid.Data1, _guid.Data2, _guid.Data3,
				_guid.Data4[0], _guid.Data4[1],
				_guid.Data4[2], _guid.Data4[3],
				_guid.Data4[4], _guid.Data4[5],
				_guid.Data4[6], _guid.Data4[7]);
		}
		else
		{
			_snwprintf(buf, sizeof(buf),
				_T("%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X"),
				_guid.Data1, _guid.Data2, _guid.Data3,
				_guid.Data4[0], _guid.Data4[1],
				_guid.Data4[2], _guid.Data4[3],
				_guid.Data4[4], _guid.Data4[5],
				_guid.Data4[6], _guid.Data4[7]);
		}

		return buf;
	}

	CGuid& CGuid::Clear()
	{
		memset(&_guid, 0, sizeof(GUID));
		return *this;
	}

	bool CGuid::IsEmpty() const
	{
		return (_guid.Data1 == 0 && _guid.Data2 == 0 &&
			_guid.Data3 == 0 && *((LONGLONG*)(&_guid.Data4)) == 0);
	}

	CGuid::operator CString() const
	{
		TCHAR buf[64] = { 0 };

		_snwprintf(buf, sizeof(buf),
			_T("%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X"),
			_guid.Data1, _guid.Data2, _guid.Data3,
			_guid.Data4[0], _guid.Data4[1],
			_guid.Data4[2], _guid.Data4[3],
			_guid.Data4[4], _guid.Data4[5],
			_guid.Data4[6], _guid.Data4[7]);

		return buf;
	}

	CGuid::operator GUID() const
	{
		return _guid;
	}

	bool CGuid::operator < (const CGuid& guid) const
	{
		if (_guid.Data1 != guid._guid.Data1)
			return _guid.Data1 < guid._guid.Data1;
		if (_guid.Data2 != guid._guid.Data2)
			return _guid.Data2 < guid._guid.Data2;
		if (_guid.Data3 != guid._guid.Data3)
			return _guid.Data3 < guid._guid.Data3;
		if (*((LONGLONG*)(&_guid.Data4)) != *((LONGLONG*)(&guid._guid.Data4)))
			return *((LONGLONG*)(&_guid.Data4)) < *((LONGLONG*)(&guid._guid.Data4));
		return false;
	}

	CGuid& CGuid::operator = (const CGuid& guid)
	{
		_guid = guid._guid;
		return *this;
	}

	bool CGuid::operator == (const CGuid& guid) const
	{
		return (_guid.Data1 == guid._guid.Data1 &&
			_guid.Data2 == guid._guid.Data2 &&
			_guid.Data3 == guid._guid.Data3 &&
			*((LONGLONG*)(&_guid.Data4)) == *((LONGLONG*)(&guid._guid.Data4)));
	}

	bool CGuid::operator != (const CGuid& guid) const
	{
		return (_guid.Data1 != guid._guid.Data1 ||
			_guid.Data2 != guid._guid.Data2 ||
			_guid.Data3 != guid._guid.Data3 ||
			*((LONGLONG*)(&_guid.Data4)) != *((LONGLONG*)(&guid._guid.Data4)));
	}
}