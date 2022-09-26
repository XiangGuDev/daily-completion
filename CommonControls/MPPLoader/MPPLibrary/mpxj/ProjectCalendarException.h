#pragma once

#include "ProjectCalendarDateRanges.h"

/**
 * This class represents instances of CalendarPtr Exception records from
 * an MPX file. It is used to define exceptions to the working days described
 * in both base and resource calendars.
 */
class COMMONCONTROLS_API ProjectCalendarException : public ProjectCalendarDateRanges,
	public ComparableT<ProjectCalendarException>, public Object2
{
public:
   /**
    * Package private constructor.
    * 
    * @param fromDate exception start date
    * @param toDate exception end date
    */
	ProjectCalendarException(DatePtr fromDate, DatePtr toDate);

   /**
    * Returns the from date.
    *
    * @return Date
    */
	DatePtr getFromDate();

   /**
    * Get to date.
    *
    * @return Date
    */
	DatePtr getToDate();

   /**
    * Gets working status.
    *
    * @return bool value
    */
	bool getWorking();

   /**
    * This method determines whether the given date falls in the range of
    * dates covered by this exception. Note that this method assumes that both
    * the start and end date of this exception have been set.
    *
    * @param date Date to be tested
    * @return bool value
    */
	bool contains(DatePtr date);

   /**
    * {@inheritDoc}
    */
	virtual int compareTo(ProjectCalendarException* o);

	CString GetNote();

	void SetNote(CString strNote);
 //  /**
 //   * {@inheritDoc}
 //   */
	//virtual CString toString();

private:
   DatePtr m_fromDate;
   DatePtr m_toDate;
   CString m_strExceptionNote;
};

typedef std::shared_ptr<ProjectCalendarException> ProjectCalendarExceptionPtr;