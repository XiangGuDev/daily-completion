#pragma once

#include "IFilePackage.h"

namespace BwProj { namespace toolkit {

/** @addtogroup FilePackage
* @{
*/

/************************************************************************/
/*  CTarFilePackage                                                     */
/************************************************************************/
#define TARFILE_FLAGS_ENCRYPTTABLE	0x0001	/**< 加密文件数据表 */
#define TARFILE_FLAGS_ENCRYPTDATA	0x0002	/**< 加密数据文件 */
#define TARFILE_FLAGS_ENCRYPT		(TARFILE_FLAGS_ENCRYPTTABLE|TARFILE_FLAGS_ENCRYPTDATA)

//! 资源文件包类
class COMMONTOOLS_API CTarFilePackage : public IFilePackage
{
public:
	CTarFilePackage(DWORD submitFlags = TARFILE_FLAGS_ENCRYPTTABLE);
	virtual ~CTarFilePackage (void);

	/** 
	* 关闭文件，会放弃调用submit后的所有操作
	*/
	virtual void close (void);

	/**
	* 取得文件枚举对象
	* @return	返回文件枚举
	*/
	virtual EntryEnumeration_ptr getEntries (void);

	/**
	* 取得包内文件信息（不区分大小写）
	* @param [in] name	文件名称
	*/
	virtual LPCFILEENTRY getEntry (LPCTSTR name);

	/**
	* 取得文件内容 
	* @param [in] entry		文件索引
	* @return 返回文件流，流用完后要自己删除
	*/
	virtual RandomReadStream_ptr getReadStream (LPCFILEENTRY entry);

	/**
	* 添加文件到包中 
	* @param [in] name		文件名称
	* @param [in] stream	数据流读取对象
	*/
	virtual void putNextEntry (LPCTSTR name, RandomReadStream_ptr stream);

	/**
	* 删除包中的文件 
	* @param [in] entry		文件索引
	*/
	virtual void removeEntry (LPCFILEENTRY entry);
	

	/** 计算文件总长度 */
	ULONGLONG calcPackageLength (void);

protected:
	/** 文件头 */
	typedef struct
	{
		TCHAR		HeaderTag[30];			/**< 文件头标识 */
		TCHAR		FilePassword[30];		/**< 文件密码，暂时留空不使用，为以后扩展考虑 */
		DWORD		FileVersion;			/**< 文件版本（0,1,2） */
		DWORD		FileOptions;			/**< 文件格式选项 */
		DWORD		DataTablesPos;			/**< 数据表起始位置 */
		DWORD		DataTablesSize;			/**< 数据表大小 */
	}TARFILEHEADER;

	typedef struct 
	{
		DWORD	dataLength;	/**< 文件长度(字节数) */
		DWORD	dataOffset;	/**< 在文件中的偏移量 */
		DWORD	nameLength;	/**< 文件名称长度 */
	}ENTRYINFO;

	/** entry头 */
	typedef struct
	{
		ENTRYINFO	fileInfo;			/**< 文件信息 */
		TCHAR		fileName[MAX_PATH];	/**< 文件名称 */
	}ENTRYHEADER, *LPENTRYHEADER;

	/** 把当前文件写入新文件 */
	void _submitToFile (HANDLE handle);

	/**
	* 更新当前文件上下文，当文件打开后会调用
	* @param [in] context	已创建并设置好的上下文
	*/
	virtual void _updateContext (OPENCONTEXT* context);

	/** 初使化文件头结构 */
	void _initFileHeaderStruct (TARFILEHEADER* header);

	/** 加密数据，解密也调用此函数，明文与密文长度相同 */
	DWORD _encryptDataBuffer(LPBYTE pDataBuffer, DWORD dwLength, LPCTSTR lpPasswrod, DWORD dwPwdLength);

	/** 读取数据文件列表 */
	void _loadFileEntryList (void* lpDatabaseBuffer, DWORD size);

	/** 创建一个文件对象 */
	LPFILEENTRY _newFileEntry (LPENTRYHEADER zipentry);
	LPFILEENTRY _newFileEntry (IRandomReadStream* stream);

	/** 删除一个文件对象 */
	void _deleteFileEntry (LPFILEENTRY fileentry);

	/** 在两个列表中查找已存在的结点 */
	BOOL _findEntryInArray (LPFILEENTRY find_entry, CAtlArray<LPFILEENTRY>** save_ptr, size_t* save_idx);

	/** 计算数据表长度 */
	DWORD _calcDataTableLength (DWORD* totalCount);

	void _submit(CString filePath);
protected:
	typedef CAtlFileMapping<char> CTarFileMapping;
	std::auto_ptr<CTarFileMapping>	_fileMapping;	/**< 用于把文件映射到内存 */
	TARFILEHEADER					_fileHeader;	/**< 文件头 */
	DWORD							_submitFlags;	/**< 加密标记 */

	CAtlArray<LPFILEENTRY>	_entryList;		/**< zip中已存在的文件列表 */
	CAtlArray<LPFILEENTRY>	_addEntry;		/**< 新添加的文件列表 */
	BOOL					_srcChanged;	/**< 标记原始文件是否有修改，如删除其中文件 */
};

/** * @} */
}}	// end namespace