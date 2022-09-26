#pragma once

#include "..\Task.h"

class ProjectFile;

/**
 * This class allows recurring task definitions to be read from an MPP file.
 */
class RecurringTaskReader
{
public:
   /**
    * Constructor.
    * 
    * @param file parent project file
    */
	RecurringTaskReader(ProjectFile* file);

   /**
    * Reads recurring task data.
    * 
    * @param task Task instance
    * @param data recurring task data
    */
	void processRecurringTask(Task* task, ByteArrayPtr data);

private:
	ProjectFile* m_file;
};