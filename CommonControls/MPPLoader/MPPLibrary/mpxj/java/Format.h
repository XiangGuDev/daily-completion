  #pragma once

#include "Calendar.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// ∏Ò ΩªØ
//
class Format : public Object2
{
public:
	Format(LPCTSTR fmt);

protected:
	CString _fmt;
};

class DateFormat : public Format
{
public:
	DateFormat(LPCTSTR fmt);
	CString format(DatePtr value) const;
	DatePtr parse(CString str) const;

	void setIgnoreDate(bool val);
	void setIgnoreTime(bool val);

protected:
	bool	_ignoreDate;
	bool	_ignoreTime;
};

class NumberFormat : public Format
{
public:
	NumberFormat(LPCTSTR fmt);
	CString format(double value) const;
	double parse(CString str) const;
};