#pragma once

#include "DateRange.h"

/** 
 * This class represents a row from a resource's availability table.
 */
class Availability : public ComparableT<Availability>, public Object2
{
public:
   /**
    * Constructor.
    * 
    * @param startDate start date
    * @param endDate end date
    * @param units units for the period
    */
	Availability(DatePtr startDate, DatePtr endDate, NumberPtr units);

   /**
    * Retrieves the date range of the availability period.
    * 
    * @return start date
    */
	DateRange* getRange();

   /**
    * Retrieves the units for the availability period.
    * 
    * @return units
    */
	NumberPtr getUnits();

   /**
    * {@inheritDoc}
    */
	virtual int compareTo(Availability* o);

   ///**
   // * {@inheritDoc}
   // */
   //virtual public CString toString()
   //{
   //   return "[Availability range=" + m_range + " units=" + m_units + "]";
   //}

private:
	DateRange* m_range;
	NumberPtr m_units;
};

typedef std::shared_ptr<Availability> AvailabilityPtr;