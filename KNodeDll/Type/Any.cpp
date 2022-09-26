#include "StdAfx.h"
#include "Any.h"
#include "TypeDefine.h"
#include "Assert.h"
#include "../Tool/Convert.h"

//#pragma warning(disable:4244)

namespace CommBase
{
	CAny::CAny()
	{
		_vdType = ANY_EMPTY;
		_vdValue.valInt64 = 0;
	}

	CAny::~CAny()
	{
		ClearTypeMemory();
	}

	//bool
	CAny::CAny(const bool& value)
	{
		_vdType = ANY_BOOL;
		_vdValue.valBool = value;
	}

	void CAny::Attach(const CAny& other) const
	{
		// 采用特殊手段来处理
		CAny* pThis = const_cast<CAny*>(this);
		CAny* pOther = const_cast<CAny*>(&other);

		pThis->ClearTypeMemory();

		pThis->_vdType = pOther->_vdType;
		pThis->_vdValue.valInt64 = pOther->_vdValue.valInt64; // 随便取一个最大的值赋值

		pOther->_vdType = ANY_EMPTY;
		pOther->_vdValue.valInt64 = 0;
	}

	CAny& CAny::operator = (const bool& value)
	{
		ClearTypeMemory();
		_vdType = ANY_BOOL;
		_vdValue.valBool = value;

		return *this;
	}

	CAny::operator bool() const
	{
		switch (_vdType)
		{
		case ANY_EMPTY:
			return false;
		case ANY_BOOL:
			return _vdValue.valBool;
		case ANY_INT:
			return !!_vdValue.valInt;
		case ANY_INT64:
			return !!_vdValue.valInt64;
		case ANY_DOUBLE:
			return !!(long)_vdValue.valDouble;
		case ANY_STRING:
			return !wcscmp(_vdValue.valString, L"是");
		case ANY_GUID:
			return !CGuid(*_vdValue.valGuid).IsEmpty();
		default:
			return false;
		}
	}

	// int
	CAny::CAny(const int& value)
	{
		_vdType = ANY_INT;
		_vdValue.valInt = value;
	}

	CAny& CAny::operator = (const int& value)
	{
		ClearTypeMemory();
		_vdType = ANY_INT;
		_vdValue.valInt = value;

		return *this;
	}

	CAny::operator int() const
	{
		switch (_vdType)
		{
		case ANY_EMPTY:
			return 0;
		case ANY_BOOL:
			return _vdValue.valBool ? 1 : 0;
		case ANY_INT:
			return _vdValue.valInt;
		case ANY_INT64:
			return _vdValue.valInt64;
		case ANY_DOUBLE:
			return (int)_vdValue.valDouble;
		case ANY_STRING:
			return _wtoi(_vdValue.valString);
		default:
			return 0;
		}
	}

	// int64
	CAny::CAny(const INT64& value)
	{
		_vdType = ANY_INT64;
		_vdValue.valInt64 = value;
	}

	CAny& CAny::operator = (const INT64& value)
	{
		ClearTypeMemory();
		_vdType = ANY_INT64;
		_vdValue.valInt64 = value;

		return *this;
	}

	CAny::operator INT64() const
	{
		switch (_vdType)
		{
		case ANY_EMPTY:
			return 0;
		case ANY_BOOL:
			return _vdValue.valBool ? 1 : 0;
		case ANY_INT:
			return _vdValue.valInt;
		case ANY_INT64:
			return _vdValue.valInt64;
		case ANY_DOUBLE:
			return (INT64)_vdValue.valDouble;
		case ANY_STRING:
			return _tcstoui64(_vdValue.valString, NULL, 10);
		default:
			return 0;
		}
	}


	// size_t
	CAny::CAny(const size_t& value)
	{
		_vdType = ANY_INT;
		_vdValue.valInt = int(value);
	}

	CAny& CAny::operator = (const size_t& value)
	{
		ClearTypeMemory();
		_vdType = ANY_INT;
		_vdValue.valInt = int(value);

		return *this;
	}

	CAny::operator size_t() const
	{
		switch (_vdType)
		{
		case ANY_EMPTY:
			return 0;
		case ANY_BOOL:
			return _vdValue.valBool ? 1 : 0;
		case ANY_INT:
			return (size_t)_vdValue.valInt;
		case ANY_INT64:
			return (size_t)_vdValue.valInt64;
		case ANY_DOUBLE:
			return (size_t)_vdValue.valDouble;
		case ANY_STRING:
			return _wtol(_vdValue.valString);
		default:
			return 0;
		}
	}

	// long
	CAny::CAny(const long& value)
	{
		_vdType = ANY_INT;
		_vdValue.valInt = value;
	}

	CAny& CAny::operator = (const long& value)
	{
		ClearTypeMemory();
		_vdType = ANY_INT;
		_vdValue.valInt = value;

		return *this;
	}

	CAny::operator long() const
	{
		switch (_vdType)
		{
		case ANY_EMPTY:
			return 0;
		case ANY_BOOL:
			return _vdValue.valBool ? 1 : 0;
		case ANY_INT:
			return (long)_vdValue.valInt;
		case ANY_INT64:
			return (long)_vdValue.valInt64;
		case ANY_DOUBLE:
			return (long)_vdValue.valDouble;
		case ANY_STRING:
			return _wtol(_vdValue.valString);
		default:
			return 0;
		}
	}

	// DWORD
	CAny::CAny(const DWORD& value)
	{
		_vdType = ANY_INT;
		_vdValue.valDword = value;
	}

	CAny& CAny::operator=(const DWORD& value)
	{
		ClearTypeMemory();
		_vdType = ANY_INT;
		_vdValue.valDword = value;

		return *this;
	}

	CAny::operator DWORD() const
	{
		switch (_vdType)
		{
		case ANY_EMPTY:
			return 0;
		case ANY_BOOL:
			return _vdValue.valBool ? 1 : 0;
		case ANY_INT:
			return (DWORD)_vdValue.valInt;
		case ANY_INT64:
			return (DWORD)_vdValue.valInt64;
		case ANY_DOUBLE:
			return (DWORD)_vdValue.valDouble;
		case ANY_STRING:
			return _wtol(_vdValue.valString);
		default:
			return 0;
		}
	}

	// double
	CAny::CAny(const double& value)
	{
		_vdType = ANY_DOUBLE;
		_vdValue.valDouble = value;
	}

	CAny& CAny::operator = (const double& value)
	{
		ClearTypeMemory();
		_vdType = ANY_DOUBLE;
		_vdValue.valDouble = value;

		return *this;
	}

	CAny::operator double() const
	{
		switch (_vdType)
		{
		case ANY_EMPTY:
			return 0.0;
		case ANY_BOOL:
			return _vdValue.valBool ? 1.0 : 0.0;
		case ANY_INT:
			return (double)_vdValue.valInt;
		case ANY_INT64:
			return (double)_vdValue.valInt64;
		case ANY_DOUBLE:
			return _vdValue.valDouble;
		case ANY_STRING:
			return _wtof(_vdValue.valString);
		default:
			return 0.0;
		}
	}

	// String
	CAny::CAny(const LPCWSTR& value)
	{
		_vdType = ANY_STRING;

		int len = int(wcslen(value));
		_vdValue.valString = new WCHAR[len + 1];
		wcsncpy(_vdValue.valString, value, len);
		_vdValue.valString[len] = 0;
	}

	CAny& CAny::operator = (const LPCWSTR& value)
	{
		ClearTypeMemory();
		_vdType = ANY_STRING;

		int len = int(wcslen(value));
		_vdValue.valString = new WCHAR[len + 1];
		wcsncpy(_vdValue.valString, value, len);
		_vdValue.valString[len] = 0;

		return *this;
	}

	LPWSTR CAny::BuildString(UINT nLen)
	{
		ClearTypeMemory();
		_vdType = ANY_STRING;

		_vdValue.valString = new WCHAR[nLen+1];
		_vdValue.valString[nLen] = 0;
		return _vdValue.valString;
	}

	CAny::operator LPCWSTR() const
	{
		switch (_vdType)
		{
		case ANY_EMPTY:
			return NULL;
		case ANY_BOOL:
			return _vdValue.valBool ? L"是" : L"否";
		case ANY_STRING:
			return (LPCWSTR)_vdValue.valString;
		default:
			return NULL;
		}

		return NULL;
	}

	// CString
	CAny::CAny(const CString& value)
	{
		_vdType = ANY_STRING;

		size_t len = value.GetLength();
		_vdValue.valString = new WCHAR[len + 1];
		wcsncpy(_vdValue.valString, value, len);
		_vdValue.valString[len] = 0;
	}

	CAny& CAny::operator = (const CString& value)
	{
		ClearTypeMemory();
		_vdType = ANY_STRING;

		size_t len = value.GetLength();
		_vdValue.valString = new WCHAR[len + 1];
		wcsncpy(_vdValue.valString, value, len);
		_vdValue.valString[len] = 0;

		return *this;
	}

	CAny::operator CString() const
	{
		CString strFormat;
		switch (_vdType)
		{
		case ANY_EMPTY:
			return L"";
		case ANY_BOOL:
			return _vdValue.valBool ? L"是" : L"否";
		case ANY_INT:
			strFormat.Format(L"%d", _vdValue.valInt);
			return strFormat;
		case ANY_INT64:
			strFormat.Format(L"%I64d", _vdValue.valInt64);
			return strFormat;
		case ANY_DOUBLE:
			strFormat.Format(L"%f", _vdValue.valDouble);
			return strFormat;
		case ANY_STRING:
			return (LPCWSTR)_vdValue.valString;
		case ANY_GUID:
			return CGuid(*_vdValue.valGuid).ToString();
		default:
			return L"";
		}
	}

	CAny::CAny(const CAny& value)
	{
		_vdType = ANY_EMPTY;
		_vdValue.valString = NULL;

		switch (value._vdType)
		{
		case ANY_BOOL:
			operator =(value._vdValue.valBool);
			break;
		case ANY_INT:
			operator =(value._vdValue.valInt);
			break;
		case ANY_INT64:
			operator =(value._vdValue.valInt64);
			break;
		case ANY_DOUBLE:
			operator =(value._vdValue.valDouble);
			break;
		case ANY_STRING:
			operator =(value._vdValue.valString);
			break;
		case ANY_GUID:
			operator =(*value._vdValue.valGuid);
			break;
		case ANY_STREAM:
		{
			BYTE* valStream = NULL;
			int nByte = value.GetStreamValue(valStream);
			SetStreamValue(valStream, nByte);
		}
		break;
		case ANY_EMPTY:
		default:
			_vdType = ANY_EMPTY;
			_vdValue.valString = NULL;
			break;
		}
	}

	CAny& CAny::operator = (const CAny& value)
	{
		ClearTypeMemory();
		switch (value._vdType)
		{
		case ANY_BOOL:
			operator =(value._vdValue.valBool);
			break;
		case ANY_INT:
			operator =(value._vdValue.valInt);
			break;
		case ANY_INT64:
			operator =(value._vdValue.valInt64);
			break;
		case ANY_DOUBLE:
			operator =(value._vdValue.valDouble);
			break;
		case ANY_STRING:
			operator =(value._vdValue.valString);
			break;
		case ANY_GUID:
			operator =(*value._vdValue.valGuid);
			break;
		case ANY_STREAM:
		{
			BYTE* valStream = NULL;
			int nByte = value.GetStreamValue(valStream);
			SetStreamValue(valStream, nByte);
		}
		break;
		case ANY_EMPTY:
		default:
			_vdType = ANY_EMPTY;
			_vdValue.valString = NULL;
			break;
		}

		return *this;
	}

	//GUID
	CAny::CAny(const GUID& value)
	{
		_vdType = ANY_GUID;

		_vdValue.valGuid = new GUID();
		*(_vdValue.valGuid) = value;
	}

	CAny& CAny::operator = (const GUID& value)
	{
		ClearTypeMemory();
		_vdType = ANY_GUID;
		_vdValue.valGuid = new GUID();
		*(_vdValue.valGuid) = value;

		return *this;
	}

	CAny::operator GUID() const
	{
		GUID guid = { 0 };
		switch (_vdType)
		{
		case ANY_GUID:
			return *(_vdValue.valGuid);
		default:
			return guid;
		}
	}

	//CGuid
	CAny::CAny(const CGuid& value)
	{
		_vdType = ANY_GUID;
		_vdValue.valGuid = new GUID();
		*(_vdValue.valGuid) = GUID(value);
	}

	CAny& CAny::operator = (const CGuid& value)
	{
		ClearTypeMemory();
		_vdType = ANY_GUID;
		_vdValue.valGuid = new GUID();
		*(_vdValue.valGuid) = GUID(value);

		return *this;
	}

	CAny::operator CGuid() const
	{
		GUID guid = { 0 };
		switch (_vdType)
		{
		case ANY_GUID:
			return *(_vdValue.valGuid);
		default:
			return guid;
		}
	}

	CAny::CAny(const BYTE* value, int len)
	{
		SetStreamValue(value, len);
	}

	void CAny::SetStreamValue(const BYTE* value, int len)
	{
		ClearTypeMemory();
		_vdType = ANY_STREAM;

		_vdValue.valStream = new BYTE[len + sizeof(int)];

		// 赋值字节长度
		memcpy(_vdValue.valStream, (void*)(&len), sizeof(int));
		memcpy(_vdValue.valStream + sizeof(int), (void*)value, len);
	}

	int CAny::GetStreamValue(BYTE*& value) const
	{
		int nLen = 0;
		value = NULL;

		if (_vdType != ANY_STREAM || _vdValue.valStream == NULL)
			return nLen;

		// 获取字节长度
		memcpy((void*)(&nLen), _vdValue.valStream, sizeof(int));
		value = _vdValue.valStream + sizeof(int);

		return nLen;
	}

	BYTE* CAny::BuildStreamValue(int len)
	{
		ClearTypeMemory();
		_vdType = ANY_STREAM;

		_vdValue.valStream = new BYTE[len + sizeof(int)];

		// 赋值字节长度
		memcpy(_vdValue.valStream, (void*)(&len), sizeof(int));
		return _vdValue.valStream + sizeof(int);
	}

	CAny::CAny(const void* value)
	{
		SetVoidStreamValue(value);
	}

	void CAny::SetVoidStreamValue(const void* value)
	{
		ClearTypeMemory();

		BYTE* pData = (BYTE*)value;
		memcpy(&_vdType, pData, sizeof(int));
		int nValueLen = 0;
		memcpy(&nValueLen, pData + sizeof(int), sizeof(int));
		if (_vdType == ANY_EMPTY || nValueLen == 0)
			return;

		switch (_vdType)
		{
		case ANY_BOOL:
			memcpy(&_vdValue.valBool, pData + sizeof(int) * 2, nValueLen);
			break;
		case ANY_INT:
			memcpy(&_vdValue.valInt, pData + sizeof(int) * 2, nValueLen);
			break;
		case ANY_DOUBLE:
			memcpy(&_vdValue.valDouble, pData + sizeof(int) * 2, nValueLen);
			break;
		case ANY_STRING:
		{
			BYTE* pString = new BYTE[nValueLen];
			memcpy(pString, pData + sizeof(int) * 2, nValueLen);
			CString str;
			str.Format(L"%s", pString);
			operator =(str);
			//delete pString;
			break;
		}
		case ANY_GUID:
		{
			_vdValue.valGuid = new GUID;
			memcpy(_vdValue.valGuid, pData + sizeof(int) * 2, nValueLen);
			break;
		}
		case ANY_STREAM:
		{
			_vdValue.valStream = new BYTE[nValueLen];
			memcpy(_vdValue.valStream, pData + sizeof(int) * 2, nValueLen);
			break;
		}
		case ANY_EMPTY:
		default:
			break;
		}
	}

	int CAny::GetVoidStreamValue(void*& value) const
	{
		int nValueLen = 0;
		switch (_vdType)
		{
		case ANY_BOOL:
			nValueLen = sizeof(_vdValue.valBool);
			break;
		case ANY_INT:
			nValueLen = sizeof(_vdValue.valInt);
			break;
		case ANY_DOUBLE:
			nValueLen = sizeof(_vdValue.valDouble);
			break;
		case ANY_STRING:
			nValueLen = (int)(wcslen(_vdValue.valString) + 1) * (sizeof(WCHAR) / sizeof(BYTE));
			break;
		case ANY_GUID:
			nValueLen = sizeof(GUID);
			break;
		case ANY_STREAM:
			nValueLen = sizeof(_vdValue.valStream);
			break;
		case ANY_EMPTY:
		default:
			break;
		}

		// 变量类型 + 长度值本身 + 内容长度
		int len = sizeof(int) + sizeof(int) + nValueLen;
		BYTE* pNew = new BYTE[len];		// 外面需要析构

		int nType = _vdType;
		memcpy(pNew, (void*)(&nType), sizeof(int));	// 变量类型
		memcpy(pNew + (sizeof(int)), (void*)(&nValueLen), sizeof(int));	// 长度值本身

		switch (_vdType)
		{
		case ANY_BOOL:
			memcpy(pNew + (sizeof(int)) * 2, (void*)(&_vdValue.valBool), nValueLen);
			break;
		case ANY_INT:
			memcpy(pNew + (sizeof(int)) * 2, (void*)(&_vdValue.valInt), nValueLen);
			break;
		case ANY_DOUBLE:
			memcpy(pNew + (sizeof(int)) * 2, (void*)(&_vdValue.valDouble), nValueLen);
			break;
		case ANY_STRING:
			memcpy(pNew + (sizeof(int)) * 2, (void*)(_vdValue.valString), nValueLen);
			break;
		case ANY_GUID:
			memcpy(pNew + (sizeof(int)) * 2, (void*)(_vdValue.valGuid), nValueLen);
			break;
		case ANY_STREAM:
			memcpy(pNew + (sizeof(int)) * 2, (void*)(_vdValue.valStream), nValueLen);
			break;
		case ANY_EMPTY:
		default:
			break;
		}

		value = pNew;
		return len;	// 返回总长度
	}

	void CAny::ClearTypeMemory()
	{
		switch (_vdType)
		{
		case ANY_STRING:
			if (NULL != _vdValue.valString)
				delete[] _vdValue.valString;
			break;
		case ANY_STREAM:
			if (NULL != _vdValue.valStream)
				delete[] _vdValue.valStream;
			break;
		case ANY_GUID:
			if (NULL != _vdValue.valGuid)
				delete _vdValue.valGuid;
			break;
		}

		_vdType = ANY_EMPTY;
		_vdValue.valString = NULL;
	}

	bool CAny::IsEmpty() const
	{
		return _vdType == ANY_EMPTY;
	}

	EAnyType CAny::GetType() const
	{
		return (EAnyType)_vdType;
	}

	CString CAny::GetStringValue(int nDotNum) const
	{
		CString strDotNum, strFormat;
		switch (_vdType)
		{
		case ANY_EMPTY:
			return L"";
		case ANY_BOOL:
			return _vdValue.valBool ? L"是" : L"否";
		case ANY_INT:
			strFormat.Format(L"%d", _vdValue.valInt);
			return strFormat.GetString();
		case ANY_INT64:
			strFormat.Format(L"%I64d", _vdValue.valInt64);
			return strFormat.GetString();
		case ANY_DOUBLE:
			return CConvert::Float2Text(_vdValue.valDouble, nDotNum).GetString();
		case ANY_STRING:
			return (LPCWSTR)_vdValue.valString;
		case ANY_GUID:
			return CGuid(*_vdValue.valGuid).ToString().GetString();
		default:
			return L"";
		}
	}

	void CAny::SetStringValue(LPCWSTR lpValue, EAnyType vType)
	{
		CString strValue(lpValue);
		CString strDotNum, strFormat;
		switch (vType)
		{
		case ANY_EMPTY:
			break;
		case ANY_BOOL:
			operator =((strValue == L"是" || strValue == L"1") ? true : false);
			break;
		case ANY_INT:
			operator =(_wtoi(strValue));
			break;
		case ANY_INT64:
			operator =((INT64)(_tcstoui64(strValue, NULL, 10)));
			break;
		case ANY_DOUBLE:
			operator =(_wtof(strValue));
			break;
		case ANY_STRING:
			operator =(strValue);
			break;
		case ANY_GUID:
			KASSERT(FALSE);
			break;
		default:
			break;
		}
	}

	bool CAny::operator ==(const CAny& value) const
	{
		// 修复XMDX保存导致空字符串问题
		if((_vdType == ANY_EMPTY && value._vdType == ANY_STRING)
		  || (_vdType == ANY_STRING && value._vdType == ANY_EMPTY))
		{
			return (lstrlen(value._vdValue.valString) == lstrlen(_vdValue.valString));
		}

		if (_vdType != value._vdType)
			return false;

		switch (_vdType)
		{
		case ANY_EMPTY:
			return true;
		case ANY_BOOL:
			return value._vdValue.valBool == _vdValue.valBool;
		case ANY_INT:
			return value._vdValue.valInt == _vdValue.valInt;
		case ANY_INT64:
			return value._vdValue.valInt64 == _vdValue.valInt64;
		case ANY_DOUBLE:
			return value._vdValue.valDouble == _vdValue.valDouble;
		case ANY_STRING:
			return !wcscmp(value._vdValue.valString, _vdValue.valString);
		case ANY_GUID:
			return !((*_vdValue.valGuid) != (*value._vdValue.valGuid));
		case ANY_STREAM:
			return false;
		default:
			return true;
		}
		return true;
	}

	bool CAny::operator !=(const CAny& value) const
	{
		return !(*this == value);
	}
}