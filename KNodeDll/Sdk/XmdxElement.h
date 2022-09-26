//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           XmdxElement.H
//    @author         fanHong
//    @date           2017/11/3 11:29
//    @brief          Xmdx元素
//
//*****************************************************

#pragma once

#include "XmdxDefines.h"
#include <atlcoll.h>

namespace CommBase
{
	/*
	*	结点对象，主要包括属性操作方法
	*/
	class COMMBASE_API CXmdxElementWriter : public IXmdxElementWriter
	{
	public:
		CXmdxElementWriter();
		virtual ~CXmdxElementWriter();

		// 取得关联结点
		void setNode(XMDXHANDLE node);
		virtual XMDXHANDLE getNode();

		// 取得关联的文件写对象
		void setWriter(IXmdxDocumentWriter* writer);
		virtual IXmdxDocumentWriter* getWriter();

		// 读写名称
		virtual LPCSTR getName();
		virtual void setName(LPCSTR name);

		// 读写结点值
		virtual LPVOID getValue(size_t* readBytes);
		virtual void setValue(LPCSTR value, size_t length);

		// 设置属性值
		virtual BOOL addAttrText(LPCSTR name, LPCSTR value);
		virtual BOOL addAttrText(LPCSTR name, LPCWSTR value);
		virtual BOOL addAttrDouble(LPCSTR name, double value);
		virtual BOOL addAttrLong(LPCSTR name, DWORD value);
		virtual BOOL addAttrShort(LPCSTR name, WORD value);
		virtual BOOL addAttrByte(LPCSTR name, BYTE value);
		virtual BOOL addAttrBuffer(LPCSTR name, byte* value, int length);

		// 读写结点数据
		virtual BOOL write(IRandomWriteStream* stream);

		void clear();

	protected:
		// 写入字符串
		BOOL _writeText(IDataWriteStream* stream, LPCSTR text);

		// 如果属性已存在则直接返回，否则添加一个新的
		LPXMDXATTR _resetOrAddAttr(LPCSTR name);

		// 分配内存
		__inline LPVOID _malloc(size_t size, size_t nGrowBy);
		__inline void _free(LPVOID buffer);

	protected:
		IXmdxDocumentWriter*	_writer;
		XMDXHANDLE				_attachNode;

		LPVOID					_value;
		WORD					_valLength;

		char					_name[MAX_PATH];
		CAtlArray<LPXMDXATTR>	_attrArray;
	};

	/*
	*	结点对象，主要包括属性操作方法
	*/
	class COMMBASE_API CXmdxElementReader : public IXmdxElementReader
	{
	public:
		CXmdxElementReader();
		virtual ~CXmdxElementReader();

		// 取得关联结点
		void setNode(XMDXHANDLE node);
		virtual XMDXHANDLE getNode();

		// 取得关联的文件读对象
		void setReader(IXmdxDocumentReader* reader);
		virtual IXmdxDocumentReader* getReader();

		// 读写名称
		virtual LPCSTR getName();

		// 读写结点值
		virtual size_t getValueLength();
		virtual LPCSTR getValue();

		virtual size_t getAttrCount();
		virtual LPCSTR getAttrName(int index);
		virtual BYTE getAttrType(int index);

		// 取属性值
		virtual BOOL getAttrValue(int index, LPXMDXATTR attr);
		virtual LPCSTR getAttrTextA(LPCSTR name, LPCSTR defVal = "");
		virtual LPCWSTR getAttrTextW(LPCSTR name, LPCWSTR defVal = L"");
		virtual double getAttrDouble(LPCSTR name, double defVal = 0.0);
		virtual DWORD getAttrLong(LPCSTR name, DWORD defVal);
		virtual WORD getAttrShort(LPCSTR name, WORD defVal);
		virtual BYTE getAttrByte(LPCSTR name, BYTE defVal);
		virtual byte* getAttrBuffer(LPCSTR name, int &length, byte* defVal = 0);

		// 读写结点数据
		virtual BOOL read(IRandomReadStream* stream);

		void clear();

	protected:
		// 取得指定索引的属性的名称及值
		__inline void _lookupAttr(int index, LPCSTR* namePtr, LPCSTR* valuePtr);
		void _lookupAttr(int index, LPXMDXATTR findAttr);
		BOOL _lookupAttr(LPCSTR name, LPXMDXATTR findAttr);

	protected:
		IXmdxDocumentReader*	_reader;
		XMDXHANDLE				_attachNode;

		LPCSTR					_buffer;

		LPCSTR					_name;			// 名称及长度
		WORD					_nameLength;
		LPCSTR					_value;			// 值及长度
		WORD					_valLength;

		LPXMDXATTRHEADER		_attrHeadEntry;	// 属性头开始地址
		LPCSTR					_attrValueEntry;// 属性值开始地址
	};
}