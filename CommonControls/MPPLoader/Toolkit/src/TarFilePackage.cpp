#include "StdAfx.h"
#include "..\include\TarFilePackage.h"
#include "..\include\FileDataStream.h"
#include "..\include\MemoryDataStream.h"
#include "..\include\LibToolkit.h"
#include "..\include\FileToolkit.h"

using namespace BwProj::toolkit;

/** 资源库格式标识 */
#define TARFILE_HEADER_TEXTTAG		_T("BwProj Resource2 Database File")

/** 资源库异或加密密码 */
#define TARFILE_PASSWORD_DEFAULT	_T("0x420\x6F0\x6F0\x770\x610\x790\x52")


/** 当前版本号 */
#define TARFILE_CURRENT_VERSION	0

CTarFilePackage::CTarFilePackage(DWORD submitFlags)
{
	_srcChanged = FALSE;
	_submitFlags = submitFlags;
	setPassword  (TARFILE_PASSWORD_DEFAULT);
	_initFileHeaderStruct (&_fileHeader);
}

CTarFilePackage::~CTarFilePackage (void)
{
	close ();
}

/** 
* 关闭文件，会放弃调用submit后的所有操作
*/
void CTarFilePackage::close (void)
{
	// 删除文件列表
	for (size_t i = 0; i < _entryList.GetCount(); i ++)
		_deleteFileEntry(_entryList[i]);
	_entryList.RemoveAll();

	// 删除还添提交的文件
	for (size_t i = 0; i < _addEntry.GetCount(); i ++)
		_deleteFileEntry(_addEntry[i]);
	_addEntry.RemoveAll();

	if (_fileMapping.get())
	{
		_fileMapping.reset();
	}

	_srcChanged = FALSE;

	__super::close();
}

/**
* 取得文件枚举对象
* @return	返回文件枚举
*/
EntryEnumeration_ptr CTarFilePackage::getEntries (void)
{
	CEntryEnumeration* enumeration = new CEntryEnumeration ();
	CAtlArray<LPFILEENTRY>&	saveList = enumeration->getEntryList();

	// 加载已存在文件
	saveList.RemoveAll();
	for (size_t i = 0; i < _entryList.GetCount(); i ++)
		saveList.Add(_entryList[i]);

	// 加载新添加的文件
	for (size_t i = 0; i < _addEntry.GetCount(); i ++)
		saveList.Add(_addEntry[i]);

	return EntryEnumeration_ptr(enumeration);
}

/**
* 取得包内文件信息（不区分大小写）
* @param [in] name	文件名称
*/
LPCFILEENTRY CTarFilePackage::getEntry (LPCTSTR name)
{
	CString name2 (name);
	name2 = name2.MakeLower();

	EntryEnumeration_ptr list = getEntries();
	while (list->hasMoreElements ())
	{
		LPCFILEENTRY entry = list->nextElement();
		CString entry2 = entry->name;
		entry2 = entry2.MakeLower();

		if (name2 == entry2)
			return entry;
	}

	return NULL;
}

/**
* 添加文件到包中 
* @param [in] name		文件名称
* @param [in] stream	数据流读取对象
*/
void CTarFilePackage::putNextEntry (LPCTSTR name, RandomReadStream_ptr stream)
{	
	// 2013.6.20 jxh 线程池优化
	CCriticalSectionSync sc(_CSC);
	
	ULONGLONG packageLength = calcPackageLength(); 
	if ((packageLength >= MAXDWORD) || (stream->GetLength() > (MAXDWORD - packageLength)))
		throw BwProj::Exception(_T("文件已达到最大长度。"));

	if (_tcslen (name) >= MAX_PATH)
		throw BwProj::Exception(_T("文件名称超出允许的长度。"));

	// 先删除已存在的文件
	while (LPCFILEENTRY existEntry = getEntry (name))
	{
		removeEntry(existEntry);
	}

	// 添加到添加列表
	LPFILEENTRY entry = _newFileEntry (stream.release());
	_tcscpy_s(entry->name, MAX_PATH, name);
	_addEntry.Add(entry);
}

/**
* 删除包中的文件 
* @param [in] entry		文件索引
*/
void CTarFilePackage::removeEntry (LPCFILEENTRY entry)
{
	CAtlArray<LPFILEENTRY>* list = NULL;
	size_t index = 0;

	if (_findEntryInArray (entry, &list, &index))
	{
		_deleteFileEntry(list->GetAt(index));
		list->RemoveAt(index);

		// 标记原文件改变
		if (list == &_entryList)
			_srcChanged = TRUE;
	}
	else
	{
		throw BwProj::Exception (CString (entry->name) + _T(" 不存在。"));
	}
}

/**
* 取得文件内容 
* @param [in] entry		文件索引
* @return 返回文件流，流用完后要自己删除
*/
RandomReadStream_ptr CTarFilePackage::getReadStream (LPCFILEENTRY entry)
{
	if (!_findEntryInArray (entry, NULL, NULL))
		throw BwProj::Exception (CString (entry->name) + _T(" 不存在。"));

	switch (entry->extendType)
	{
	case ENTRY_EXTEND_EXISTENTRY:
		{
			// 从文件中读取数据到内存，然后再以流的方式返回
			ENTRYHEADER* ze = (ENTRYHEADER*)entry->extendData;
			CMemoryDataReadStream* stream = NULL;

			// 如果可转换成内存流，则取指针
			CMemoryDataReadStream* testStream =
				dynamic_cast<CMemoryDataReadStream*> (_openContext->readStream);

			if (testStream == NULL)
			{
				// 从文件中读取
				DWORD nBytesRead = 0;
				c_buffer_ptr buffer = c_buffer_ptr::alloc(ze->fileInfo.dataLength);
				_openContext->readStream->SetPosition(ze->fileInfo.dataOffset);
				_openContext->readStream->Read(buffer.get(), ze->fileInfo.dataLength, nBytesRead);

				if (_fileHeader.FileOptions & TARFILE_FLAGS_ENCRYPTDATA)
				{
					_encryptDataBuffer((LPBYTE) buffer.get(), buffer.length(),
						_filePassword, _filePassword.GetLength());
				}

				stream = new CMemoryDataReadStream (buffer);
			}
			else
			{
				// 从内存中读取
				char* buffer = (char*)testStream->getOriginalBuffer();
				buffer += ze->fileInfo.dataOffset;

				if (_fileHeader.FileOptions & TARFILE_FLAGS_ENCRYPTDATA)
				{
					// 为解密数据重新分配缓存区
					c_buffer_ptr bufptr = c_buffer_ptr::alloc(ze->fileInfo.dataLength);
					memcpy (bufptr.get(), buffer, ze->fileInfo.dataLength);

					_encryptDataBuffer((LPBYTE) bufptr.get(), bufptr.length(),
										_filePassword, _filePassword.GetLength());

					stream = new CMemoryDataReadStream (bufptr);
				}
				else
				{
					// 使用原始内存区
					stream = new CMemoryDataReadStream (buffer, ze->fileInfo.dataLength);
				}
			}

			return RandomReadStream_ptr (stream);
		}
		break;
	case ENTRY_EXTEND_NEWSTREAM:
		{
			// 从输入流中读取数据，只支持内存流
			CMemoryDataReadStream* stream = dynamic_cast<CMemoryDataReadStream*>
				((IDataReadStream*)entry->extendData);
			if (!stream)
			{
				throw BwProj::Exception (CString (entry->name) +
					_T("还在内存中，需要为CMemoryDataReadStream类型的原始数据流才能读取。"));
			}

			return RandomReadStream_ptr (new CMemoryDataReadStream(
									stream->getOriginalBuffer(), stream->GetLength()));
		}
		break;
	}

	return RandomReadStream_ptr();
}


void BwProj::toolkit::CTarFilePackage::_submit(CString tempPath)
{
	HANDLE tempHandle = ::CreateFile (tempPath, GENERIC_WRITE,
		FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_HIDDEN, NULL);
	if (tempHandle == INVALID_HANDLE_VALUE)
		throw BwProj::Exception(CLibToolkit::GetWin32LastError());

	// 写入文件
	try
	{
		_submitToFile (tempHandle);
		CloseHandle(tempHandle);
	}
	catch (BwProj::Exception& e)
	{
		CloseHandle(tempHandle);
		DeleteFile (tempPath);
		throw BwProj::Exception (e);
	}	
}


/** 把当前文件写入新文件 */
void CTarFilePackage::_submitToFile (HANDLE handle)
{
	// 计算要写入数据表大小
	DWORD entryCount = 0;
	DWORD tableSizes = _calcDataTableLength (&entryCount);
	
	CFileDataWriteStream stream (handle);

	// 写入文件头及entry信息
	TARFILEHEADER tempHeader = { 0 };
	_initFileHeaderStruct(&tempHeader);
	tempHeader.DataTablesSize = tableSizes;
	tempHeader.DataTablesPos = sizeof(TARFILEHEADER);

	if (tempHeader.FileOptions & TARFILE_FLAGS_ENCRYPTTABLE)
	{
		_encryptDataBuffer((LPBYTE)&tempHeader, sizeof(TARFILEHEADER),
							_filePassword, _filePassword.GetLength());
	}

	stream.Write (&tempHeader, sizeof(TARFILEHEADER));

	CMemoryDataWriteStream listStream;
	EntryEnumeration_ptr list = getEntries();
	DWORD dataOffset = sizeof (TARFILEHEADER) + tableSizes;
	listStream.Write(&entryCount, sizeof (DWORD));	/**< 写入文件数量 */
	while (list->hasMoreElements())
	{
		ENTRYHEADER entryHeader = { 0 };
		LPCFILEENTRY entry = list->nextElement();

		switch (entry->extendType)
		{
		case ENTRY_EXTEND_EXISTENTRY:
			{
				entryHeader = *(LPENTRYHEADER) entry->extendData;
			}
			break;
		case ENTRY_EXTEND_NEWSTREAM:
			{
				IRandomReadStream* newstream = (IRandomReadStream*) entry->extendData;
				entryHeader.fileInfo.dataLength = newstream->GetLength();
				entryHeader.fileInfo.nameLength = _tcslen (entry->name) + 1;
				_tcscpy_s(entryHeader.fileName, MAX_PATH, entry->name);
			}
			break;
		}

		entryHeader.fileInfo.dataOffset = dataOffset;
		listStream.Write(&entryHeader.fileInfo, sizeof (ENTRYINFO));
		listStream.Write(entryHeader.fileName, entryHeader.fileInfo.nameLength * sizeof(TCHAR));
		dataOffset += entryHeader.fileInfo.dataLength;
	}

	c_buffer_ptr listBuffer = c_buffer_ptr::alloc(listStream.GetLength());
	listStream.CopyBuffer(listBuffer.get());
	if (tempHeader.FileOptions & TARFILE_FLAGS_ENCRYPTTABLE)
	{
		_encryptDataBuffer((LPBYTE)listBuffer.get(), listBuffer.length(),
							_filePassword, _filePassword.GetLength());
	}
	stream.Write(listBuffer.get(), listBuffer.length());

	// 写入文件数据
	list = getEntries();
	c_buffer_ptr readBuffer = c_buffer_ptr::alloc(10240);
	while (list->hasMoreElements())
	{
		LPCFILEENTRY entry = list->nextElement();
		RandomReadStream_ptr readStream = getReadStream(entry);

		// LY 2014-04-29 根据输入流尺寸来自适应分配缓冲区，以解决TARFILE_FLAGS_ENCRYPTDATA模式下由于保存与加载时
		// 调用_encryptDataBuffer函数输入数据差异而导致的数据文件解密识别BUG
		if (readBuffer.length() < entry->originalSize)
		{
			readBuffer = c_buffer_ptr::alloc(entry->originalSize);
		}

		DWORD nBytesRead = 0;
		while (readStream->Read(readBuffer.get(), entry->originalSize, nBytesRead))
		{
			if (!nBytesRead) break;
			if (tempHeader.FileOptions & TARFILE_FLAGS_ENCRYPTDATA)
			{
				_encryptDataBuffer((LPBYTE)readBuffer.get(), entry->originalSize,
									_filePassword, _filePassword.GetLength());
			}
			stream.Write(readBuffer.get(), nBytesRead);
		}
	}
}

/**
* 更新当前文件上下文，当文件打开后会调用
* @param [in] context	已创建并设置好的上下文
*/
void CTarFilePackage::_updateContext (OPENCONTEXT* context)
{
	// 如果是文件则打开数据流
	std::auto_ptr<CTarFileMapping> fileMap;
	if (context->fileHandle && !context->readStream)
	{
		fileMap.reset(new CTarFileMapping ());
		if (fileMap->MapFile(context->fileHandle) == S_OK)
		{
			ULARGE_INTEGER liFileSize;
			liFileSize.LowPart = ::GetFileSize(context->fileHandle, &liFileSize.HighPart);

			context->readStream = new CMemoryDataReadStream ((char*)(*fileMap), liFileSize.LowPart);
			context->closeDeleteReadStream = TRUE;
		}
		else
		{
			context->readStream = new CFileDataReadStream (context->fileHandle);
			context->closeDeleteReadStream = TRUE;
		}
	}

	// 要求数据流
	IRandomReadStream* stream = context->readStream;
	if (!stream)
		throw BwProj::Exception(_T("缺少数据流。"));

	// 读取文件头及文件列表
	TARFILEHEADER tempHeader = { 0 };
	if (stream->GetLength() >= sizeof (TARFILEHEADER))
	{
		DWORD nBytesRead = 0;
		stream->Read (&tempHeader, sizeof (TARFILEHEADER), nBytesRead);

		// 检查文件头格式是否正确(未加密格式则直接读取)
		if (_tcsicmp(tempHeader.HeaderTag, TARFILE_HEADER_TEXTTAG) != 0)
		{
			_encryptDataBuffer((LPBYTE)&tempHeader, sizeof(TARFILEHEADER),
								_filePassword, _filePassword.GetLength());
			if (_tcsicmp(tempHeader.HeaderTag, TARFILE_HEADER_TEXTTAG))
				throw BwProj::Exception (_T("文件头校验错误。"));
		}

		if (tempHeader.FileVersion > TARFILE_CURRENT_VERSION)
		{
			CString msg;
			msg.Format (_T("不支持高版本“%d”的文件。"), tempHeader.FileVersion);
			throw BwProj::Exception (msg);
		}

		memcpy (&_fileHeader, &tempHeader, sizeof (TARFILEHEADER));

		// 读取文件表头到内存
		c_buffer_ptr buffer = c_buffer_ptr::alloc(tempHeader.DataTablesSize);
		stream->Read (buffer.get(), tempHeader.DataTablesSize, nBytesRead);

		if (tempHeader.FileOptions & TARFILE_FLAGS_ENCRYPTTABLE)
		{
			_encryptDataBuffer((LPBYTE)buffer.get(), buffer.length(),
								_filePassword, _filePassword.GetLength());
		}

		// 读取数据文件列表
		_loadFileEntryList (buffer.get(), buffer.length());
	}

	if (fileMap.get())
	{
		_fileMapping.reset(fileMap.release());
	}

	__super::_updateContext (context);
}

/** 初使化文件头结构 */
void CTarFilePackage::_initFileHeaderStruct (TARFILEHEADER* header)
{
	memset (header, 0, sizeof (TARFILEHEADER));
	header->FileVersion = TARFILE_CURRENT_VERSION;
	header->FileOptions = _submitFlags;
	header->DataTablesSize = 0;
	header->DataTablesPos = sizeof(TARFILEHEADER);
	_tcscpy_s(header->HeaderTag, 30, TARFILE_HEADER_TEXTTAG);
}

/** 加密数据，解密也调用此函数，明文与密文长度相同 */
DWORD CTarFilePackage::_encryptDataBuffer(LPBYTE pDataBuffer,
						DWORD dwLength, LPCTSTR lpPasswrod, DWORD dwPwdLength)
{
	ATLASSERT(pDataBuffer != NULL && lpPasswrod != NULL);
	if (pDataBuffer == NULL || lpPasswrod == NULL || dwLength == 0 || dwPwdLength == 0)
	{
		ATLASSERT(FALSE);
		return 0;
	}

	for (DWORD i = 0, j = 0; i < dwLength; ++i,++j)
	{
		if (j > dwPwdLength)
			j = 0;

		pDataBuffer[i] ^= lpPasswrod[j];
	}

	return 0;
}

/** 读取数据文件列表 */
void CTarFilePackage::_loadFileEntryList(void* lpDatabaseBuffer, DWORD size)
{
	// 删除老数据
	for (size_t i = 0; i < _entryList.GetCount(); i ++)
		_deleteFileEntry(_entryList[i]);
	_entryList.RemoveAll();

	// 头4个字节为entry数量
	CMemoryDataReadStream stream (lpDatabaseBuffer, size);
	DWORD nBytesRead = 0;

	DWORD entryCount = 0;
	stream.Read(&entryCount, sizeof (DWORD), nBytesRead);

	for (size_t i = 0; i < entryCount; ++i)
	{
		ENTRYHEADER entryHeader = { 0 };
		stream.Read(&entryHeader.fileInfo, sizeof (ENTRYINFO), nBytesRead);
		stream.Read(entryHeader.fileName, entryHeader.fileInfo.nameLength * sizeof(TCHAR), nBytesRead);

		_entryList.Add(_newFileEntry (&entryHeader));
	}
}

/** 创建一个文件对象 */
LPFILEENTRY CTarFilePackage::_newFileEntry (CTarFilePackage::LPENTRYHEADER zipentry)
{
	LPFILEENTRY fileentry = new FILEENTRY ();
	memset (fileentry, 0, sizeof (FILEENTRY));

	if (zipentry)
	{
		fileentry->extendType = ENTRY_EXTEND_EXISTENTRY;
		fileentry->extendData = ::malloc(sizeof (ENTRYHEADER));
		memcpy (fileentry->extendData, zipentry, sizeof (ENTRYHEADER));

		_tcscpy_s(fileentry->name, MAX_PATH, zipentry->fileName);
		fileentry->originalSize = zipentry->fileInfo.dataLength;
		fileentry->compressedSize = zipentry->fileInfo.dataLength;
	}

	return fileentry;
}

LPFILEENTRY CTarFilePackage::_newFileEntry (IRandomReadStream* stream)
{
	LPFILEENTRY fileentry = new FILEENTRY ();
	memset (fileentry, 0, sizeof (FILEENTRY));

	if (stream)
	{
		fileentry->extendType = ENTRY_EXTEND_NEWSTREAM;
		fileentry->extendData = stream;

		fileentry->originalSize = stream->GetLength();
	}

	return fileentry;
}

/** 删除一个文件对象 */
void CTarFilePackage::_deleteFileEntry (LPFILEENTRY fileentry)
{
	if (fileentry->extendData)
	{
		switch (fileentry->extendType)
		{
		case ENTRY_EXTEND_EXISTENTRY:
			::free(fileentry->extendData);
			break;
		case ENTRY_EXTEND_NEWSTREAM:
			RandomReadStream_ptr stream ((IRandomReadStream*)fileentry->extendData);
			break;
		}
	}
	delete fileentry;
}

/** 在两个列表中查找已存在的结点 */
BOOL CTarFilePackage::_findEntryInArray (LPFILEENTRY find_entry,
										 CAtlArray<LPFILEENTRY>** save_ptr, size_t* save_idx)
{
	// 查找已存在文件
	for (size_t i = 0; i < _entryList.GetCount(); i ++)
	{
		if (_entryList[i] == find_entry)
		{
			if (save_ptr)
				*save_ptr = &_entryList;
			if (save_idx)
				*save_idx = i;
			return TRUE;
		}
	}

	// 查找新添加的文件
	for (size_t i = 0; i < _addEntry.GetCount(); i ++)
	{
		if (_addEntry[i] == find_entry)
		{
			if (save_ptr)
				*save_ptr = &_addEntry;
			if (save_idx)
				*save_idx = i;
			return TRUE;
		}
	}

	return FALSE;
}

/** 计算文件总长度 */
ULONGLONG CTarFilePackage::calcPackageLength (void)
{
	 ULONGLONG totalLength = sizeof (TARFILEHEADER) + _calcDataTableLength(NULL);
	 EntryEnumeration_ptr list = getEntries();
	 while (list->hasMoreElements())
	 {
		 LPCFILEENTRY entry = list->nextElement();

		 switch(entry->extendType)
		 {
		 case ENTRY_EXTEND_EXISTENTRY:
			 {
				LPENTRYHEADER entryHeader = (LPENTRYHEADER) entry->extendData;
				totalLength += entryHeader->fileInfo.dataLength;
			 }
			 break;
		 case ENTRY_EXTEND_NEWSTREAM:
			 {
				IRandomReadStream* newstream = (IRandomReadStream*) entry->extendData;
				totalLength += newstream->GetLength();
			 }
			 break;
		 }
	 }

	 return totalLength;
}

/** 计算数据表长度 */
DWORD CTarFilePackage::_calcDataTableLength (DWORD* totalCount)
{
	DWORD tableSizes = sizeof (DWORD);
	DWORD entryCount = 0;
	EntryEnumeration_ptr list = getEntries();
	while (list->hasMoreElements())
	{
		LPCFILEENTRY entry = list->nextElement();
		tableSizes += sizeof (ENTRYINFO);
		tableSizes += (_tcslen (entry->name) + 1) * sizeof (TCHAR);	/**< 包括\0结束符 */
		entryCount ++;
	}

	if (totalCount)
		*totalCount = entryCount;

	return tableSizes;
}

