#include "StdAfx.h"
#include "..\include\IFilePackage.h"
#include "..\include\MemoryDataStream.h"
#include "..\include\FileDataStream.h"
#include "..\include\LibToolkit.h"
#include "..\include\FileToolkit.h"


using namespace BwProj::toolkit;
using namespace BwProj::toolkit;

/************************************************************************/
/* IFilePackage                                                         */
/************************************************************************/
IFilePackage::~IFilePackage(void)
{
	// 要求在派生类中关闭文件
	ATLASSERT (_openContext.get() == NULL);
}

/**
* 设置、读取加密用的密码
*/
void IFilePackage::setPassword (LPCTSTR password)
{
	_filePassword = password;
}

LPCTSTR IFilePackage::getPassword (void)
{
	return _filePassword;
}


/** 
* 以可读取模式打开文件
* @param [in] filePath	文件路径
*/
void IFilePackage::open (LPCTSTR filePath)
{
	if (_openContext.get())
		throw new BwProj::Exception (_T("文件已打开，请先关闭。"));

	// 打开文件
	HANDLE handle = ::CreateFile (filePath, GENERIC_READ | GENERIC_WRITE,
						FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
	if (handle == INVALID_HANDLE_VALUE)
		throw BwProj::Exception(CLibToolkit::GetWin32LastError());

	std::auto_ptr<OPENCONTEXT> context (new OPENCONTEXT ());
	context->fileHandle = handle;
	context->closeDeleteReadStream = TRUE;
	context->closeDeleteWriteStream = TRUE;
	context->readStream = NULL;
	context->writeStream = NULL;
	context->filename = filePath;

	CString errmsg;
	if (!_tryUpdateContext (context.get(), errmsg))
		throw BwProj::Exception (errmsg);

	context.release();
}

void IFilePackage::open (HANDLE fileHandle)
{
	if (_openContext.get())
		throw BwProj::Exception (_T("文件已打开，请先关闭。"));

	std::auto_ptr<OPENCONTEXT> context (new OPENCONTEXT ());
	context->fileHandle = fileHandle;
	context->closeDeleteReadStream = TRUE;
	context->closeDeleteWriteStream = TRUE;
	context->readStream = NULL;
	context->writeStream = NULL;

	CString errmsg;
	if (!_tryUpdateContext (context.get(), errmsg))
		throw BwProj::Exception (errmsg);

	context.release();
}

/** 
* 以只读取模式打开文件
* @param [in] fileStream	文件流
*/
void IFilePackage::open (IRandomReadStream* fileStream)
{
	if (_openContext.get())
		throw BwProj::Exception (_T("文件已打开，请先关闭。"));

	std::auto_ptr<OPENCONTEXT> context (new OPENCONTEXT ());
	context->fileHandle = NULL;
	context->closeDeleteReadStream = FALSE;
	context->closeDeleteWriteStream = TRUE;
	context->readStream = fileStream;
	context->writeStream = NULL;

	CString errmsg;
	if (_tryUpdateContext (context.get(), errmsg))
		throw BwProj::Exception (errmsg);

	context.release();
}

/** 
* 关闭文件，会放弃调用submit后的所有操作
*/
void IFilePackage::close (void)
{
	if (_openContext.get())
	{
		if (_openContext->fileHandle)
			::CloseHandle(_openContext->fileHandle);

		if (_openContext->closeDeleteReadStream && _openContext->readStream)
			delete _openContext->readStream;

		if (_openContext->closeDeleteWriteStream && _openContext->writeStream)
			delete _openContext->writeStream;

		_openContext.reset();
	}
}

/**
* 更新当前文件上下文，当文件打开后会调用
* @param [in] context	已创建并设置好的上下文
*/
void IFilePackage::_updateContext (OPENCONTEXT* context)
{
	_openContext.reset(context);
}

/** 尝试调用_updateContext，如果有异常则会释放context数据 */
BOOL IFilePackage::_tryUpdateContext (OPENCONTEXT* context, CString& errmsg)
{
	try
	{
		_updateContext (context);
		return TRUE;
	}
	catch (BwProj::Exception& e)
	{
		if (context->fileHandle)
			::CloseHandle(context->fileHandle);

		if (context->closeDeleteReadStream && context->readStream)
			delete context->readStream;

		if (context->closeDeleteWriteStream && context->writeStream)
			delete context->writeStream;
		errmsg = e.what();
		return FALSE;
	}
}

void IFilePackage::submit(CString filename)
{
	CString filePath;
	if (filename.IsEmpty())	// 未指定文件
	{
		// 使用原文件名
		if (_openContext->filename.IsEmpty())
		{
			throw BwProj::Exception (_T("未指定文件路径。"));
		}
		else
		{
			filePath = _openContext->filename;
		}
	}
	else // 指定了文件名
	{
		filePath = filename;
	}

	// 只能保存到文件中
	if (!_openContext->fileHandle)
		throw BwProj::Exception (_T("缺少文件句柄。"));

	CString tempPath = filePath;
	tempPath += _T(".") + CLibToolkit::CreateGuid ();
	_submit(tempPath);

	if (!CFileToolkit::FileExist(tempPath))
	{
		return;
	}

	close ();

	// 目标文件存在时，首先备份目标文件
	CString fileBuckup = CString (filePath) + _T(".") + CLibToolkit::CreateGuid();
	if (CFileToolkit::FileExist(filePath))
	{
		CopyFile (filePath, fileBuckup, FALSE);
		DeleteFile(filePath);
	}


	CString errmsg;
	if (!MoveFile (tempPath, filePath))
	{
		errmsg = filePath + _T("生成失败：") + CLibToolkit::GetWin32LastError();

		// 还原文件并重新打开文件
		MoveFile (fileBuckup, filePath);
	}
	::DeleteFile (tempPath);
	::DeleteFile (fileBuckup);

	// 去除隐藏属性
	DWORD dwAttrs = GetFileAttributes(filePath); 
	SetFileAttributes(filePath, dwAttrs & ~FILE_ATTRIBUTE_HIDDEN);

	// 重新打开文件
	open (filePath);

	if (!errmsg.IsEmpty())
	{
		throw BwProj::Exception(errmsg);
	}
}

/************************************************************************/
/* CEntryEnumeration                                                 */
/************************************************************************/
CEntryEnumeration::CEntryEnumeration ()
{
	_position = 0;
}

CEntryEnumeration::~CEntryEnumeration ()
{
	_entryList.RemoveAll();
}

BOOL CEntryEnumeration::hasMoreElements (void)
{
	return _position < _entryList.GetCount();
}

LPCFILEENTRY CEntryEnumeration::nextElement (void)
{
	if (_position >= _entryList.GetCount())
		return NULL;

	LPCFILEENTRY entry = _entryList[_position];
	_position ++;
	return entry;
}

CAtlArray<LPFILEENTRY>& CEntryEnumeration::getEntryList (void)
{
	return _entryList;
}