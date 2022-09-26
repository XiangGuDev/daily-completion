#pragma once

#include "..\Task.h"
#include "..\TimephasedWork.h"

/**
 * This class contains methods to create lists of Dates representing
 * task splits.
 */
class SplitTaskFactory
{
public:
   /**
    * Process the timephased resource assignment data to work out the 
    * split structure of the task.
    * 
    * @param task parent task
    * @param timephasedComplete completed resource assignment work
    * @param timephasedPlanned planned resource assignment work
    */
	void processSplitData(Task* task, TimephasedWorkListPtr timephasedComplete, TimephasedWorkListPtr timephasedPlanned);

};