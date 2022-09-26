#pragma once

#include <math.h>

class COMMONCONTROLS_API NumberHelper
{
public:
	static bool equals(double lhs, double rhs, double delta);

	static bool isDoubleString(LPCTSTR text);

	static bool isNumberString(LPCTSTR text);

	static double getDouble(ObjectPtr arg0);

	static bool getBool(ObjectPtr arg0);

	static int getInt(ObjectPtr arg0);

	static CString getString(ObjectPtr arg0);

	static LPCGUID getGuid(ObjectPtr arg0);

	static bool isNumber(ObjectPtr arg0);

	static double longBitsToDouble(__int64 arg0);

	/**
	* Utility method used to truncate a double to the given precision.
	*
	* @param value value to truncate
	* @param precision Number of decimals to truncate to.
	* @return double value
	*/
	static double truncate(double value, double precision);
};