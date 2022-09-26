#include "stdafx.h"
#include "Priority.h"
//#include "GarbageCollector.h"

/**
* Array of type values matching the above constants.
*/
PriorityPtr Priority::VALUE[10];


/**
* This constructor takes the numeric enumerated representation of a
* priority and populates the class instance appropriately.
* Note that unrecognised values are treated as medium priorities.
*
* @param priority int representation of the priority
*/
Priority::Priority(int priority)
{
    if (priority < 0 || priority > DO_NOT_LEVEL)
    {
        m_value = MEDIUM;
    }
    else
    {
        m_value = priority;
    }
}

/**
* This method takes an integer enumeration of a priority
* and returns an appropriate instance of this class. Note that unrecognised
* values are treated as medium priority.
*
* @param priority int version of the priority
* @return Priority class instance
*/
PriorityPtr Priority::getInstance(int priority)
{
	//class free_static_priority : public IGcRecycle
	//{
	//public:
	//	virtual void gc()
	//	{
	//		for (int i = 0; i <= 0; i++)
	//			Priority::VALUE[i].reset();
	//	}
	//};

	if (VALUE[0] == NULL)
	{
		VALUE[0].reset(new Priority(LOWEST));
		VALUE[1].reset(new Priority(VERY_LOW));
		VALUE[2].reset(new Priority(LOWER));
		VALUE[3].reset(new Priority(LOW));
		VALUE[4].reset(new Priority(MEDIUM));
		VALUE[5].reset(new Priority(HIGH));
		VALUE[6].reset(new Priority(HIGHER));
		VALUE[7].reset(new Priority(VERY_HIGH));
		VALUE[8].reset(new Priority(HIGHEST));
		VALUE[9].reset(new Priority(DO_NOT_LEVEL));

		//GarbageCollector::add(new free_static_priority());
	}

    PriorityPtr result;

    if (priority >= LOWEST && priority <= DO_NOT_LEVEL && (priority % 100 == 0))
    {
        result = VALUE[(priority / 100) - 1];
    }
    else
    {
        result.reset (new Priority(priority));
    }

    return result;
}

/**
* Accessor method used to retrieve the numeric representation of the
* priority.
*
* @return int representation of the priority
*/
int Priority::getValue()
{
    return (m_value);
}