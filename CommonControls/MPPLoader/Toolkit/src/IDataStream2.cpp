#include "StdAfx.h"
#include "..\include\IDataStream2.h"

namespace BwProj {
	namespace toolkit {
		TCHAR emptyString = _T('\0');

		/************************************************************************/
		/* IDataReadStream                                                      */
		/************************************************************************/
		IDataReadStream::IDataReadStream()
		{
			_lastString = NULL;
			_lastStrLen = 0;
		}
		IDataReadStream::~IDataReadStream()
		{
			if (_lastString)
			{
				delete _lastString;
				_lastStrLen = 0;
			}
		}

		BYTE IDataReadStream::ReadByte(void)
		{
			BYTE val = 0;
			DWORD size = 0;
			Read(&val, sizeof(BYTE), size);
			return val;
		}

		long IDataReadStream::ReadLong(void)
		{
			long val = 0;
			DWORD size = 0;
			Read(&val, sizeof(long), size);
			return val;
		}

		__int64 IDataReadStream::ReadI64(void)
		{
			__int64 val = 0;
			DWORD size = 0;
			Read(&val, sizeof(__int64), size);
			return val;
		}

		LPCWSTR IDataReadStream::ReadString(void)
		{
			StringStreamData &stringData = ReadStringData();
			return (LPCWSTR)stringData.Buffer;
		}

		// 跳过几个字节，返回跳过的字节数
		size_t IDataReadStream::skip(size_t bytes)
		{
			if (bytes <= 0) return 0;

			char val[1024] = { 0 };
			DWORD size = 0;
			size_t total = 0;
			do
			{
				DWORD readSize = min(bytes - total, 1024);
				Read(val, readSize, size);
				total += size;
			} while (total < bytes && size);

			return total;
		}

		StringStreamData IDataReadStream::ReadStringData(void)
		{
			// 读字符串头
			StringStreamData stringData;
			DWORD size = 0;
			Read(&stringData, sizeof(STRING_STREAM_HEAD), size);

			if (stringData.nByteLength == 0)
			{
				stringData.Buffer = (LPBYTE)&emptyString;
				return stringData;
			}

			size_t needCacheBytes = stringData.nByteLength;

			// 读字符串
			if (_lastStrLen < needCacheBytes)
			{
				if (_lastString)
				{
					delete[] _lastString;
					_lastString = NULL;
				}
				_lastStrLen = needCacheBytes;
				_lastString = new BYTE[needCacheBytes];
			}

			Read((void*)_lastString, stringData.nByteLength, size);

			stringData.Buffer = _lastString;
			return stringData;
		}

		DWORD IRandomReadStream::available()
		{
			return GetLength() - GetPosition();
		}

		//LPCTSTR IDataReadStream::ReadString (void)
		//{
		//	// 读字符串头
		//	STRING_STREAM_HEAD head = { 0 };
		//	DWORD size = 0;
		//	Read (&head, sizeof(STRING_STREAM_HEAD), size);
		//
		//	if (head.nByteLength == 0)
		//		return NULL;
		//
		//	size_t needCacheBytes = head.bUnicode ? head.nByteLength : head.nByteLength * 2;
		//
		//	// 读字符串
		//	if (_lastStrLen < needCacheBytes)
		//	{
		//		if (_lastString)
		//		{
		//			delete[] _lastString;
		//			_lastString = NULL;
		//		}
		//		_lastStrLen = needCacheBytes;
		//		_lastString = new BYTE[needCacheBytes];
		//	}
		//
		//	Read ((void*) _lastString, head.nByteLength, size);
		//
		//	// 把单字节转换为双字节
		//	if (head.bUnicode == FALSE)
		//	{
		//		_tcscpy ((LPTSTR) _lastString, CA2W ((LPCSTR)_lastString));
		//	}
		//
		//	return (LPCTSTR)_lastString;
		//}

		/************************************************************************/
		/* IDataWriteStream                                                     */
		/************************************************************************/
		void IDataWriteStream::WriteLong(const long& val)
		{
			Write(&val, sizeof(long));
		}

		void IDataWriteStream::WriteI64(const __int64& val)
		{
			Write(&val, sizeof(__int64));
		}

		void IDataWriteStream::WriteString(LPCWSTR val)
		{
			// 写入字符串头
			STRING_STREAM_HEAD head = { 0 };

			head.bUnicode = TRUE;
			head.nByteLength = (USHORT)(wcslen(val) + 1) * sizeof(WCHAR);
			Write(&head, sizeof(STRING_STREAM_HEAD));
			Write(val, head.nByteLength);
		}

		void IDataWriteStream::WriteString(LPCSTR val)
		{
			// 写入字符串头
			STRING_STREAM_HEAD head = { 0 };

			head.bUnicode = FALSE;
			head.nByteLength = (USHORT)strlen(val) + 1;
			Write(&head, sizeof(STRING_STREAM_HEAD));
			Write(val, head.nByteLength);
		}

	}
}