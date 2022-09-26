#pragma once

#include "..\poifs\POIFSFileSystem.h"

class MPPReader;
class ProjectFile;

//enum SaveVersion
//{
//	Project2002 = 9,
//	Project2003 = 9,
//	Project2007 = 12,
//	Project2010 = 14
//};

struct MPPVersionParams
{
	int FileVersion;			// SaveVersion
	LPCTSTR PropsEntry;			// Props14, Props12
	LPCTSTR ProjectDirEntry;	// L"   114"
};

/**
 * This interface is implemented by classes which understand how
 * to read one of the MPP file formats.
 */
class MPPVariantReader
{
public:
   /**
    * Reads an MPP file an populates the file data structure.
    *
    * @param reader parent file reader
    * @param file data structure to be populated
    * @param root Root of the POI file system.
    */
	virtual void process(MPPReader* reader, ProjectFile* file, std::shared_ptr<DirectoryEntry> root) = 0;

	// 获取不同版本参数
	virtual const MPPVersionParams* getVersionParams() = 0;
};

typedef std::shared_ptr<MPPVariantReader> MPPVariantReaderPtr;