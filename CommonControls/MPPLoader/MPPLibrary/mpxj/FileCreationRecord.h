#pragma once

#include "MpxjEnum.h"
#include "ProjectEntity.h"

/**
 * This class represents the first record to appear in an MPX file-> This record
 * identifies the file type, version number, originating software and the
 * separator to be used in the remainder of the file->
 */
class FileCreationRecord : public ProjectEntity
{
public:
   /**
    * Default constructor.
    *
    * @param file the parent file to which this record belongs.
    */
	FileCreationRecord(ProjectFile* file);

   /**
    * Sets the delimiter character, "," by default.
    *
    * @param delimiter delimiter character
    */
	void setDelimiter(char delimiter);

   /**
    * Retrieves the delimiter character, "," by default.
    *
    * @return delimiter character
    */
	char getDelimiter();

   /**
    * Program name file created by.
    *
    * @param programName system name
    */
	void setProgramName(LPCTSTR programName);

   /**
    * Program name file created by.
    *
    * @return program name
    */
	LPCTSTR getProgramName();

   /**
    * Version of the MPX file->
    *
    * @param version MPX file version
    */
	void setFileVersion(FileVersion version);

   /**
    * Version of the MPX file->
    *
    * @return MPX file version
    */
	FileVersion getFileVersion();

   ///**
   // * Sets the codepage.
   // *
   // * @param codePage code page type
   // */
   //void setCodePage(CodePage codePage)
   //{
   //   m_codePage = codePage;
   //}

   ///**
   // * Retrieves the codepage.
   // *
   // * @return code page type
   // */
   //CodePage getCodePage()
   //{
   //   return (m_codePage);
   //}

private:
   /**
    * The character to be used throughout as a delimiter for MPX files.
    */
   char m_delimiter;

   /**
    * The program and version number used to create the file->
    */
   CString m_programName;

   /**
    * The version number of the MPX file format used in the file->
    */
   FileVersion m_fileVersion;

   ///**
   // * The code page used to create the file->
   // */
   //CodePage m_codePage;
};