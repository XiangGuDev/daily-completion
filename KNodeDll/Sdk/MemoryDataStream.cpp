#include "StdAfx.h"
#include "MemoryDataStream.h"

//#pragma warning(disable:4244)

namespace CommBase
{
	/************************************************************************/
	/* CBufferPtr                                                         */
	/************************************************************************/
	CBufferPtr::CBufferPtr()
	{
		_buffer = NULL;
		_size = 0;
	}

	CBufferPtr::CBufferPtr(LPBYTE buffer, DWORD size)
	{
		_buffer = buffer;
		_size = size;
	}

	CBufferPtr::CBufferPtr(CBufferPtr& ptr)
	{
		_size = ptr._size;
		_buffer = ptr.release();
	}

	CBufferPtr::~CBufferPtr()
	{
		reset();
	}

	CBufferPtr::operator void* ()
	{
		return get();
	}

	CBufferPtr::operator char* ()
	{
		return (char*)get();
	}

	CBufferPtr& CBufferPtr::operator = (CBufferPtr& src)
	{
		reset();
		_size = src._size;
		_buffer = src.release();
		return *this;
	}

	CBufferPtr CBufferPtr::alloc(size_t size)
	{
		return CBufferPtr((LPBYTE)::malloc(size), DWORD(size));
	}

	DWORD CBufferPtr::length()
	{
		return _size;
	}

	LPBYTE CBufferPtr::get()
	{
		return _buffer;
	}

	LPBYTE CBufferPtr::release()
	{
		LPBYTE temp = _buffer;
		_buffer = NULL;
		_size = 0;
		return temp;
	}

	void CBufferPtr::reset(LPBYTE buffer, size_t size)
	{
		ATLASSERT((buffer && size) || (!buffer && !size));

		if (_buffer)
			::free(_buffer);
		_buffer = buffer;
		_size = DWORD(size);
	}

	//------------------------------------------------------------------
	// CMemoryDataReadStream 文件映射资源数据读取对象
	//------------------------------------------------------------------
	CMemoryDataReadStream::CMemoryDataReadStream()
	{
		_CurrentDataPos = NULL;
		_DataBuffer = NULL;
		_CurrentDataPos = NULL;
		_DataBufferLength = 0;
	}

	CMemoryDataReadStream::CMemoryDataReadStream(const CString& strFile)
	{
		_CurrentDataPos = NULL;
		_DataBuffer = NULL;
		_CurrentDataPos = NULL;
		_DataBufferLength = 0;

		ReadFromFile(strFile);
	}

	CMemoryDataReadStream::CMemoryDataReadStream(LPVOID pBuffer, DWORD dwBufSize)
	{
		_CurrentDataPos = NULL;
		_DataBuffer = (LPBYTE)pBuffer;
		_CurrentDataPos = (LPBYTE)pBuffer;
		_DataBufferLength = dwBufSize;
	}

	CMemoryDataReadStream::CMemoryDataReadStream(CBufferPtr buffer)
	{
		DWORD length = buffer.length();
		_buffer_ptr.reset(buffer.release(), length);

		_CurrentDataPos = NULL;

		_DataBuffer = (LPBYTE)_buffer_ptr.get();
		_DataBufferLength = _buffer_ptr.length();
		_CurrentDataPos = (LPBYTE)_DataBuffer;
	}

	CMemoryDataReadStream::~CMemoryDataReadStream(void)
	{
		_buffer_ptr.reset();
	}

	/** 读取指定字节到缓冲区 */
	BOOL CMemoryDataReadStream::Read(LPVOID pBuffer, DWORD nBufSize, DWORD& nBytesRead)
	{
		ATLASSERT(pBuffer != NULL);
		if (pBuffer == NULL || nBufSize == 0)
			return FALSE;

		DWORD dwReadSize = 0;

		// 计算可拷贝数据长度
		dwReadSize = DWORD(min(_DataBuffer + _DataBufferLength, _CurrentDataPos + nBufSize) - _CurrentDataPos);

		// 拷贝数据
		memcpy(pBuffer, _CurrentDataPos, dwReadSize);

		nBytesRead = dwReadSize;

		_CurrentDataPos += dwReadSize;

		return TRUE;
	}

	/**
	* 取提当前指针位置
	* @return	返回指针位置
	*/
	DWORD CMemoryDataReadStream::GetPosition()
	{
		return DWORD(_CurrentDataPos - _DataBuffer);
	}

	/**
	* 设置当前指针位置
	* @return	成功返回，失败返回假（如地址无效）
	*/
	BOOL CMemoryDataReadStream::SetPosition(DWORD position)
	{
		if (position >= _DataBufferLength)
			return FALSE;

		_CurrentDataPos = _DataBuffer + position;
		return TRUE;
	}

	/**
	* 取得当前可访问数据长度
	* @return	返回数据长度
	*/
	DWORD CMemoryDataReadStream::GetLength()
	{
		return _DataBufferLength;
	}

	/** 取得原始缓存区 */
	LPVOID CMemoryDataReadStream::getOriginalBuffer()
	{
		return _DataBuffer;
	}

	BOOL CMemoryDataReadStream::WritToFile(const CString& strFile)
	{
		CAtlFile file;
		if (file.Create(strFile, GENERIC_WRITE, 0, CREATE_NEW) != S_OK)
			return FALSE;

		file.Write(getOriginalBuffer(), GetLength());
		file.Close();

		return TRUE;
	}

	BOOL CMemoryDataReadStream::ReadFromFile(const CString& strFile)
	{
		CAtlFile file;
		if (file.Create(strFile, GENERIC_READ, 0, OPEN_EXISTING) != S_OK)
			return FALSE;

		ULONGLONG nFileSize = 0;
		file.GetSize(nFileSize);
		_buffer_ptr = CBufferPtr::alloc((size_t)nFileSize);
		file.Read(_buffer_ptr.get(), (UINT)nFileSize);
		file.Close();

		_CurrentDataPos = NULL;

		_DataBuffer = (LPBYTE)_buffer_ptr.get();
		_DataBufferLength = _buffer_ptr.length();
		_CurrentDataPos = (LPBYTE)_DataBuffer;

		return TRUE;
	}

	//------------------------------------------------------------------
	// CMemoryDataWriteStream 文件映射资源数据存储对象
	//------------------------------------------------------------------
	CMemoryDataWriteStream::CMemoryDataWriteStream(const CString& strFile, int nBufSize)
	{
		_CurPosition = 0;
		_DefBufferBytes = nBufSize;
		ReadFromFile(strFile);
	}

	CMemoryDataWriteStream::CMemoryDataWriteStream(int nBufSize)
	{
		_CurPosition = 0;
		_DefBufferBytes = nBufSize;
	}

	CMemoryDataWriteStream::~CMemoryDataWriteStream(void)
	{
		ReleaseAllDataBuffer();
	}

	/**
	* 取提当前指针位置
	* @return	返回指针位置
	*/
	DWORD CMemoryDataWriteStream::GetPosition()
	{
		return _CurPosition;
	}

	/**
	* 设置当前指针位置
	* @return	成功返回，失败返回假（如地址无效）
	*/
	BOOL CMemoryDataWriteStream::SetPosition(DWORD position)
	{
		if (position >= GetLength())
			return FALSE;

		_CurPosition = position;
		return TRUE;
	}

	/**
	* 取得当前可访问数据长度
	* @return	返回数据长度
	*/
	DWORD CMemoryDataWriteStream::GetLength()
	{
		DWORD dwLength = 0;

		for (size_t i = 0; i < _DataBufferList.GetCount(); ++i)
		{
			dwLength += _DataBufferList.GetAt(i)->GetDataLength();
		}

		return dwLength;
	}

	BOOL CMemoryDataWriteStream::ReadFromFile(const CString& strFile)
	{
		CAtlFile file;
		if (file.Create(strFile, GENERIC_READ, 0, OPEN_EXISTING) != S_OK)
			return FALSE;

		LPBYTE lpBuf = new BYTE[_DefBufferBytes];
		do
		{
			DWORD nByteRead = 0;
			file.Read(lpBuf, _DefBufferBytes, nByteRead);
			if (0 == nByteRead)
				break;
			Write(lpBuf, nByteRead);
		} while (true);

		delete[] lpBuf;
		file.Close();

		return TRUE;
	}

	BOOL CMemoryDataWriteStream::WriteToFile(const CString& strFile)
	{
		CAtlFile file;
		if (S_OK != file.Create(strFile, GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS))
			return FALSE;

		// 循环写入缓存内存片
		size_t nDataPieceSize = _DataBufferList.GetCount();
		for (size_t i = 0; i < nDataPieceSize; ++i)
		{
			DATABUFFERINFO *pBufInfo = _DataBufferList.GetAt(i);
			ATLASSERT(pBufInfo != NULL);
			DWORD nBufferLength = pBufInfo->GetDataLength();
			file.Write(pBufInfo->GetBufferAddress(), nBufferLength);
		}
		file.Close();
		return TRUE;
	}

	/** 写入数据到缓冲区 */
	BOOL CMemoryDataWriteStream::Write(LPCVOID pBuffer, DWORD nBufSize)
	{
		DWORD spareSize = nBufSize;	// 剩下的字节数
		BYTE* byteBuffer = (BYTE*)pBuffer;

		// 先写入要覆盖的数据
		while (spareSize && GetLength() - _CurPosition > 0)
		{
			size_t bufferIndex = 0, byteIndex = 0;
			if (!GetDataBufferByPosition(_CurPosition, &bufferIndex, &byteIndex))
				break;

			DATABUFFERINFO *pDataBuf = _DataBufferList.GetAt(bufferIndex);
			DWORD writeSize = pDataBuf->WriteBuffer(byteBuffer, DWORD(byteIndex), spareSize);

			spareSize -= writeSize;
			byteBuffer += writeSize;
			_CurPosition += writeSize;
		}

		// 再写入新数据
		while (spareSize > 0)
		{
			DATABUFFERINFO *pDataBuf = GetNextDataBuffer(nBufSize);
			if (!pDataBuf)
				break;

			DWORD writeSize = pDataBuf->WriteBuffer(byteBuffer, spareSize);

			spareSize -= writeSize;
			byteBuffer += writeSize;
			_CurPosition += writeSize;
		}

		return spareSize == 0;
	}


	// 获取流内数据缓冲区地址
	DWORD CMemoryDataWriteStream::CopyBuffer(LPVOID output)
	{
		DWORD dwDataLength = GetLength();

		LPBYTE _BufferAddress = (LPBYTE)output;

		LPBYTE pCurrentPos = _BufferAddress;

		for (size_t i = 0; i < _DataBufferList.GetCount(); ++i)
		{
			DATABUFFERINFO *pBufInfo = _DataBufferList.GetAt(i);
			ATLASSERT(pBufInfo != NULL);

			DWORD dwData = pBufInfo->GetDataLength();
			memcpy(pCurrentPos, pBufInfo->GetBufferAddress(), dwData);

			pCurrentPos += dwData;
		}

		return dwDataLength;
	}

	// 获取下一个可用的缓冲区(返回当前可用的缓冲区，如果当前缓冲区已满则分配一个)
	CMemoryDataWriteStream::DATABUFFERINFO* CMemoryDataWriteStream::GetNextDataBuffer(DWORD dwNeedLength)
	{
		// 先判断最后一个是否有空间，如果有就先用最后一个，否则创建新的
		DATABUFFERINFO *pBufInfo = NULL;
		if (_DataBufferList.GetCount() != 0)
		{
			DATABUFFERINFO *pTempBufInfo = _DataBufferList.GetAt(_DataBufferList.GetCount() - 1);
			if (pTempBufInfo->GetBufferLength() - pTempBufInfo->GetDataLength() > 0)
			{
				pBufInfo = pTempBufInfo;
			}
		}

		if (pBufInfo == NULL)
		{
			pBufInfo = AllocDataBuffer();
			_DataBufferList.Add(pBufInfo);
		}

		return pBufInfo;
	}

	/** 取得指定位置所在的数据区 */
	BOOL CMemoryDataWriteStream::GetDataBufferByPosition(DWORD positon,
		size_t* bufferIndex, size_t* byteIndex)
	{
		size_t index1 = positon / _DefBufferBytes;
		size_t index2 = positon - index1 * _DefBufferBytes;

		if (index1 >= _DataBufferList.GetCount() || index2 >= _DefBufferBytes)
			return FALSE;

		*bufferIndex = index1;
		*byteIndex = index2;
		return TRUE;
	}

	// 分配新缓冲区
	CMemoryDataWriteStream::DATABUFFERINFO* CMemoryDataWriteStream::AllocDataBuffer(void)
	{
		LPVOID pBuffer = _DataBufferPool.Alloc(_DefBufferBytes, 2);	// 一次分配16K
		ATLASSERT(pBuffer != NULL);

		DATABUFFERINFO *pNewBufInfo = new DATABUFFERINFO(pBuffer, _DefBufferBytes);

		ATLASSERT(pNewBufInfo != NULL);
		return pNewBufInfo;
	}

	// 清除缓冲区块所占内存
	void CMemoryDataWriteStream::ReleaseAllDataBuffer(void)
	{
		for (size_t i = 0; i < _DataBufferList.GetCount(); ++i)
		{
			DATABUFFERINFO *pBufInfo = _DataBufferList.GetAt(i);

			_DataBufferPool.Free(pBufInfo->GetBufferAddress());

			delete pBufInfo;
		}

		_DataBufferList.RemoveAll();
	}


	CMemoryDataWriteStream::DATABUFFERINFO::DATABUFFERINFO(LPVOID pBuffer, DWORD nBufSize)
	{
		ATLASSERT(pBuffer != NULL);
		BufferAddress = (LPBYTE)pBuffer;
		BufferLength = nBufSize;

		BufferPos = 0;
	}

	/** 向数据缓冲区内写入数据并返回写入尺寸，如果缓冲区已满，则只写入能够写入的最大尺寸 */
	DWORD CMemoryDataWriteStream::DATABUFFERINFO::WriteBuffer(LPCVOID pBuffer, DWORD nBufSize)
	{
		return WriteBuffer(pBuffer, BufferPos, nBufSize);
	}

	/** 向数据缓冲区内写入数据并返回写入尺寸，如果缓冲区已满，则只写入能够写入的最大尺寸 */
	DWORD CMemoryDataWriteStream::DATABUFFERINFO::WriteBuffer(LPCVOID pBuffer, DWORD index, DWORD nBufSize)
	{
		ATLASSERT(BufferAddress != NULL && pBuffer != NULL);

		DWORD dwSize = nBufSize > BufferLength - index ? BufferLength - index : nBufSize;

		memcpy(BufferAddress + index, pBuffer, dwSize);

		// 更新已使用的最大数量
		index += dwSize;
		if (BufferPos < index)
			BufferPos = index;

		ATLASSERT(BufferPos <= BufferLength);
		return dwSize;
	}

	/** 获取有效数据尺寸 */
	DWORD CMemoryDataWriteStream::DATABUFFERINFO::GetDataLength(void)
	{
		return BufferPos;
	}

	/** 获取缓冲区大写 */
	DWORD CMemoryDataWriteStream::DATABUFFERINFO::GetBufferLength(void)
	{
		return BufferLength;
	}

	/** 获取缓冲区地址 */
	LPBYTE CMemoryDataWriteStream::DATABUFFERINFO::GetBufferAddress(void)
	{
		return BufferAddress;
	}
}