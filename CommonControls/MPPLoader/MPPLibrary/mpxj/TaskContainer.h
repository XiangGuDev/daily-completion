#pragma once

//#include "Task.h"

class Task;

/**
 * Interface implemented by classes which have child tasks.
 */

class TaskContainer
{
   /**
    * Retrieve a list of child tasks held by this object.
    * 
    * @return list of child tasks
    */
public:
	virtual List<Task*>* getChildTasks() = 0;
};