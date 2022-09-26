#pragma once

#include "..\ProjectFile.h"
#include "..\poifs\DirectoryEntry.h"

/**
 * Common implementation detail to extract task constraint data from 
 * MPP9, MPP12, and MPP14 files.
 */
class COMMONCONTROLS_API ConstraintFactory
{
public:
   /**
    * Main entry point when called to process constraint data.
    * 
    * @param projectDir project directory
    * @param file parent project file
    */
	void process(DirectoryEntryPtr projectDir, ProjectFile* file);
};