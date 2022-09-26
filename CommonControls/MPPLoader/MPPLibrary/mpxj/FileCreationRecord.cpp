#pragma once
#include "stdafx.h"
#include "FileCreationRecord.h"
#include "ProjectFile.h"

/**
* Default constructor.
*
* @param file the parent file to which this record belongs.
*/
FileCreationRecord::FileCreationRecord(ProjectFile* file)
	:ProjectEntity(file)
{
	setDelimiter(',');
	setProgramName(_T("Microsoft Project for Windows"));
	//setCodePage(CodePage.ANSI);
	setFileVersion(FileVersion::VERSION_4_0);
}

/**
* Sets the delimiter character, "," by default.
*
* @param delimiter delimiter character
*/
void FileCreationRecord::setDelimiter(char delimiter)
{
	m_delimiter = delimiter;
	getParentFile()->setDelimiter(m_delimiter);
}

/**
* Retrieves the delimiter character, "," by default.
*
* @return delimiter character
*/
char FileCreationRecord::getDelimiter()
{
	return (m_delimiter);
}

/**
* Program name file created by.
*
* @param programName system name
*/
void FileCreationRecord::setProgramName(LPCTSTR programName)
{
	m_programName = programName;
}

/**
* Program name file created by.
*
* @return program name
*/
LPCTSTR FileCreationRecord::getProgramName()
{
	return (m_programName);
}

/**
* Version of the MPX file->
*
* @param version MPX file version
*/
void FileCreationRecord::setFileVersion(FileVersion version)
{
	m_fileVersion = version;
}

/**
* Version of the MPX file->
*
* @return MPX file version
*/
FileVersion FileCreationRecord::getFileVersion()
{
	return (m_fileVersion);
}