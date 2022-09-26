#pragma once

#include "Object.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// 日期对象
//
class COMMONCONTROLS_API Date : public COleDateTime, public ComparableT<Date>, public Object2
{
public:
	Date();
	Date(DATE dtSrc);
	Date(COleDateTime& dtSrc);
	Date(time_t dtSrc);

	time_t getTime();

	virtual int compareTo(Date* arg0);

	virtual CString toString();

private:
#ifdef _DEBUG
	CString _textDate;
#endif
};

typedef std::shared_ptr<Date> DatePtr;