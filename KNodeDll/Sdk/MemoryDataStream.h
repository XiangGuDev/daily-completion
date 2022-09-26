//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           MemoryDataStream.H
//    @author         fanHong
//    @date           2017/11/3 11:26
//    @brief          内存数据流操作
//
//*****************************************************

#pragma once

#include <vector>
#include "IDataStream.h"
#include "AllocMemoryPool.h"

namespace CommBase
{
	//!C风格的内存块指能指针，可自动释放，要求必须是 malloc 分配的
	class COMMBASE_API CBufferPtr
	{
	public:
		CBufferPtr();
		CBufferPtr(LPBYTE buffer, DWORD size);
		CBufferPtr(CBufferPtr& ptr);
		/*virtual*/ ~CBufferPtr();

		operator void* ();
		operator char* ();
		CBufferPtr& operator = (CBufferPtr& src);

		static CBufferPtr alloc(size_t size);
		DWORD length(void);
		LPBYTE get(void);
		LPBYTE release(void);
		void reset(LPBYTE buffer = NULL, size_t size = 0);

	protected:
		LPBYTE	_buffer;	/**< 缓存数据 */
		DWORD	_size;		/**< 缓存长度 */
	};

	//=====================================================================
	// CMemoryDataReadStream
	//=====================================================================
	//! 内存数据读取对象
	class COMMBASE_API CMemoryDataReadStream : public IRandomReadStream
	{
	public:
		CMemoryDataReadStream();
		CMemoryDataReadStream(const CString& strFile);
		CMemoryDataReadStream(LPVOID pBuffer, DWORD dwBufSize);
		CMemoryDataReadStream(CBufferPtr buffer);
		~CMemoryDataReadStream(void);

		/**
		* 读取指定字节到缓冲区
		* @param [in] pBuffer		缓冲数据
		* @param [in] nBufSize		缓冲区大小
		* @param [out] nBytesRead	字节数
		* @return 返回读取指定字节到缓冲区是否成功
		* - TRUE 成功， FALSE 失败
		*/
		virtual BOOL Read(LPVOID pBuffer, DWORD nBufSize, DWORD& nBytesRead);

		/**
		* 取提当前指针位置
		* @return	返回指针位置
		*/
		virtual DWORD GetPosition();
		/**
		* 设置当前指针位置
		* @return	成功返回，失败返回假（如地址无效）
		*/
		virtual BOOL SetPosition(DWORD position);
		/**
		* 取得当前可访问数据长度
		* @return	返回数据长度
		*/
		virtual DWORD GetLength();

		/** 取得原始缓存区 */
		LPVOID getOriginalBuffer(void);

		/**
		* 从文件中读取数据
		* @return	成功返回TRUE
		*/
		virtual BOOL ReadFromFile(const CString& strFile);

		/**
		* 写入到文件中
		* @return	成功返回TRUE
		*/
		virtual BOOL WritToFile(const CString& strFile);

	protected:
		/** 数据起始位置 */
		LPBYTE				_DataBuffer;
		CBufferPtr			_buffer_ptr;

		/** 数据缓冲区长度 */
		DWORD				_DataBufferLength;

		/** 当前读取位置 */
		LPBYTE				_CurrentDataPos;
	};

	//=====================================================================
	// CMemoryDataWriteStream
	//=====================================================================
	//! 文件映射资源数据存储对象
	class COMMBASE_API CMemoryDataWriteStream : public IRandomWriteStream
	{
	public:
		CMemoryDataWriteStream(const CString& strFile, int nBufSize = 0x2000);
		CMemoryDataWriteStream(int nBufSize = 0x2000 /*8K*/);
		~CMemoryDataWriteStream(void);

		/**
		* 获取流内数据缓冲区地址
		*/
		DWORD CopyBuffer(LPVOID output);

		/**
		* 写入数据到缓冲区
		* @param [in] pBuffer		字节字符串
		* @param [in] nBufSize		字节长度
		* @return 返回写入数据到缓冲区是否成功
		* - TRUE 成功， FALSE 失败
		*/
		virtual BOOL Write(LPCVOID pBuffer, DWORD nBufSize);

		/**
		* 取提当前指针位置
		* @return	返回指针位置
		*/
		virtual DWORD GetPosition();
		/**
		* 设置当前指针位置
		* @return	成功返回，失败返回假（如地址无效）
		*/
		virtual BOOL SetPosition(DWORD position);
		/**
		* 取得当前可访问数据长度
		* @return	返回数据长度
		*/
		virtual DWORD GetLength();

		/**
		* 从文件中读取数据
		* @return	成功返回TRUE
		*/
		virtual BOOL ReadFromFile(const CString& strFile);

		/**
		* 把数据写入到文件中
		* @return	成功返回TRUE
		*/
		virtual BOOL WriteToFile(const CString& strFile);

	protected:
		class DATABUFFERINFO
		{
		public:
			DATABUFFERINFO(LPVOID pBuffer, DWORD nBufSize);

			/** 向数据缓冲区内写入数据并返回写入尺寸，如果缓冲区已满，则只写入能够写入的最大尺寸 */
			DWORD WriteBuffer(LPCVOID pBuffer, DWORD nBufSize);
			DWORD WriteBuffer(LPCVOID pBuffer, DWORD index, DWORD nBufSize);

			/** 获取有效数据尺寸 */
			DWORD GetDataLength(void);

			/** 获取缓冲区大写 */
			DWORD GetBufferLength(void);

			/** 获取缓冲区地址 */
			LPBYTE GetBufferAddress(void);

		protected:
			/** 数据缓冲区地址 */
			LPBYTE				BufferAddress;

			/** 缓冲区尺寸 */
			DWORD				BufferLength;

			/** 当前缓冲区写入位置 */
			DWORD				BufferPos;
		};

		/** 获取下一个能够容纳dwNeedLength长度的缓冲区(返回当前可用的缓冲区，如果当前缓冲区已满则分配一个) */
		DATABUFFERINFO* GetNextDataBuffer(DWORD dwNeedLength);

		/** 取得指定位置所在的数据区 */
		BOOL GetDataBufferByPosition(DWORD positon, size_t* bufferIndex, size_t* byteIndex);

		/** 分配新缓冲区 */
		DATABUFFERINFO* AllocDataBuffer(void);

		/** 清除缓冲区块所占内存 */
		void ReleaseAllDataBuffer(void);

	protected:
		CAllocMemoryPool			_DataBufferPool;
		/** 默认需分配的 Cache 大小 */
		DWORD						_DefBufferBytes;
		/** 用于存储数据的缓冲区列表 */
		CAtlArray<DATABUFFERINFO*>	_DataBufferList;

		/** 当前指针位置 */
		DWORD						_CurPosition;
	};

	/*工程ZIP数据读取内存结构*/
	typedef struct _tag_FileMReadData
	{
		_tag_FileMReadData()
		{
			pFileDataStream = NULL;
		}

		CString strFileName; // 文件名称
		CMemoryDataReadStream* pFileDataStream; // 文件数据
	}FileMReadData;
	typedef std::vector<FileMReadData> FileMReadDataArray;

	/*工程ZIP数据写入内存结构*/
	typedef struct _tag_FileMWriteData
	{
		_tag_FileMWriteData()
		{
			pFileDataStream = NULL;
		}

		CString strFileName; // 文件名称
		CMemoryDataWriteStream* pFileDataStream; // 文件数据
	}FileMWriteData;
	typedef std::vector<FileMWriteData> FileMWriteDataArray;

}