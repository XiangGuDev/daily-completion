#include "stdafx.h"
#include "XsdDuration.h"
#include "..\java\StringBuilder.h"

NumberFormat XsdDuration::FORMAT(L"#");

/**
* Constructor. Parses the xsd:duration value and extracts the
* duration data from it.
*
* @param duration value formatted as an xsd:duration
*/
XsdDuration::XsdDuration(CString duration)
:m_hasTime(false), m_years(0), m_months(0), m_days(0), m_hours(0), m_minutes(0), m_seconds(0)
{
	int length = duration.GetLength();
	if (length == 0)
		return;

    if (duration.GetAt(0) != 'P')
    {
		if (length < 2 || (duration.GetAt(0) != '-' && duration.GetAt(1) != 'P'))
        {
            throw new toolkit::Exception(duration);
        }
    }

    int index = 0;
    bool negative = false;
	if (duration.GetAt(0) == '-')
    {
        index = 2;
        negative = true;
    }
    else
    {
        index = 1;
        negative = false;
    }

    while (index < length)
    {
        index = readComponent(duration, index, length);
    }

    if (negative == true)
    {
        m_years = -m_years;
        m_months = -m_months;
        m_days = -m_days;
        m_hours = -m_hours;
        m_minutes = -m_minutes;
        m_seconds = -m_seconds;
    }
}

/**
* This constructor allows an xsd:duration to be created from
* an MPX duration.
*
* @param duration An MPX duration.
*/
XsdDuration::XsdDuration(DurationPtr duration)
:m_hasTime(false), m_years(0), m_months(0), m_days(0), m_hours(0), m_minutes(0), m_seconds(0)
{
	if (duration == NULL)
		return;

    double amount = duration->getDuration();

	/*if (((int)amount) == 0)
		return;*/

	switch (duration->getUnits())
	{
	case TimeUnit::MINUTES:
	case TimeUnit::ELAPSED_MINUTES:
		{
			m_minutes = (int) amount;
			m_seconds = (amount * 60) - (m_minutes * 60);
			break;
		}

	case TimeUnit::HOURS:
	case TimeUnit::ELAPSED_HOURS:
		{
			m_hours = (int) amount;
			amount = (amount * 60) - (m_hours * 60);
			m_minutes = (int) amount;
			m_seconds = (amount * 60) - (m_minutes * 60);
			break;
		}

	case TimeUnit::DAYS:
	case TimeUnit::ELAPSED_DAYS:
		{
			m_days = (int) amount;
			amount = (amount * 24) - (m_days * 24);
			m_hours = (int) amount;
			amount = (amount * 60) - (m_hours * 60);
			m_minutes = (int) amount;
			m_seconds = (amount * 60) - (m_minutes * 60);
			break;
		}

	case TimeUnit::WEEKS:
	case TimeUnit::ELAPSED_WEEKS:
		{
			amount *= 7;
			m_days = (int) amount;
			amount = (amount * 24) - (m_days * 24);
			m_hours = (int) amount;
			amount = (amount * 60) - (m_hours * 60);
			m_minutes = (int) amount;
			m_seconds = (amount * 60) - (m_minutes * 60);
			break;
		}

	case TimeUnit::MONTHS:
	case TimeUnit::ELAPSED_MONTHS:
		{
			m_months = (int) amount;
			amount = (amount * 28) - (m_months * 28);
			m_days = (int) amount;
			amount = (amount * 24) - (m_days * 24);
			m_hours = (int) amount;
			amount = (amount * 60) - (m_hours * 60);
			m_minutes = (int) amount;
			m_seconds = (amount * 60) - (m_minutes * 60);
			break;
		}

	case TimeUnit::YEARS:
	case TimeUnit::ELAPSED_YEARS:
		{
			m_years = (int) amount;
			amount = (amount * 12) - (m_years * 12);
			m_months = (int) amount;
			amount = (amount * 28) - (m_months * 28);
			m_days = (int) amount;
			amount = (amount * 24) - (m_days * 24);
			m_hours = (int) amount;
			amount = (amount * 60) - (m_hours * 60);
			m_minutes = (int) amount;
			m_seconds = (amount * 60) - (m_minutes * 60);
			break;
		}

		default:
		{
			break;
		}
	}
}

/**
* This method is called repeatedly to parse each duration component
* from sorting data in xsd:duration format. Each component consists
* of a number, followed by a letter representing the type.
*
* @param duration xsd:duration formatted string
* @param index current position in the string
* @param length length of string
* @return current position in the string
*/
int XsdDuration::readComponent(CString duration, int index, int length)
{
    TCHAR c = 0;
    //StringBuilder number = new StringBuilder();
	CString number;

    while (index < length)
    {
        c = duration.GetAt(index);
		if (_istdigit(c) || c == '.')
        {
			number += c;
        }
        else
        {
			break;
        }

		index ++;
    }

    switch (c)
    {
        case 'Y':
        {
        m_years = _ttol(number);
        break;
        }

        case 'M':
        {
        if (m_hasTime == false)
        {
			m_months = _ttol(number);
        }
        else
        {
			m_minutes = _ttol(number);
        }
        break;
        }

        case 'D':
        {
			m_days = _ttol(number);
			break;
        }

        case 'T':
        {
			m_hasTime = true;
			break;
        }

        case 'H':
        {
			m_hours = _ttol(number);
			break;
        }

        case 'S':
        {
			m_seconds = _ttof(number);
			break;
        }

        default:
        {
			throw toolkit::Exception(duration);
        }
    }

    ++index;

    return (index);
}

/**
* Retrieves the number of days.
*
* @return int
*/
int XsdDuration::getDays()
{
    return (m_days);
}

/**
* Retrieves the number of hours.
*
* @return int
*/
int XsdDuration::getHours()
{
    return (m_hours);
}

/**
* Retrieves the number of minutes.
*
* @return int
*/
int XsdDuration::getMinutes()
{
    return (m_minutes);
}

/**
* Retrieves the number of months.
*
* @return int
*/
int XsdDuration::getMonths()
{
    return (m_months);
}

/**
* Retrieves the number of seconds.
*
* @return double
*/
double XsdDuration::getSeconds()
{
    return (m_seconds);
}

/**
* Retrieves the number of years.
*
* @return int
*/
int XsdDuration::getYears()
{
    return (m_years);
}

/**
* This method generates the string representation of an xsd:duration value.
*
* @return xsd:duration value
*/
CString XsdDuration::toString()
{
    StringBuilder buffer("P");
    bool negative = false;

    if (m_years != 0 || m_months != 0 || m_days != 0)
    {
        if (m_years < 0)
        {
        negative = true;
        buffer.append(-m_years);
        }
        else
        {
        buffer.append(m_years);
        }
        buffer.append("Y");

        if (m_months < 0)
        {
        negative = true;
        buffer.append(-m_months);
        }
        else
        {
        buffer.append(m_months);
        }
        buffer.append("M");

        if (m_days < 0)
        {
        negative = true;
        buffer.append(-m_days);
        }
        else
        {
        buffer.append(m_days);
        }
        buffer.append("D");
    }

    buffer.append("T");

    if (m_hours < 0)
    {
        negative = true;
        buffer.append(-m_hours);
    }
    else
    {
        buffer.append(m_hours);
    }
    buffer.append("H");

    if (m_minutes < 0)
    {
        negative = true;
        buffer.append(-m_minutes);
    }
    else
    {
        buffer.append(m_minutes);
    }
    buffer.append("M");

    if (m_seconds < 0)
    {
        negative = true;
        buffer.append(FORMAT.format(-m_seconds));
    }
    else
    {
        buffer.append(FORMAT.format(m_seconds));
    }
    buffer.append("S");

	CString result = buffer.toString();
    if (negative == true)
    {
		result.Insert(0, L"-");
        //buffer.insert(0, '-');
    }

	return result;// (buffer.toString());
}