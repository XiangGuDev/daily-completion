#pragma once

#include "DateRange.h"

/**
 * An abstract class representing a collection of Date ranges 
 * with supporting methods.
 */
class COMMONCONTROLS_API ProjectCalendarDateRanges// : public Iterable<DateRange>
{
public:
   /**
    * Add a Date range to the list of Date ranges->
    *
    * @param range Date range
    */
	void addRange(DateRangePtr range);

   /**
    * Retrieve the Date range at the specified index.
    * The index is zero based, and this method will return
    * NULL if the requested Date range does not exist.
    *
    * @param index range index
    * @return Date range instance
    */
	DateRangePtr getRange(int index);

   /**
    * Replace a Date range at the specified index.
    * 
    * @param index range index
    * @param value DateRange instance
    */
	void setRange(int index, DateRangePtr value);

   /**
    * Retrieve an iterator to allow the list of Date ranges to be traversed.
    *
    * @return iterator.
    */
	//Iterator<DateRange> iterator();


   /**
    * Returns the number of Date ranges associated with this instance.
    * 
    * @return number of Date ranges
    */
	int getRangeCount();

private:
	List<DateRangePtr> m_ranges;
	//List<DateRange> m_ranges = new List<DateRange>();
};

typedef std::shared_ptr<ProjectCalendarDateRanges> ProjectCalendarDateRangesPtr;