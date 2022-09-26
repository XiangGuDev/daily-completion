#pragma once

#include "..\Duration.h"

/**
 * This class parses and represents an xsd:duration value.
 */
class XsdDuration : public Object2
{
public:
   /**
    * Constructor. Parses the xsd:duration value and extracts the
    * duration data from it.
    *
    * @param duration value formatted as an xsd:duration
    */
	XsdDuration(CString duration);

   /**
    * This constructor allows an xsd:duration to be created from
    * an MPX duration.
    *
    * @param duration An MPX duration.
    */
	XsdDuration(DurationPtr duration);

   /**
    * Retrieves the number of days.
    *
    * @return int
    */
	int getDays();

   /**
    * Retrieves the number of hours.
    *
    * @return int
    */
	int getHours();

   /**
    * Retrieves the number of minutes.
    *
    * @return int
    */
	int getMinutes();

   /**
    * Retrieves the number of months.
    *
    * @return int
    */
	int getMonths();

   /**
    * Retrieves the number of seconds.
    *
    * @return double
    */
	double getSeconds();

   /**
    * Retrieves the number of years.
    *
    * @return int
    */
	int getYears();

   /**
    * This method generates the string representation of an xsd:duration value.
    *
    * @return xsd:duration value
    */
	virtual CString toString();

private:
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
	int readComponent(CString duration, int index, int length);

    bool m_hasTime;
    int m_years;
    int m_months;
    int m_days;
    int m_hours;
    int m_minutes;
    double m_seconds;

   ///**
   // * Configure the decimal separator to be independent of the
   // * one used by the default locale.
   // */
   // static DecimalFormatSymbols SYMBOLS = new DecimalFormatSymbols();
   //static
   //{
   //   SYMBOLS.setDecimalSeparator('.');
   //}

	static NumberFormat FORMAT;// = new DecimalFormat("#", SYMBOLS);
};

typedef std::shared_ptr<XsdDuration> XsdDurationPtr;