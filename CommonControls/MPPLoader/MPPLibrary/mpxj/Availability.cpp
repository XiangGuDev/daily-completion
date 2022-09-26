#include "stdafx.h"
#include "Availability.h"
/**
* Constructor.
* 
* @param startDate start date
* @param endDate end date
* @param units units for the period
*/
Availability::Availability(DatePtr startDate, DatePtr endDate, NumberPtr units)
{
    m_range = new DateRange(startDate, endDate);
    m_units = units;
}

/**
* Retrieves the date range of the availability period.
* 
* @return start date
*/
DateRange* Availability::getRange()
{
    return m_range;
}

/**
* Retrieves the units for the availability period.
* 
* @return units
*/
NumberPtr Availability::getUnits()
{
    return m_units;
}

/**
 * {@inheritDoc}
 */
int Availability::compareTo(Availability* o)
{
   return m_range->compareTo(o->m_range);
}

///**
// * {@inheritDoc}
// */
//virtual public CString toString()
//{
//   return "[Availability range=" + m_range + " units=" + m_units + "]";
//}