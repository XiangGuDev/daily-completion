#pragma once

/**
 * This class represents a period of time.
 */
class COMMONCONTROLS_API DateRange : public ComparableT<DateRange>
{
	DateRange();
public:
   /**
    * Constructor.
    *
    * @param startDate start Date
    * @param endDate end Date
    */
	DateRange(DatePtr startDate, DatePtr endDate);

   /**
    * Retrieve the Date at the start of the range.
    *
    * @return start Date
    */
	DatePtr getStart();

   /**
    * Retrieve the Date at the end of the range.
    *
    * @return end Date
    */
	DatePtr getEnd();

   /**
    * This method compares a target Date with a Date range. The method will
    * return 0 if the Date is within the range, less than zero if the Date
    * is before the range starts, and greater than zero if the Date is after
    * the range ends. 
    * 
    * @param Date target Date
    * @return comparison result
    */
   int compareTo(DatePtr Date);

   /**
    * {@inheritDoc}
    */
   int compareTo(DateRange* o);

   ///**
   // * {@inheritDoc}
   // */
   //bool equals(Object o);

   ///**
   // * {@inheritDoc}
   // */
   //int hashCode();

   /**
    * {@inheritDoc}
    */
   virtual CString toString();

	static std::shared_ptr<DateRange> EMPTY_RANGE;// = new DateRange(NULL, NULL);

private:
	DatePtr m_start;
	DatePtr m_end;
};

typedef std::shared_ptr<DateRange> DateRangePtr;
typedef List<DateRangePtr> DateRangeList;
typedef std::shared_ptr<DateRangeList> DateRangeListPtr;