#include "StdAfx.h"
#include "FileDataStream.h"

//------------------------------------------------------------------
// CFileDataReadStream 定义文档读数据流
//------------------------------------------------------------------

namespace CommBase
{

#define DEFCACHEBYTES (32 * 1024)

	CFileDataReadStream::CFileDataReadStream(HANDLE file)
	{
		if (file)
		{
			_File.Attach(file);
		}

		_DefCacheBytes = DEFCACHEBYTES;

		_CacheBlock = new char[_DefCacheBytes];

		_CachePosition = 0;
		_CacheReadBytes = 0;
		_CacheFilePosition = 0;
	}

	CFileDataReadStream::~CFileDataReadStream(void)
	{
		_File.Detach();

		if (_CacheBlock)
			delete[] _CacheBlock;
	}

	BOOL CFileDataReadStream::OpenFile(LPCTSTR szFilename)
	{
		ATLASSUME(((HANDLE)_File) == NULL);

		HANDLE hFile = ::CreateFile(szFilename, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
			return FALSE;

		_File.Attach(hFile);

		return TRUE;
	}

	void CFileDataReadStream::CloseFile(void)
	{
		_CachePosition = 0;
		_CacheReadBytes = 0;

		_File.Close();
	}

	CFileDataReadStream::operator HANDLE() const throw()
	{
		return (HANDLE)_File;
	}

	/** 读取指定字节到缓冲区 */
	BOOL CFileDataReadStream::Read(LPVOID pBuffer, DWORD nBufSize, DWORD& nBytesRead)
	{
		DWORD	sumReadBytes = 0;	// 已写入字节数
		LPBYTE	bBuffer = (LPBYTE)pBuffer;

		while (sumReadBytes < nBufSize)
		{
			// 测试 Cache 中是否还有数据可读，如果没有则要更新缓存
			if (_CachePosition >= _CacheReadBytes)
			{
				// 从磁盘中读数据
				_File.GetPosition(_CacheFilePosition);
				if (_File.Read(_CacheBlock, _DefCacheBytes, _CacheReadBytes) != S_OK)
					return FALSE;

				_CachePosition = 0;
			}

			// 计算剩余要读取数据长度
			DWORD needReadBytes = nBufSize - sumReadBytes;

			// 计算 cache 中可读数据长度
			DWORD cacheBackBytes = _CacheReadBytes - _CachePosition;

			// 计算可以从 cache 中读取的数据长度，如果没有数据可读取，则可能出错了
			DWORD cacheReadBytes = cacheBackBytes >= needReadBytes ? needReadBytes : cacheBackBytes;
			if (cacheReadBytes == 0)
				break;

			// 从内存中复制数据
			memcpy(bBuffer, &_CacheBlock[_CachePosition], cacheReadBytes);
			_CachePosition += cacheReadBytes;
			bBuffer += cacheReadBytes;
			sumReadBytes += (DWORD)cacheReadBytes;
		}

		nBytesRead = sumReadBytes;

		return sumReadBytes == nBufSize;
	}

	/**
	* 取提当前指针位置
	* @return	返回指针位置
	*/
	DWORD CFileDataReadStream::GetPosition()
	{
		return (DWORD)(_CacheFilePosition + _CachePosition);
	}
	/**
	* 设置当前指针位置
	* @return	成功返回，失败返回假（如地址无效）
	*/
	BOOL CFileDataReadStream::SetPosition(DWORD position)
	{
		// 文件读取流的位置要在 0到GetLength()范围内 SHl
		if (position > GetLength())
			return FALSE;

		// 看新位置是否在cache范围内
		if (position >= _CacheFilePosition && position < _CacheFilePosition + _CacheReadBytes)
		{
			_CachePosition = (DWORD)(position - _CacheFilePosition);
		}
		else
		{
			HRESULT r = _File.Seek(position, FILE_BEGIN);
			if (r == S_OK)
			{
				_CachePosition = 0;
				_CacheReadBytes = 0;
				_CacheFilePosition = position;
				return TRUE;
			}
			return FALSE;
		}

		return TRUE;
	}
	/**
	* 取得当前可访问数据长度
	* @return	返回数据长度
	*/
	DWORD CFileDataReadStream::GetLength()
	{
		ULONGLONG fileSize = 0;
		if (_File.GetSize(fileSize) != S_OK)
			return 0;
		return (DWORD)fileSize;
	}

	//------------------------------------------------------------------
	// CFileDataWriteStream 定义文档写数据流
	//------------------------------------------------------------------
	CFileDataWriteStream::CFileDataWriteStream(HANDLE file)
	{
		_File.Attach(file);
		_DefCacheBytes = DEFCACHEBYTES;

		_CacheBlock = (char*) ::malloc(_DefCacheBytes);
		_CachePosition = 0;
		_LastWritePosition = 0;
		_CacheLength = 0;
	}

	CFileDataWriteStream::~CFileDataWriteStream(void)
	{
		// 把未写入文件的数据写入文件
		FlushStream();

		_File.Detach();

		if (_CacheBlock)
		{
			::free(_CacheBlock);
		}
	}

	BOOL CFileDataWriteStream::FlushStream(void)
	{
		if (_CacheLength > 0)
		{
			DWORD writeLen = _CacheLength;
			DWORD byWrite = 0;
			_File.Write(_CacheBlock, writeLen, &byWrite);
			_CachePosition = 0;
			_CacheLength = 0;

			if (_File.GetSize(_LastWritePosition) != S_OK)
				return FALSE;

			return byWrite == writeLen;
		}

		return TRUE;
	}

	/** 写入数据到缓冲区 */
	BOOL CFileDataWriteStream::Write(LPCVOID pBuffer, DWORD nBufSize)
	{
		if (pBuffer == NULL || nBufSize == 0)
			return FALSE;

		DWORD	sumWriteBytes = 0;	// 已写入字节数
		LPBYTE	bBuffer = (LPBYTE)pBuffer;

		while (sumWriteBytes < nBufSize)
		{
			// 测试 Cache 中是否还有空间，如果没有则把数据写入
			if (_CachePosition >= _DefCacheBytes)
			{
				if (!FlushStream())
					return FALSE;
			}

			// 计算剩余要写入数据长度
			DWORD needWriteBytes = nBufSize - sumWriteBytes;

			// 计算 cache 中可写数据长度
			DWORD cacheBackBytes = _DefCacheBytes - _CachePosition;

			// 计算可以写入 cache 的数据长度，如果没有空间则可能出错了
			DWORD cacheWriteBytes = cacheBackBytes >= needWriteBytes ? needWriteBytes : cacheBackBytes;
			if (cacheWriteBytes == 0)
				break;

			// 把数据写入内存
			memcpy(&_CacheBlock[_CachePosition], bBuffer, cacheWriteBytes);
			_CachePosition += cacheWriteBytes;
			if (_CachePosition > _CacheLength)
				_CacheLength = _CachePosition;

			bBuffer += cacheWriteBytes;
			sumWriteBytes += (DWORD)cacheWriteBytes;
		}

		return sumWriteBytes == nBufSize;
	}

	/**
	* 取提当前指针位置
	* @return	返回指针位置
	*/
	DWORD CFileDataWriteStream::GetPosition()
	{
		return (DWORD)(_LastWritePosition + _CachePosition);
	}
	/**
	* 设置当前指针位置
	* @return	成功返回，失败返回假（如地址无效）
	*/
	BOOL CFileDataWriteStream::SetPosition(DWORD position)
	{
		// 判决否在缓存中，如果是则只是缓存指针移动，否则清除缓存
		if (position >= _LastWritePosition && position <= _LastWritePosition + _CacheLength)
		{
			_CachePosition = (DWORD)(position - _LastWritePosition);
		}
		else
		{
			// 先写当前缓存写入文件
			FlushStream();

			if (_File.Seek(position, FILE_BEGIN) != S_OK)
				return FALSE;

			_CachePosition = 0;
			_CacheLength = 0;
			_LastWritePosition = position;
		}

		return TRUE;
	}
	/**
	* 取得当前可访问数据长度
	* @return	返回数据长度
	*/
	DWORD CFileDataWriteStream::GetLength()
	{
		ULONGLONG fileSize = 0;
		if (_File.GetSize(fileSize) != S_OK)
			return 0;
		return (DWORD)fileSize;
	}
}