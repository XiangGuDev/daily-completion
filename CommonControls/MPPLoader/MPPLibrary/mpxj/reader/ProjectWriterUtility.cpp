#include "stdafx.h"
#include "ProjectWriterUtility.h"
#include "..\common\StringHelper.h"
#include "..\mspdi\MSPDIWriter.h"

/**
* Constructor.
*/
ProjectWriterUtility::ProjectWriterUtility()
{
    // Private constructor to prevent instantiation.
}

/**
* Retrieves a ProjectWriter instance which can write a file of the
* type specified by the supplied file name. 
* 
* @param name file name
* @return ProjectWriter instance
*/
ProjectWriterPtr ProjectWriterUtility::getProjectWriter(CString name)
{
	int index = StringHelper::lastIndexOf(name, L".");
    if (index == -1)
    {
		throw toolkit::Exception(L"Filename has no extension");
    }

	CString extension = name.Mid(index + 1).MakeUpper();

	ProjectWriterPtr file;

	if (extension == L"XML")
		file.reset (new MSPDIWriter()); 
	else if (extension == L"BPP")
		file.reset(new MSPDIWriter());
	else
		throw toolkit::Exception(L"不支持的文件类型“" + extension + L"”。");

    return (file);
}

///**
//* Retrieves a set containing the file extensions supported by the
//* getProjectWriter method.
//* 
//* @return set of file extensions
//*/
//const List<CString>* ProjectWriterUtility::getSupportedFileExtensions()
//{
//    return (WRITER_MAP.keySet());
//}