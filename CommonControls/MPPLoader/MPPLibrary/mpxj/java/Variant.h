#pragma once

#include "Object.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// 基本数据类型
//

class COMMONCONTROLS_API Variant : public Object2
{
public:
	Variant();
	Variant(bool value);
	Variant(int value);
	Variant(double value);
	Variant(LPCTSTR value);
	Variant(LPCGUID value);
	Variant(const Variant& value);
	virtual ~Variant();

	Variant& operator = (bool value);
	Variant& operator = (int value);
	Variant& operator = (double value);
	Variant& operator = (LPCTSTR value);
	Variant& operator = (LPCGUID value);
	Variant& operator = (const Variant& value);

	VARTYPE type();

	operator double();
	operator int();
	operator bool();
	operator CString();
	operator LPCGUID();

	double doubleValue();
	LPCGUID guidValue();
	int intValue();
	bool boolValue();
	CString stringValue();

	CComVariant& getComVariant();

	virtual CString toString();

private:
	VARTYPE _type;
	CComVariant _value;
	LPGUID _guid;
	//CString* _toString;
};

typedef std::shared_ptr<Variant> VariantPtr;

///////////////////////////////////////////////////////////////////////////////////////////////////
// 基本类型定义
//
typedef Variant Number;
typedef Variant Integer;
typedef Variant Double;
typedef Variant Boolean;
typedef Number BigInteger;
typedef Number BigDecimal;

typedef std::shared_ptr<Number> NumberPtr;
typedef std::shared_ptr<BigInteger> BigIntegerPtr;
typedef std::shared_ptr<BigDecimal> BigDecimalPtr;
typedef std::shared_ptr<Integer> IntegerPtr;
typedef std::shared_ptr<Double> DoublePtr;
typedef std::shared_ptr<Boolean> BooleanPtr;

// 字符串指针，当没有字符串可返回空串，而不是NULL
class StringPtr : public std::shared_ptr<CString>
{
public:
	operator CString();	// 没有值时返回空串
	operator LPCTSTR();	// 没有值时返回NULL

	void reset(LPCTSTR value);
};