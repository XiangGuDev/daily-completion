//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           XmdxDefines.H
//    @author         fanHong
//    @date           2017/11/3 11:21
//    @brief          Xmdx读写
//
//*****************************************************

#pragma once

#include "IDataStream.h"
#include "AllocMemoryPoolDef.h"

namespace CommBase
{
	//! 用于xmdx文件的读写，最多支持MAXWORD个结点（65535）
	#pragma pack(push, 1)

	// xmdx 文件头
	typedef struct
	{
		char	tag[5];			// 文件标记 XMDXHEADER_TAG
		BYTE	version;		// 版本号, 1,2,3……
		BYTE	encrypt;		// 加密版本号，0表示不加密
		WORD	NodeCount;		// 结点数量
		DWORD	ElementBytes;	// 结点内容字节数
		DWORD	NodeBytes;		// 结点字节数
		DWORD	ElementAddress;	// 结点内容开始地址
		DWORD	NodeAddress;	// 结点开始地址
	}XMDXHEADER;

	#define XMDXHEADER_TAG "xmdx"
	#define	XMDXFILE_VERSION	1	// xmdx文件版本号

	#define XMDXATTR_FLOAT		1	// 浮点数
	#define XMDXATTR_DWORD		2	// 长整型
	#define XMDXATTR_WORD		3	// 短型型
	#define XMDXATTR_BYTE		4	// 字节型
	#define XMDXATTR_TEXT		5	// 文本型
	#define XMDXATTR_TEXTW		6	// Unicode文本型
	#define XMDXATTR_BUFFER		7	// 字节流

	// 结点属性
	typedef struct
	{
		char name[MAX_PATH];
		BYTE type;
		union
		{
			double	floatVal;
			DWORD	longVal;
			WORD	intVal;
			BYTE	byteVal;
			char*	textVal;
			WCHAR*	wTextVal;
			BYTE*	bufferVal;
		};
	}XMDXATTR, *LPXMDXATTR;

	// 写入文件的结点属性值
	typedef struct
	{
		BYTE	type;		// 结点类型
		WORD	nameLen;	// 名称长度
		WORD	valueLen;	// 值长度
		WORD	datOffset;	// 数据偏移量
	}XMDXATTRHEADER, *LPXMDXATTRHEADER;

	// 结点内存信息
	typedef struct
	{
		DWORD	elementBytes;	// 结点总字节数
		BYTE	attrCount;		// 总共属性数量
	}XMDXELMTINFO, *LPXMDXELMTINFO;

	// xmdx结点
	typedef struct
	{
		WORD			index;			// 结点索引，为0表示根结点
		WORD			parent;			// 父结点索引，index为0时parent无意义
		WORD			nextSibling;	// 下一兄弟结点，为0表示没有
		WORD			firstChildren;	// 存储第一个子结点的索引，为0表示无子结点
		WORD			lastChildren;	// 存储最后个子结点的索引，为0表示无子结点

		XMDXELMTINFO	entryInfo;		// 结点值信息
		DWORD			entryAddress;	// 本结点在文件中的数据地址 > 0
	}XMDXNODE, *LPXMDXNODE;

	// 用于查找的结点
	typedef struct
	{
		LPXMDXNODE	parent;		// 查找结点的父结点
		WORD		index;		// 当前索引，默认为0
	}XMDXFIND, *LPXMDXFIND;

	#pragma pack(pop)

	typedef LPXMDXNODE XMDXHANDLE;
	typedef LPXMDXFIND FINDHANDLE;

	class COMMBASE_API IXmdxDocumentWriter;
	class COMMBASE_API IXmdxDocumentReader;

	/*
	*	结点对象，主要包括属性操作方法
	*
	*	节点名、节点值、属性名只支持使用ANSI字符集
	*	字符串类型的属性值同时支持ANSI和Unicode，根据设置属性值时传入的参数类型自动判断
	*/
	class COMMBASE_API IXmdxElementWriter
	{
	public:
		virtual ~IXmdxElementWriter()
		{
		}

		// 取得关联结点
		virtual XMDXHANDLE getNode() = 0;

		// 取得关联的文件写对象
		virtual IXmdxDocumentWriter* getWriter() = 0;

		// 读写名称
		virtual LPCSTR getName() = 0;
		virtual void setName(LPCSTR name) = 0;

		// 读写结点值
		virtual LPVOID getValue(size_t* readBytes = 0) = 0;
		virtual void setValue(LPCSTR value, size_t length) = 0;

		// 设置属性值
		virtual BOOL addAttrText(LPCSTR name, LPCSTR value) = 0;	// XMDXATTR_TEXT
		virtual BOOL addAttrText(LPCSTR name, LPCWSTR value) = 0;	// XMDXATTR_TEXTW
		virtual BOOL addAttrDouble(LPCSTR name, double value) = 0;	// XMDXATTR_FLOAT
		virtual BOOL addAttrLong(LPCSTR name, DWORD value) = 0;	// XMDXATTR_DWORD
		virtual BOOL addAttrShort(LPCSTR name, WORD value) = 0;	// XMDXATTR_WORD
		virtual BOOL addAttrByte(LPCSTR name, BYTE value) = 0;		// XMDXATTR_BYTE
		virtual BOOL addAttrBuffer(LPCSTR name, byte* value, int length) = 0;	// XMDXATTR_BUFFER

		// 读写结点数据												   
		virtual BOOL write(IRandomWriteStream* stream) = 0;
	};

	/*
	*	结点对象，主要包括属性操作方法
	*/
	class COMMBASE_API IXmdxElementReader
	{
	public:
		virtual ~IXmdxElementReader()
		{
		}

		// 取得关联结点
		virtual XMDXHANDLE getNode() = 0;

		// 取得关联的文件读对象
		virtual IXmdxDocumentReader* getReader() = 0;

		// 读写名称
		virtual LPCSTR getName() = 0;

		// 读写结点值
		virtual size_t getValueLength() = 0;
		virtual LPCSTR getValue() = 0;

		virtual size_t getAttrCount() = 0;
		virtual LPCSTR getAttrName(int index) = 0;
		virtual BYTE getAttrType(int index) = 0;

		// 取属性值
		virtual BOOL getAttrValue(int index, LPXMDXATTR attr) = 0;

		// 该方法不进行类型转换，指定name的属性值不是XMDXATTR_TEXT时返回defVal
		virtual LPCSTR getAttrTextA(LPCSTR name, LPCSTR defVal = "") = 0;

		// 该方法不进行类型转换，指定name的属性值不是XMDXATTR_TEXTW时返回defVal
		virtual LPCWSTR getAttrTextW(LPCSTR name, LPCWSTR defVal = L"") = 0;

		// 该方法会进行类型转换，所有类型均会以double返回
		virtual double getAttrDouble(LPCSTR name, double defVal = 0.0) = 0;

		// 该方法会进行类型转换，所有类型均会以DWORD返回
		virtual DWORD getAttrLong(LPCSTR name, DWORD defVal = 0) = 0;

		// 该方法会进行类型转换，所有类型均会以WORD返回
		virtual WORD getAttrShort(LPCSTR name, WORD defVal = 0) = 0;

		// 该方法会进行类型转换，所有类型均会以BYTE返回
		virtual BYTE getAttrByte(LPCSTR name, BYTE defVal = 0) = 0;

		// 该方法不进行类型转换，指定name的属性值不是XMDXATTR_BUFFER时返回defVal
		virtual byte* getAttrBuffer(LPCSTR name, int &length, byte* defVal = 0) = 0;

		// 读写结点数据
		virtual BOOL read(IRandomReadStream* stream) = 0;
	};

	/*
	*	生成xmdx文档
	*/
	class COMMBASE_API IXmdxDocumentWriter
	{
	public:
		virtual ~IXmdxDocumentWriter()
		{
		}

		// 取得root结点
		virtual IXmdxElementWriter* rootElement() = 0;

		// 生成了一个新结点
		virtual IXmdxElementWriter* newElement(IXmdxElementWriter* parent, LPCSTR name) = 0;

		virtual void freeElement(IXmdxElementWriter* element) = 0;

		// 写入一个对象对数据源
		virtual size_t writeElement(IXmdxElementWriter* element) = 0;

		// 关闭文件
		virtual void close() = 0;

		virtual LPCTSTR getLastError() = 0;

		// 取得内存池
		virtual IAllocMemoryPool* getMemoryPool() = 0;

		// 取得节点数量
		virtual size_t GetNodeCount() = 0;
	};

	/*
	*	读取xmdx文档
	*/
	class COMMBASE_API IXmdxDocumentReader
	{
	public:
		virtual ~IXmdxDocumentReader()
		{
		}

		// 取得root结点
		virtual XMDXHANDLE rootNode() = 0;

		// 从流中读取一个对象
		virtual IXmdxElementReader* readElement(XMDXHANDLE handle) = 0;

		virtual void freeElement(IXmdxElementReader* element) = 0;

		// 查找结点的第一个子结点
		virtual FINDHANDLE findFirstChild(XMDXHANDLE parent, XMDXHANDLE* findNode) = 0;

		// 查找结点的下一下子结点
		virtual BOOL findNextChild(FINDHANDLE handle, XMDXHANDLE* findNode) = 0;

		// 关闭查找句柄
		virtual void closeFindHandle(FINDHANDLE handle) = 0;

		// 关闭读取文件
		virtual void close() = 0;

		virtual LPCTSTR getLastError() = 0;

		// 取得内存池
		virtual IAllocMemoryPool* getMemoryPool() = 0;
	};

	/*
	*	xmdx对象存储接口
	*/
	class COMMBASE_API IXmdxStorage
	{
	public:
		virtual ~IXmdxStorage()
		{
		}

		virtual void loadXmdxStorage(IXmdxElementReader* element) = 0;
		virtual void saveXmdxStorage(IXmdxElementWriter* element) = 0;
	};
}