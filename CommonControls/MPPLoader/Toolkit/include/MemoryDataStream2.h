#pragma once

#include "IDataStream2.h"
#include "allocmemorypool2.h"

// begin namespace
namespace BwProj{ namespace toolkit{

/** @addtogroup DataStream
* @{
*/

//!C风格的内存块指能指针，可自动释放，要求必须是 malloc 分配的
class COMMONCONTROLS_API c_buffer_ptr
{
public:
	c_buffer_ptr ();
	c_buffer_ptr (LPBYTE buffer, DWORD size);
	c_buffer_ptr (c_buffer_ptr& ptr);
	/*virtual*/ ~c_buffer_ptr ();

	operator void* ();
	operator char* ();
	c_buffer_ptr& operator = (c_buffer_ptr& src);

	static c_buffer_ptr alloc (size_t size);
	DWORD length (void);
	LPBYTE get (void);
	LPBYTE release (void);
	void reset (LPBYTE buffer = NULL, size_t size = 0);

protected:
	LPBYTE	_buffer;	/**< 缓存数据 */
	DWORD	_size;		/**< 缓存长度 */
};

//=====================================================================
// CMemoryDataReadStream
//=====================================================================
//! 内存数据读取对象
class  CMemoryDataReadStream : public BwProj::toolkit::IRandomReadStream
{
public:
	CMemoryDataReadStream(LPVOID pBuffer, DWORD dwBufSize);
	CMemoryDataReadStream(c_buffer_ptr buffer);
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
	virtual DWORD GetPosition ();
	/**
	* 设置当前指针位置
	* @return	成功返回，失败返回假（如地址无效）
	*/
	virtual BOOL SetPosition (DWORD position);
	/**
	* 取得当前可访问数据长度
	* @return	返回数据长度
	*/
	virtual DWORD GetLength ();

	virtual void reset();

	/** 取得原始缓存区 */
	LPVOID getOriginalBuffer (void);

protected:
	BwProj::toolkit::CAllocMemoryPool	_StringPool;

	/** 由 _StringPool 分配的空间，用于保存临时字符串 */
	LPCTSTR				_StringBuffer;

	/** 数据起始位置 */
	LPBYTE				_DataBuffer;
	c_buffer_ptr		_buffer_ptr;

	/** 数据缓冲区长度 */
	DWORD				_DataBufferLength;

	/** 当前读取位置 */
	LPBYTE				_CurrentDataPos;
};

//=====================================================================
// CMemoryDataWriteStream2
//=====================================================================
//! 文件映射资源数据存储对象
class CMemoryDataWriteStream2 : public BwProj::toolkit::IRandomWriteStream
{
public:
	CMemoryDataWriteStream2(void);
	virtual ~CMemoryDataWriteStream2(void);

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
	virtual DWORD GetPosition ();
	/**
	* 设置当前指针位置
	* @return	成功返回，失败返回假（如地址无效）
	*/
	virtual BOOL SetPosition (DWORD position);
	/**
	* 取得当前可访问数据长度
	* @return	返回数据长度
	*/
	virtual DWORD GetLength ();

protected:
	class DATABUFFERINFO
	{
	public:
		DATABUFFERINFO(LPVOID pBuffer, DWORD nBufSize);

		/** 向数据缓冲区内写入数据并返回写入尺寸，如果缓冲区已满，则只写入能够写入的最大尺寸 */
		DWORD WriteBuffer (LPCVOID pBuffer, DWORD nBufSize);
		DWORD WriteBuffer (LPCVOID pBuffer, DWORD index, DWORD nBufSize);

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
	BOOL GetDataBufferByPosition (DWORD positon, size_t* bufferIndex, size_t* byteIndex);

	/** 分配新缓冲区 */
	DATABUFFERINFO* AllocDataBuffer(void);

	/** 清除缓冲区块所占内存 */
	void ReleaseAllDataBuffer(void);

protected:
	BwProj::toolkit::CAllocMemoryPool			_DataBufferPool;
	/** 默认需分配的 Cache 大小 */
	DWORD						_DefBufferBytes;
	/** 用于存储数据的缓冲区列表 */
	CAtlArray<DATABUFFERINFO*>	_DataBufferList;

	/** 当前指针位置 */
	DWORD						_CurPosition;
};

/** * @} */
// end namespace
}}