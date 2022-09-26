#pragma once

#include "ProjectReader.h"

#include "..\StaticResourceClear.h"

/**
 * This class contains utility methods for working with ProjectReaderss.
 */
class COMMONCONTROLS_API ProjectReaderUtility
{
private:
   /**
    * Constructor.
    */
	ProjectReaderUtility();

public:
   /**
    * Retrieves a ProjectReader instance which can read a file of the
    * type specified by the supplied file name.  
    * 
    * @param name file name
    * @return ProjectReader instance
    */
	static ProjectReaderPtr getProjectReader(CString name);


	/**
	* 获取打开文件的版本
	*
	* @param   文件全路径
	* @return password
	*/
	static CString getApplicationName(CString strFileName);
 //  /**
 //   * Retrieves a set containing the file extensions supported by the
 //   * getProjectReader method.
 //   * 
 //   * @return set of file extensions
 //   */
	//static const List<CString>* getSupportedFileExtensions();
};