#pragma once

#include "ProjectWriter.h"

/**
 * This class contains utility methods for working with ProjectWriters.
 */
class COMMONCONTROLS_API ProjectWriterUtility
{
private:
   /**
    * Constructor.
    */
	ProjectWriterUtility();

public:
   /**
    * Retrieves a ProjectWriter instance which can write a file of the
    * type specified by the supplied file name. 
    * 
    * @param name file name
    * @return ProjectWriter instance
    */
	static ProjectWriterPtr getProjectWriter(CString name);

 //  /**
 //   * Retrieves a set containing the file extensions supported by the
 //   * getProjectWriter method.
 //   * 
 //   * @return set of file extensions
 //   */
	//static const List<CString>* getSupportedFileExtensions();
};