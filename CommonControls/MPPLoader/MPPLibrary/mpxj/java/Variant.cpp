#include "stdafx.h"
#include "Variant.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Variant
//

Variant::Variant() :_guid(NULL)
{
	_type = VT_EMPTY;
	_guid = { 0 };
}
Variant::Variant(bool value) : _guid(NULL)
{
	_type = VT_BOOL;
	_value = value;
}
Variant::Variant(int value) : _guid(NULL)
{
	_type = VT_I4;
	_value = value;
}

Variant::Variant(double value) :_guid(NULL)
{
	_type = VT_R8;
	_value = value;
}

Variant::Variant(LPCTSTR value) :_guid(NULL)
{
	_type = VT_BSTR;
	_value = value;
}

Variant::Variant(LPCGUID value)
{
	_type = VT_CLSID;
	_guid = new GUID();
	*_guid = *value;
}

Variant::Variant(const Variant& value)
:_type(VT_EMPTY), _guid(NULL)
{
	operator=(value);
}

Variant::~Variant()
{
	if (_type == VT_CLSID && _guid)
	{
		delete _guid;
		_guid = NULL;
	}
}

Variant& Variant::operator = (bool value)
{
	_type = VT_BOOL;
	_value = value;
	return *this;
}

Variant& Variant::operator = (int value)
{
	_type = VT_I4;
	_value = value;
	return *this;
}

Variant& Variant::operator = (double value)
{
	_type = VT_R8;
	_value = value;
	return *this;
}

Variant& Variant::operator = (LPCTSTR value)
{
	_type = VT_BSTR;
	_value = value;
	return *this;
}

Variant& Variant::operator = (LPCGUID value)
{
	_type = VT_CLSID;

	if (!_guid) _guid = new GUID();
	*_guid = *value;
	_value.Clear();
	return *this;
}

Variant& Variant::operator = (const Variant& value)
{
	_type = value._type;
	_value = value._value;

	if (value._guid)
	{
		if (!_guid) _guid = new GUID();
		*_guid = *value._guid;
	}

	return *this;
}


VARTYPE Variant::type()
{
	return _type;
}


Variant::operator double()
{
	return doubleValue();
}

Variant::operator int()
{
	return intValue();
}

Variant::operator bool()
{
	return boolValue();
}

Variant::operator CString()
{
	return stringValue();
}

Variant::operator LPCGUID()
{
	return guidValue();
}

double Variant::doubleValue()
{
	switch (_type)
	{
	case VT_R8:
		return _value.dblVal;
	case VT_I4:
		return _value.intVal;
	case VT_BOOL:
		return _value.boolVal == VARIANT_TRUE ? 1 : 0;
	case VT_BSTR:
		return _ttof(CString(_value.bstrVal));
	default:
		return 0;
	}
}

LPCGUID Variant::guidValue()
{
	return _guid;
}

int Variant::intValue()
{
	switch (_type)
	{
	case VT_R8:
		return (int)_value.dblVal;
	case VT_I4:
		return _value.intVal;
	case VT_BOOL:
		return _value.boolVal == VARIANT_TRUE ? 1 : 0;
	case VT_BSTR:
		return _ttol(CString(_value.bstrVal));
	default:
		return 0;
	}
}

bool Variant::boolValue()
{
	switch (_type)
	{
	case VT_R8:
		return _value.dblVal != 0;
	case VT_I4:
		return _value.intVal != 0;
	case VT_BOOL:
		return _value.boolVal == VARIANT_TRUE;
	case VT_BSTR:
	{
		static CString TRUE_1 = L"true";
		static CString TRUE_2 = L"1";
		CString txt(_value.bstrVal);
		if (txt == TRUE_1 || txt == TRUE_2)
			return true;
		return false;
	}
	}

	return false;
}

CString Variant::stringValue()
{
	//if (_toString == NULL)
	//{
	//	_toString = new CString();
	//}
	CString str;
	switch (_type)
	{
	case VT_R8:
		{
			str.Format(L"%f", _value.dblVal);
			str = CStringToolkit::TrimNumberString(str);
		}
		break;
	case VT_I4:
		str.Format(L"%d", _value.intVal);
		break;
	case VT_BOOL:
		str = _value.boolVal == VARIANT_TRUE ? L"true" : L"false";
		break;
	case VT_BSTR:
		str = _value.bstrVal;
		break;
	default:
		ATLASSERT(false);
	}

	return str;
}

CComVariant& Variant::getComVariant()
{
	return _value;
}

CString Variant::toString()
{
	return stringValue();
}


// ×Ö·û´®Ö¸Õë
StringPtr::operator CString()
{
	if (get())
		return *get();
	return L"";
}

StringPtr::operator LPCTSTR()
{
	if (get())
		return *get();
	return NULL;
}

void StringPtr::reset(LPCTSTR value)
{
	__super::reset(new CString(value));
}