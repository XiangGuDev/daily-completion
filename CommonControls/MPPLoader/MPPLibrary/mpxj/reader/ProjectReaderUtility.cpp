#include "stdafx.h"
#include "ProjectReaderUtility.h"
#include "..\common\StringHelper.h"
#include "..\mspdi\MSPDIReader.h"
#include "..\mpp\MPPReader.h"
#include "..\mpp\CompObj.h"
#include "..\..\..\Toolkit\include\FileDataStream2.h"
/**
* Constructor.
*/
ProjectReaderUtility::ProjectReaderUtility()
{
    // Private constructor to prevent instantiation.
}

/**
* Retrieves a ProjectReader instance which can read a file of the
* type specified by the supplied file name.  
* 
* @param name file name
* @return ProjectReader instance
*/
ProjectReaderPtr ProjectReaderUtility::getProjectReader(CString name)
{
	int index = StringHelper::lastIndexOf(name, L".");
    if (index == -1)
    {
		throw toolkit::Exception(L"没找到文件扩展名：" + name);
    }

    CString extension = name.Mid(index + 1).MakeUpper();

	ProjectReaderPtr file;
	if (extension == L"XML")
		file.reset (new MSPDIReader());
	else if (extension == L"MPP" || extension == L"MPT")
		file.reset (new MPPReader());
	else if (extension == L"BPP")
		file.reset(new MSPDIReader());
	
	else
		throw toolkit::Exception(L"不支持的文件类型：" + extension);

	return file;
}

CString ProjectReaderUtility::getApplicationName(CString strFileName)
{
	BwProj::toolkit::CFileDataReadStream fis(NULL);
	if (!fis.OpenFile(strFileName)) {
		CString tipMsg = CFileToolkit::GetFileLogicName(strFileName) + L"：文件打开失败。";
		throw toolkit::Exception(tipMsg);
	}

	POIFSFileSystem fs(&fis);
	DirectoryEntryPtr root = fs.getRoot();
	DocumentInputStream is(std::dynamic_pointer_cast<DocumentEntry> (root->getEntry(L"\1CompObj")));
	CompObj compObj(&is);

	fis.CloseFile();
	return compObj.getApplicationName();
}

///**
//* Retrieves a set containing the file extensions supported by the
//* getProjectReader method.
//* 
//* @return set of file extensions
//*/
//const List<CString>* ProjectReaderUtility::getSupportedFileExtensions()
//{
//    return (READER_MAP.keySet());
//}
//
//private static final Map<String, Class<? extends ProjectReader>> READER_MAP = new HashMap<String, Class<? extends ProjectReader>>();
//static
//{
//    READER_MAP.put("MPP", MPPReader.class);
//    READER_MAP.put("MPT", MPPReader.class);
//    READER_MAP.put("MPX", MPXReader.class);
//    READER_MAP.put("XML", MSPDIReader.class);
//    READER_MAP.put("MPD", MPDDatabaseReader.class);
//    READER_MAP.put("PLANNER", PlannerReader.class);
//    READER_MAP.put("XER", PrimaveraXERFileReader.class);
//    READER_MAP.put("PMXML", PrimaveraPMFileReader.class);
//    READER_MAP.put("PP", AstaFileReader.class);
//}