#include "stdafx.h"
#include "AbstractProjectWriter.h"
#include "../../../Toolkit/include/FileDataStream2.h"

/**
* {@inheritDoc}
*/
void AbstractProjectWriter::write(ProjectFile* projectFile, CString fileName)
{
	//// 先写到内存中，如果成功就继续写到文件
	//BwProj::toolkit::CMemoryDataWriteStream2 buffer;
	//write(projectFile, &buffer);

	CAtlFile file;
	if(file.Create(fileName, GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS) != S_OK)
		throw toolkit::Exception(_T("写入文件失败。"));

	write(projectFile, &file);

	file.Close();
}

/**
* {@inheritDoc}
*/
void AbstractProjectWriter::write(ProjectFile* projectFile, CAtlFile* file)
{
	BwProj::toolkit::CFileDataWriteStream fso(*file);
	write(projectFile, &fso);
}