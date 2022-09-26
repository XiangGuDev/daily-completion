#include "stdafx.h"
#include "CompObj.h"
#include "MPPUtility.h"

/**
* Constructor. Reads and processes the block data.
*
* @param is input stream
* @throws IOException on read failure
*/
CompObj::CompObj(BwProj::toolkit::IDataReadStream* is)
{
    int length;

    is->skip(28);	// ???

    length = readInt(is);
	m_applicationName = MPPUtility::ByteArray2String(readByteArray2(is, length).get(), 0, length - 1);

    if (m_applicationName == L"Microsoft Project 4.0")
    {
        m_fileFormat = "MSProject.MPP4";
        m_applicationID = "MSProject.Project.4";
    }
    else
    {
        length = readInt(is);
		m_fileFormat = MPPUtility::ByteArray2String(readByteArray2(is, length).get(), 0, length - 1);
        length = readInt(is);
        if (length > 0)
        {
			m_applicationID = MPPUtility::ByteArray2String(readByteArray2(is, length).get(), 0, length - 1);
        }
    }
}

/**
* Accessor method to retrieve the application name.
*
* @return Name of the application
*/
CString CompObj::getApplicationName()
{
    return (m_applicationName);
}

/**
* Accessor method to retrieve the application ID.
*
* @return Application ID
*/
CString CompObj::getApplicationID()
{
    return (m_applicationID);
}

/**
* Accessor method to retrieve the file format.
*
* @return File format
*/
CString CompObj::getFileFormat()
{
    return (m_fileFormat);
}