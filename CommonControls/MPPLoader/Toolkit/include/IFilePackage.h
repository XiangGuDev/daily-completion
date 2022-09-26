
#pragma once

#include "IDataStream.h"
#include "CriticalSection.h"
#include "Exception.h"

namespace BwProj { namespace toolkit {

/** @addtogroup FilePackage
* @{
*/

/** 文件信息 */
typedef struct
{
	TCHAR	name[MAX_PATH];		/**< 文件名称 */
	DWORD	originalSize;		/**< 未压缩文件长度 */
	DWORD	compressedSize;		/**< 压缩后文件长度 */
	DWORD	extendType;			/**< 扩展值类型，用于存储实现类 */
	void*	extendData;			/**< 扩展值数据，用于存储实现类的数据 */
}FILEENTRY, *LPFILEENTRY;
typedef const LPFILEENTRY LPCFILEENTRY;

/** 以下定义用于FILEENTRY.extendType  */
#define ENTRY_EXTEND_EXISTENTRY	0x01	/**< 文件是已存在在的 */
#define ENTRY_EXTEND_NEWSTREAM	0x02	/**< 文件是新添加的的 */

//! EntryEnumeration 文件包内文件的遍历器
class IEntryEnumeration
{
public:
	virtual ~IEntryEnumeration () {}
	virtual BOOL hasMoreElements (void) = 0;
	virtual LPCFILEENTRY nextElement (void) = 0;
};
typedef std::auto_ptr<IEntryEnumeration> EntryEnumeration_ptr;

//! IFilePackage 文件包管理接口，用于把多个文件放在一个文件中管理
class IFilePackage
{
public:
	virtual ~IFilePackage ();

	/**
	* 设置、读取加密用的密码
	*/
	virtual void setPassword (LPCTSTR password);
	virtual LPCTSTR getPassword (void);

	/** 
	* 以可读取模式打开文件
	* @param [in] filePath	文件路径
	*/
	virtual void open (LPCTSTR filePath);
	virtual void open (HANDLE fileHandle);

	/** 
	* 以只读取模式打开文件
	* @param [in] fileStream	文件流
	*/
	virtual void open (IRandomReadStream* fileStream);

	/** 
	* 关闭文件，会放弃调用submit后的所有操作
	*/
	virtual void close (void);

	/**
	* 取得文件枚举对象
	* @return	返回文件枚举
	*/
	virtual EntryEnumeration_ptr getEntries (void) = 0;

	/**
	* 取得包内文件信息（不区分大小写）
	* @param [in] name	文件名称
	*/
	virtual LPCFILEENTRY getEntry (LPCTSTR name) = 0;

	/**
	* 取得文件内容(注意外部生成的ZIP包内可能存在目录，当entry为目录时此方法返回RandomReadStream_ptr内部为空，
	* 请使用.get方法判断返回的读取流对象是否有效)
	* @param [in] entry		文件索引
	* @return 返回文件流，流用完后要自己删除
	*/
	virtual RandomReadStream_ptr getReadStream (LPCFILEENTRY entry) = 0;

	/**
	* 添加文件到包中 
	* @param [in] name		文件名称
	* @param [in] stream	数据流读取对象
	*/
	virtual void putNextEntry (LPCTSTR name, RandomReadStream_ptr stream) = 0;

	/**
	* 删除包中的文件 
	* @param [in] entry		文件索引
	*/
	virtual void removeEntry (LPCFILEENTRY entry) = 0;

	/**
	* 更新物理文件
	* -文件被添加或删除后，需调用 Flush 更新到物理文件，Close时将自动调用此函数
	* -当文件比较多或大时，Flush的时间可能会比较长
	* @param [in] entry		文件名，为空时使用内部保存的文件名（来自通过文件名打开的包）
	* @result 等待保存线程退出，失败抛出 BwProj::Exception 异常
	*/
	virtual void submit (CString filename);

protected:
	/** 文件信息 */
	typedef struct
	{
		HANDLE				fileHandle; /**< 打开的文件句柄，允许为空 */
		CString				filename;	/**< 打开的文件名，允许为空 */
		IRandomReadStream*	readStream;	/**< 文件数据读取流 */
		IRandomWriteStream*	writeStream;/**< 文件数据写入流 */
		BOOL	closeDeleteReadStream;	/**< 退出时是否删除readStream */
		BOOL	closeDeleteWriteStream;	/**< 退出时是否删除writeStream */
	}OPENCONTEXT;

	/**
	* 更新当前文件上下文，当文件打开后会调用
	* @param [in] context	已创建并设置好的上下文
	*/
	virtual void _updateContext (OPENCONTEXT* context);

	/** 尝试调用_updateContext，如果有异常则会释放context数据 */
	BOOL _tryUpdateContext (OPENCONTEXT* context, CString& errmsg);

	virtual void _submit(CString filePath) = 0;

protected:
	std::auto_ptr<OPENCONTEXT>	_openContext;	/**< 文件打开信息 */
	CString						_filePassword;	/**< 文件打开密码 */
	BwProj::toolkit::CCriticalSectionHandle	_CSC;
};

//! CEntryEnumeration 文件包内文件的遍历器
class CEntryEnumeration : public IEntryEnumeration
{
public:
	CEntryEnumeration ();
	virtual ~CEntryEnumeration ();
	virtual BOOL hasMoreElements (void);
	virtual LPCFILEENTRY nextElement (void);

	CAtlArray<LPFILEENTRY>& getEntryList (void);

protected:
	size_t					_position;	/**< 当前索引位置 */
	CAtlArray<LPFILEENTRY>	_entryList;	/**< 结点列表 */
};

/** * @} */
}}	// end namespace
/** @} */