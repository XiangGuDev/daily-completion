
#pragma once

namespace BwProj { namespace toolkit {

/** @addtogroup DataStream
* @{
*/

//=====================================================================
// IByteReadStream
//=====================================================================
//! 用于数据读取的流的基类
class IByteReadStream
{
public:
	virtual ~IByteReadStream () {}

	/** 
	* 读取指定字节到缓冲区 （纯虚函数）
	* @param [in] pBuffer		缓冲数据
	* @param [in] nBufSize		缓冲区大小
	* @param [out] nBytesRead	字节数
	* @return	返回TRUE
	*/
	virtual BOOL Read(LPVOID pBuffer, DWORD nBufSize, DWORD& nBytesRead) = 0;
};

typedef std::auto_ptr<IByteReadStream> ByteReadStream_ptr;

//=====================================================================
// IByteWriteStream
//=====================================================================
//! 用于数据写入的流的基类
class  IByteWriteStream
{
public:
	virtual ~IByteWriteStream () {}

	/** 
	* 写入数据到缓冲区 （纯虚函数）
	* @param [in] pBuffer	缓冲数据
	* @param [in] nBufSize	缓冲区大小
	* @return	返回TRUE
	*/
	virtual BOOL Write(const void* pBuffer, DWORD nBufSize)  = 0;
};

typedef std::auto_ptr<IByteWriteStream> ByteWriteStream_ptr;

//=====================================================================
// STRING_STREAM_HEAD
//=====================================================================
//! 定义字符串文本流头结构（4字节）
struct STRING_STREAM_HEAD
{
	USHORT	bUnicode;		/**< 是否是双字节字符串（TRUE, FALSE）*/
	USHORT	nByteLength;	/**< 字符串长度（包括结束符0）*/
};

struct StringStreamData : public STRING_STREAM_HEAD
{
	LPBYTE	Buffer;		// 字符串地址
};

//=====================================================================
// IDataReadStream
//=====================================================================
//! 数据流读取器
class COMMONCONTROLS_API IDataReadStream : public IByteReadStream
{
public:
	IDataReadStream (void);
	virtual ~IDataReadStream (void);

	virtual BYTE ReadByte(void);
	
	/**
	* 读取long格式数值
	* @return	返回long格式数值
	*/
	virtual long ReadLong (void);

	/**
	* 读取__int64格式数值
	* @return	返回__int64格式数值
	*/
	virtual __int64 ReadI64 (void);

	/**
	* 读取LPCWSTR格式字符串
	* @return	返回LPCTSTR格式字符串
	*/
	virtual LPCWSTR ReadString (void);

	// 跳过几个字节，返回跳过的字节数
	virtual size_t skip(size_t bytes);

	// 复原位置
	virtual void reset() = 0;

	virtual DWORD available() = 0;

	/** 
	* 取得数据
	* @return	返回数据
	*/
	virtual StringStreamData ReadStringData (void);

protected:
	BYTE*	_lastString;
	size_t	_lastStrLen;
};

typedef std::auto_ptr<IDataReadStream> DataReadStream_ptr;
typedef std::shared_ptr<IDataReadStream> DataReadStreamPtr;

//! 随即数据读取流
class IRandomReadStream : public IDataReadStream
{
public:
	/**
	* 取提当前指针位置
	* @return	返回指针位置
	*/
	virtual DWORD GetPosition () = 0;
	/**
	* 设置当前指针位置
	* @return	成功返回，失败返回假（如地址无效）
	*/
	virtual BOOL SetPosition (DWORD position) = 0;
	/**
	* 取得当前可访问数据长度
	* @return	返回数据长度
	*/
	virtual DWORD GetLength () = 0;

	virtual DWORD available();
};

typedef std::auto_ptr<IRandomReadStream> RandomReadStream_ptr;

//=====================================================================
// IDataWriteStream
//=====================================================================
//! 数据流存储器
class COMMONCONTROLS_API IDataWriteStream : public IByteWriteStream
{
public:

	/**
	* 读取LPCTSTR格式字符串
	* @param [out] val	要写入的long格式的数值
	*/
	virtual void WriteLong (const long& val);

	/**
	* 读取LPCTSTR格式字符串
	* @param [out] val	要写入的__int64格式的数值
	*/
	virtual void WriteI64 (const __int64& val);

	/**
	* 读取LPCWSTR格式字符串
	* @param [in] val	要写入的LPCWSTR格式的字符串
	*/
	virtual void WriteString (LPCWSTR val);

	/** 
	* 写数据
	* @param [in] val	写数据
	*/
	virtual void WriteString (LPCSTR val);

};

typedef std::auto_ptr<IDataWriteStream> DataWriteStream_ptr;

//! 随即数据写流
class  COMMONCONTROLS_API IRandomWriteStream : public IDataWriteStream
{
public:
	/**
	* 取提当前指针位置
	* @return	返回指针位置
	*/
	virtual DWORD GetPosition () = 0;
	/**
	* 设置当前指针位置
	* @return	成功返回，失败返回假（如地址无效）
	*/
	virtual BOOL SetPosition (DWORD position) = 0;
	/**
	* 取得当前可访问数据长度
	* @return	返回数据长度
	*/
	virtual DWORD GetLength () = 0;
};

typedef std::auto_ptr<IRandomWriteStream> RandomWriteStream_ptr;

/** * @} */
}}	// end namespace