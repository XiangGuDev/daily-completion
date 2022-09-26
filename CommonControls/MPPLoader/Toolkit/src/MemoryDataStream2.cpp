#include "StdAfx.h"
#include "..\include\MemoryDataStream2.h"

namespace BwProj {
	namespace toolkit {
		/************************************************************************/
		/* c_buffer_ptr                                                         */
		/************************************************************************/
		c_buffer_ptr::c_buffer_ptr()
		{
			_buffer = NULL;
			_size = 0;
		}

		c_buffer_ptr::c_buffer_ptr(LPBYTE buffer, DWORD size)
		{
			_buffer = buffer;
			_size = size;
		}

		c_buffer_ptr::c_buffer_ptr(c_buffer_ptr& ptr)
		{
			_size = ptr._size;
			_buffer = ptr.release();
		}

		c_buffer_ptr::~c_buffer_ptr()
		{
			reset();
		}

		c_buffer_ptr::operator void* ()
		{
			return get();
		}

		c_buffer_ptr::operator char* ()
		{
			return (char*)get();
		}

		c_buffer_ptr& c_buffer_ptr::operator = (c_buffer_ptr& src)
		{
			_size = src._size;
			_buffer = src.release();
			return *this;
		}

		c_buffer_ptr c_buffer_ptr::alloc(size_t size)
		{
			return c_buffer_ptr((LPBYTE)::malloc(size), size);
		}

		DWORD c_buffer_ptr::length(void)
		{
			return _size;
		}

		LPBYTE c_buffer_ptr::get(void)
		{
			return _buffer;
		}

		LPBYTE c_buffer_ptr::release(void)
		{
			LPBYTE temp = _buffer;
			_buffer = NULL;
			_size = 0;
			return temp;
		}

		void c_buffer_ptr::reset(LPBYTE buffer, size_t size)
		{
			ATLASSERT((buffer && size) || (!buffer && !size));

			if (_buffer)
				::free(_buffer);
			_buffer = buffer;
			_size = size;
		}

		//------------------------------------------------------------------
		// CMemoryDataReadStream 文件映射资源数据读取对象
		//------------------------------------------------------------------
		CMemoryDataReadStream::CMemoryDataReadStream(LPVOID pBuffer, DWORD dwBufSize)
		{
			_CurrentDataPos = NULL;
			_StringBuffer = NULL;

			_DataBuffer = (LPBYTE)pBuffer;
			_CurrentDataPos = (LPBYTE)pBuffer;
			_DataBufferLength = dwBufSize;
		}

		CMemoryDataReadStream::CMemoryDataReadStream(c_buffer_ptr buffer)
		{
			DWORD length = buffer.length();
			_buffer_ptr.reset(buffer.release(), length);

			_CurrentDataPos = NULL;
			_StringBuffer = NULL;

			_DataBuffer = (LPBYTE)_buffer_ptr.get();
			_DataBufferLength = _buffer_ptr.length();
			_CurrentDataPos = (LPBYTE)_DataBuffer;
		}

		CMemoryDataReadStream::~CMemoryDataReadStream(void)
		{
			// 释放上一次分配的字符串
			if (_StringBuffer)
			{
				_StringPool.Free((void*)_StringBuffer);
				_StringBuffer = NULL;
			}

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
			dwReadSize = min(_DataBuffer + _DataBufferLength, _CurrentDataPos + nBufSize) - _CurrentDataPos;

			// 拷贝数据
			memcpy(pBuffer, _CurrentDataPos, dwReadSize);

			nBytesRead = dwReadSize;

			_CurrentDataPos += dwReadSize;

			return nBytesRead > 0;
		}

		/**
		* 取提当前指针位置
		* @return	返回指针位置
		*/
		DWORD CMemoryDataReadStream::GetPosition()
		{
			return _CurrentDataPos - _DataBuffer;
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

		void CMemoryDataReadStream::reset()
		{
			SetPosition(0);
		}

		/** 取得原始缓存区 */
		LPVOID CMemoryDataReadStream::getOriginalBuffer(void)
		{
			return _DataBuffer;
		}

		//------------------------------------------------------------------
		// CMemoryDataWriteStream2 文件映射资源数据存储对象
		//------------------------------------------------------------------
		CMemoryDataWriteStream2::CMemoryDataWriteStream2(void)
		{
			_DefBufferBytes = 8 * 1024;// 8K
			_CurPosition = 0;
		}

		CMemoryDataWriteStream2::~CMemoryDataWriteStream2(void)
		{
			ReleaseAllDataBuffer();
		}

		/**
		* 取提当前指针位置
		* @return	返回指针位置
		*/
		DWORD CMemoryDataWriteStream2::GetPosition()
		{
			return _CurPosition;
		}

		/**
		* 设置当前指针位置
		* @return	成功返回，失败返回假（如地址无效）
		*/
		BOOL CMemoryDataWriteStream2::SetPosition(DWORD position)
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
		DWORD CMemoryDataWriteStream2::GetLength()
		{
			DWORD dwLength = 0;

			for (size_t i = 0; i < _DataBufferList.GetCount(); ++i)
			{
				dwLength += _DataBufferList.GetAt(i)->GetDataLength();
			}

			return dwLength;
		}

		/** 写入数据到缓冲区 */
		BOOL CMemoryDataWriteStream2::Write(LPCVOID pBuffer, DWORD nBufSize)
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
				DWORD writeSize = pDataBuf->WriteBuffer(byteBuffer, byteIndex, spareSize);

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
		DWORD CMemoryDataWriteStream2::CopyBuffer(LPVOID output)
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
		CMemoryDataWriteStream2::DATABUFFERINFO* CMemoryDataWriteStream2::GetNextDataBuffer(DWORD dwNeedLength)
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
		BOOL CMemoryDataWriteStream2::GetDataBufferByPosition(DWORD positon,
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
		CMemoryDataWriteStream2::DATABUFFERINFO* CMemoryDataWriteStream2::AllocDataBuffer(void)
		{
			LPVOID pBuffer = _DataBufferPool.Alloc(_DefBufferBytes, 2);	// 一次分配16K
			ATLASSERT(pBuffer != NULL);

			DATABUFFERINFO *pNewBufInfo = new DATABUFFERINFO(pBuffer, _DefBufferBytes);

			ATLASSERT(pNewBufInfo != NULL);
			return pNewBufInfo;
		}

		// 清除缓冲区块所占内存
		void CMemoryDataWriteStream2::ReleaseAllDataBuffer(void)
		{
			for (size_t i = 0; i < _DataBufferList.GetCount(); ++i)
			{
				DATABUFFERINFO *pBufInfo = _DataBufferList.GetAt(i);

				_DataBufferPool.Free(pBufInfo->GetBufferAddress());

				delete pBufInfo;
			}

			_DataBufferList.RemoveAll();
		}


		CMemoryDataWriteStream2::DATABUFFERINFO::DATABUFFERINFO(LPVOID pBuffer, DWORD nBufSize)
		{
			ATLASSERT(pBuffer != NULL);
			BufferAddress = (LPBYTE)pBuffer;
			BufferLength = nBufSize;

			BufferPos = 0;
		}

		/** 向数据缓冲区内写入数据并返回写入尺寸，如果缓冲区已满，则只写入能够写入的最大尺寸 */
		DWORD CMemoryDataWriteStream2::DATABUFFERINFO::WriteBuffer(LPCVOID pBuffer, DWORD nBufSize)
		{
			return WriteBuffer(pBuffer, BufferPos, nBufSize);
		}

		/** 向数据缓冲区内写入数据并返回写入尺寸，如果缓冲区已满，则只写入能够写入的最大尺寸 */
		DWORD CMemoryDataWriteStream2::DATABUFFERINFO::WriteBuffer(LPCVOID pBuffer, DWORD index, DWORD nBufSize)
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
		DWORD CMemoryDataWriteStream2::DATABUFFERINFO::GetDataLength(void)
		{
			return BufferPos;
		}

		/** 获取缓冲区大写 */
		DWORD CMemoryDataWriteStream2::DATABUFFERINFO::GetBufferLength(void)
		{
			return BufferLength;
		}

		/** 获取缓冲区地址 */
		LPBYTE CMemoryDataWriteStream2::DATABUFFERINFO::GetBufferAddress(void)
		{
			return BufferAddress;
		}
	}
}