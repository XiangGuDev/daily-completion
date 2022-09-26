// mpxc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "mpxj\reader\ProjectReaderUtility.h"
#include "mpxj\reader\ProjectWriterUtility.h"
//#include "mpxj\GarbageCollector.h"

void test_sample1_xml(LPCTSTR filename, LPCTSTR filename2)
{
	// 读文件
	ProjectReaderPtr reader = ProjectReaderUtility::getProjectReader(filename);
	ProjectFilePtr project = reader->read(filename);

	// 写文件
	ProjectWriterPtr writer = ProjectWriterUtility::getProjectWriter(filename2);
	writer->write(project.get(), filename2);
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	return 0;
}