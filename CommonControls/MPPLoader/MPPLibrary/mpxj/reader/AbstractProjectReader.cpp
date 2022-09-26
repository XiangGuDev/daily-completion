#include "stdafx.h"
#include "AbstractProjectReader.h"
#include "../../../Toolkit/include/FileDataStream2.h"

/**
* {@inheritDoc}
*/
ProjectFilePtr AbstractProjectReader::read(CString fileName)
{
	BwProj::toolkit::CFileDataReadStream fis(NULL);
	if (!fis.OpenFile(fileName)) {
		CString tipMsg = CFileToolkit::GetFileLogicName(fileName) + L"：文件打开失败。";
		throw toolkit::Exception(tipMsg);
	}

	try
	{
		ProjectFilePtr projectFile = read(&fis);
		fis.CloseFile();
		return projectFile;
	}
	catch (toolkit::Exception& ex)
	{
		fis.CloseFile();
		CString tipMsg = CFileToolkit::GetFileLogicName(fileName) + L"：" + ex.what();
		throw toolkit::Exception (tipMsg);
	}
}

/**
* {@inheritDoc}
*/
ProjectFilePtr AbstractProjectReader::read(CAtlFile* file)
{
	BwProj::toolkit::CFileDataReadStream fis(*file);

	return read(&fis);
}