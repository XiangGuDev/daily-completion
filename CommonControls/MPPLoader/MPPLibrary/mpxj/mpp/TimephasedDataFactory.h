#pragma once

#include "..\ProjectCalendar.h"
#include "..\ResourceAssignment.h"
#include "..\TimephasedWorkContainer.h"
#include "..\TimephasedCostContainer.h"
#include "..\common\TimephasedWorkNormaliser.h"
#include "..\common\TimephasedCostNormaliser.h"

/**
 * This class contains methods to create lists of TimephasedWork
 * and TimephasedCost instances.
 */
class TimephasedDataFactory
{
public:
   /**
    * Given a block of data representing completed work, this method will
    * retrieve a set of TimephasedWork instances which represent
    * the day by day work carried out for a specific resource assignment.
    *
    * @param calendar calendar on which date calculations are based
    * @param startDate assignment start date 
    * @param data completed work data block
    * @return list of TimephasedWork instances
    */
	 TimephasedWorkListPtr getCompleteWork(ProjectCalendarPtr calendar, DatePtr startDate, ByteArrayPtr data);

   /**
    * Given a block of data representing planned work, this method will
    * retrieve a set of TimephasedWork instances which represent
    * the day by day work planned for a specific resource assignment.
    *
    * @param calendar calendar on which date calculations are based
    * @param startDate assignment start date
    * @param units assignment units 
    * @param data planned work data block
    * @param timephasedComplete list of complete work 
    * @return list of TimephasedWork instances 
    */
	TimephasedWorkListPtr getPlannedWork(ProjectCalendarPtr calendar, DatePtr startDate,
		double units, ByteArrayPtr data, TimephasedWorkListPtr timephasedComplete);

   /**
    * Test the list of TimephasedWork instances to see
    * if any of them have been modified. 
    * 
    * @param list list of TimephasedWork instances
    * @return bool flag
    */
	bool getWorkModified(TimephasedWorkListPtr list);

   /**
    * Extracts baseline work from the MPP file for a specific baseline.
    * Returns null if no baseline work is present, otherwise returns
    * a list of timephased work items.
    * 
    * @param assignment parent assignment
    * @param calendar baseline calendar
    * @param normaliser normaliser associated with this data
    * @param data timephased baseline work data block
    * @param raw flag indicating if this data is to be treated as raw
    * @return timephased work
    */
	TimephasedWorkContainerPtr getBaselineWork(Resource2AssignmentPtr assignment,
		ProjectCalendarPtr calendar, TimephasedWorkNormaliserPtr normaliser, ByteArrayPtr data, bool raw);

   /**
    * Extracts baseline cost from the MPP file for a specific baseline.
    * Returns null if no baseline cost is present, otherwise returns
    * a list of timephased work items.
    * 
    * @param calendar baseline calendar
    * @param normaliser normaliser associated with this data
    * @param data timephased baseline work data block
    * @param raw flag indicating if this data is to be treated as raw
    * @return timephased work
    */
	TimephasedCostContainerPtr getBaselineCost(ProjectCalendarPtr calendar,
		TimephasedCostNormaliserPtr normaliser, ByteArrayPtr data, bool raw);

private:
   /**
    * Equality test cost values.
    * 
    * @param lhs cost value
    * @param rhs cost value
    * @return true if costs are equal, within the allowable delta
    */
	bool costEquals(double lhs, double rhs);

};