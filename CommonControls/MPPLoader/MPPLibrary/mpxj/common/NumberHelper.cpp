#include "stdafx.h"
#include "NumberHelper.h"

bool NumberHelper::equals(double lhs, double rhs, double delta)
{
	return abs(lhs - rhs) < delta;
}

double NumberHelper::getDouble(ObjectPtr arg0)
{
	VariantPtr var = std::dynamic_pointer_cast<Variant> (arg0);
	if (var)
		return var->doubleValue();
	return 0;
}

bool NumberHelper::getBool(ObjectPtr arg0)
{
	VariantPtr var = std::dynamic_pointer_cast<Variant> (arg0);
	if (var)
		return var->boolValue();
	return false;
}

int NumberHelper::getInt(ObjectPtr arg0)
{
	VariantPtr var = std::dynamic_pointer_cast<Variant> (arg0);
	if (var)
		return var->intValue();
	return 0;
}

CString NumberHelper::getString(ObjectPtr arg0)
{
	VariantPtr var = std::dynamic_pointer_cast<Variant> (arg0);
	if (var)
		return var->toString();
	return L"";
}

LPCGUID NumberHelper::getGuid(ObjectPtr arg0)
{
	VariantPtr var = std::dynamic_pointer_cast<Variant> (arg0);
	if (var)
		return var->guidValue();
	return NULL;
}

bool NumberHelper::isNumber(ObjectPtr arg0)
{
	VariantPtr var = std::dynamic_pointer_cast<Variant> (arg0);
	if (!var)return false;

	int type = var->type();
	if (type == VT_I4 || type == VT_R8)
		return true;
	return false;
}

double NumberHelper::longBitsToDouble(__int64 arg0)
{
	if (arg0 == 0) return 0.0;
	
	double result = *(((double*)&arg0));

	//BYTE* bytes = (BYTE*)&arg0;

	//double result = 0;
	//__int64* value = (__int64*)&result;
	//for (int i = 0; i < 8; i++)
	//{
	//	*value <<= 8;
	//	*value |= bytes[7 - i];
	//}

	return result;
}

/**
* Utility method used to truncate a double to the given precision.
*
* @param value value to truncate
* @param precision Number of decimals to truncate to.
* @return double value
*/
double NumberHelper::truncate(double value, double precision)
{
	double result;
	precision = pow(10, precision);
	if (value > 0)
	{
		result = floor(value * precision) / precision;
	}
	else
	{
		result = ceil(value * precision) / precision;
	}
	return result;
}

// 判断是否是浮点数
bool NumberHelper::isDoubleString(LPCTSTR text)
{
	int len = _tcslen(text);
	if (text == NULL || len == 0)
		return false;

	int dotcnt = 0; int numcnt = 0;
	for (int i = 0; i < len; i ++)
	{
		if (text[i] == '.')
		{
			dotcnt++;
			continue;
		}
		if (!_istdigit(text[i]))
			return false;
		numcnt++;
	}

	return dotcnt == 1 && numcnt > 0;
}

// 判断是否是整数
bool NumberHelper::isNumberString(LPCTSTR text)
{
	int len = _tcslen(text);
	if (text == NULL || len == 0)
		return false;

	for (int i = 0; i < len; i ++)
	{
		if (!_istdigit(text[i]))
			return false;
	}

	return true;
}