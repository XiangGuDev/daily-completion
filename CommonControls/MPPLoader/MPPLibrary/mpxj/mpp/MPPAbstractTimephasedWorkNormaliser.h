#pragma once

#include "..\common\AbstractTimephasedWorkNormaliser.h"

/**
 * Normalise timephased resource assignment data from an MPP file. 
 */
class MPPAbstractTimephasedWorkNormaliser : public AbstractTimephasedWorkNormaliser
{
public:
   /**
    * This method converts the internal representation of timephased 
    * resource assignment data used by MS Project into a standardised
    * format to make it easy to work with. 
    * 
    * @param calendar current calendar
    * @param list list of assignment data
    */
	virtual void normalise(ProjectCalendarPtr calendar, TimephasedWorkListPtr list);

protected:
   /**
    * This method merges together assignment data for the same day.
    * 
    * @param calendar current calendar
    * @param list assignment data
    */
	virtual void mergeSameDay(ProjectCalendarPtr calendar, TimephasedWorkListPtr list) = 0;

private:
	/**
	* This method breaks down spans of time into individual days.
	*
	* @param calendar current project calendar
	* @param list list of assignment data
	*/
	void splitDays(ProjectCalendarPtr calendar, TimephasedWorkListPtr list);

	/**
	* This method splits the first day off of a time span.
	*
	* @param calendar current calendar
	* @param assignment timephased assignment span
	* @return first day and remainder assignments
	*/
	TimephasedWorkArrayPtr splitFirstDay(ProjectCalendarPtr calendar, TimephasedWorkPtr assignment);

   /**
    * Retrieves the pro-rata work carried out on a given day.
    * 
    * @param calendar current calendar
    * @param assignment current assignment.
    * @return assignment work duration
    */
	DurationPtr getAssignmentWork(ProjectCalendarPtr calendar, TimephasedWorkPtr assignment);

	static const double EQUALITY_DELTA;
};