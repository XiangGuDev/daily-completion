#pragma once

#include "Props.h"
#include "..\poifs\DirectoryEntry.h"
class ProjectFile;

/**
 * This class reads project header data from MPP8, MPP9, and MPP12 files.
 */
class ProjectHeaderReader
{
public:
   /**
    * The main entry point for processing project header data.
    * 
    * @param file parent project file
    * @param props properties data
    * @param rootDir Root of the POI file system.
    */
	void process(ProjectFile* file, PropsPtr props, DirectoryEntryPtr rootDir);
};