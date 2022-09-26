//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           FileDataStream.H
//    @author         fanHong
//    @date           2017/11/3 12:48
//    @brief          文件数据流
//
//*****************************************************

#pragma once

#include <atlfile.h>
#include "IDataStream.h"

namespace CommBase
{
	//! 定义文档读数据流
	class COMMBASE_API CFileDataReadStream : public IRandomReadStream
	{
	public:
		CFileDataReadStream(HANDLE file);
		~CFileDataReadStream(void);

		/**
		* 测试文件是否打开
		* @return	返回文件是否打开
		* - true 打开，false 关闭
		*/
		BOOL IsOpen(void)
		{
			return HANDLE() != NULL;
		}

		/**
		* 打开文件
		* @param [in] szFilename	文件名称
		* @return	返回打开文件是否成功
		* - true 打开成功， false 打开失败
		*/
		BOOL OpenFile(LPCTSTR szFilename);

		/**
		* 关闭文件
		*/
		void CloseFile(void);
		operator HANDLE() const;

		/**
		* 读取指定字节到缓冲区
		* @param [out] pBuffer		缓冲数据
		* @param [in] nBufSize		缓冲区大小
		* @param [out] nBytesRead	字节数
		* @return	返回TRUE
		*/
		virtual BOOL Read(LPVOID pBuffer, DWORD nBufSize, DWORD& nBytesRead);

		/**
		* 取提当前指针位置
		* @return	返回指针位置
		*/
		virtual DWORD GetPosition();
		/**
		* 设置当前指针位置
		* 文件读取流的位置要在 0到GetLength()范围内
		* @return	成功返回，失败返回假（如地址无效）
		*/
		virtual BOOL SetPosition(DWORD position);
		/**
		* 取得当前可访问数据长度
		* @return	返回数据长度
		*/
		virtual DWORD GetLength();

	protected:
		// 从 Cache 中读数据
		void ReadBuffer(LPVOID pBuffer, DWORD nBufSize, DWORD& nBytesRead);

		/** 关联的文件 */
		CAtlFile	_File;

		/** 默认需分配的 Cache 大小 */
		DWORD		_DefCacheBytes;

		/** 文件 Cache，默认为 DEFAULT_STREAM_BLOCKSIZE 个字节 */
		char*		_CacheBlock;

		/** 当前 Cache 读位置 */
		DWORD		_CachePosition;

		/** 当前 Cache 中可读数据的长度 */
		DWORD		_CacheReadBytes;

		/** cache在文件中的位置 */
		ULONGLONG	_CacheFilePosition;
	};


	//! 定义文档写数据流
	class COMMBASE_API CFileDataWriteStream : public IRandomWriteStream
	{
	public:
		CFileDataWriteStream(HANDLE file);
		~CFileDataWriteStream(void);

		/**
		* 写入数据到缓冲区
		* @param [in] pBuffer		缓冲数据
		* @param [in] nBufSize		缓冲区大小
		*/
		virtual BOOL Write(LPCVOID pBuffer, DWORD nBufSize);

		/** 将数据写入文件，清空数据流 */
		BOOL FlushStream(void);

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
	protected:

		CAtlFile	_File;

		/** 文件 Cache，默认为 DEFAULT_STREAM_BLOCKSIZE 个字节 */
		char*		_CacheBlock;

		/** 当前 Cache 读位置 */
		DWORD		_CachePosition;

		/** 已写入的cache长度 */
		DWORD		_CacheLength;

		/** 默认需分配的 Cache 大小 */
		DWORD		_DefCacheBytes;

		/** 最后一次写的位置 */
		ULONGLONG	_LastWritePosition;
	};
}