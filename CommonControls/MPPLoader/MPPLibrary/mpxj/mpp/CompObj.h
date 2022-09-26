#pragma once

#include "MPPComponent.h"

/**
 * This class handles reading the data found in the CompObj block
 * of an MPP file. The bits we can decypher allow us to determine
 * the file format.
 */
class CompObj : public MPPComponent
{
public:
	/**
	* Constructor. Reads and processes the block data.
	*
	* @param is input stream
	* @throws IOException on read failure
	*/
	CompObj(BwProj::toolkit::IDataReadStream* is);

	/**
	 * Accessor method to retrieve the application name.
	 *
	 * @return Name of the application
	 */
	CString getApplicationName();

	/**
	 * Accessor method to retrieve the application ID.
	 *
	 * @return Application ID
	 */
	CString getApplicationID();

	/**
	 * Accessor method to retrieve the file format.
	 *
	 * @return File format
	 */
	CString getFileFormat();

private:
	/**
	 * Application name.
	 */
	CString m_applicationName;

	/**
	 * Application identifier.
	 */
	CString m_applicationID;

	/**
	 * File format.
	 */
	CString m_fileFormat;
};

typedef std::shared_ptr<CompObj> CompObjPtr;