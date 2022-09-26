//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           XmdxDocument.H
//    @author         fanHong
//    @date           2017/11/3 11:22
//    @brief          Xmdx文档
//
//*****************************************************

#pragma once

#include "XmdxElement.h"
#include "ObjectFastNew.h"
#include "AllocMemoryPool.h"

namespace CommBase
{
	/*
	*	结点列表，用于保存所有结点信息
	*/
	class CXmdxNodeList
	{
	public:
		CXmdxNodeList();
		~CXmdxNodeList();
		BOOL getNewNode(LPXMDXNODE* nodePtr);
		size_t getCount();
		LPXMDXNODE getAt(size_t nIndex);
		XMDXNODE& operator[] (size_t nIndex);
		void clear();

	protected:
		CAtlArray<LPXMDXNODE>	_nodeListArray;
		size_t					_length;
		size_t					_blockSize;
		size_t					_maxnodes;
	};

	/*
	 *	生成xmdx文档
	 */
	class COMMBASE_API CXmdxDocumentWriter : public IXmdxDocumentWriter
	{
	public:
		CXmdxDocumentWriter(IRandomWriteStream* file);
		~CXmdxDocumentWriter();

		// 取得root结点
		virtual IXmdxElementWriter* rootElement();

		// 生成了一个新结点
		virtual IXmdxElementWriter* newElement(IXmdxElementWriter* parent, LPCSTR name);

		// 写入一个对象对数据源
		virtual size_t writeElement(IXmdxElementWriter* element);

		virtual void freeElement(IXmdxElementWriter* element);

		// 关闭文件
		virtual void close();

		virtual LPCTSTR getLastError();

		// 取得内存池
		virtual IAllocMemoryPool* getMemoryPool();

		/**
		*  @brief    取得节点数量
		*
		*  @return   size_t 节点总数量
		*/
		virtual size_t GetNodeCount();

	protected:
		IRandomWriteStream*	_file;
		CXmdxNodeList		_nodes;
		IXmdxElementWriter*	_root;
		CString				_lastError;

		XMDXHEADER			_header;
		DWORD				_headerAddress;

		CAllocMemoryPool	_memoryPool;

		CObjectFastNew<CXmdxElementWriter>	_ElementFactory;

	};

	/*
	 *	读取xmdx文档
	 */
	class COMMBASE_API CXmdxDocumentReader : public IXmdxDocumentReader
	{
	public:
		CXmdxDocumentReader(IRandomReadStream* file);
		~CXmdxDocumentReader();

		// 取得root结点
		virtual XMDXHANDLE rootNode();

		// 从流中读取一个对象
		virtual IXmdxElementReader* readElement(XMDXHANDLE handle);

		virtual void freeElement(IXmdxElementReader* element);

		// 查找结点的第一个子结点
		virtual FINDHANDLE findFirstChild(XMDXHANDLE parent, XMDXHANDLE* findNode);

		// 查找结点的下一下子结点
		virtual BOOL findNextChild(FINDHANDLE handle, XMDXHANDLE* findNode);

		// 关闭查找句柄
		virtual void closeFindHandle(FINDHANDLE handle);

		// 关闭读取文件
		virtual void close();

		virtual LPCTSTR getLastError();

		// 取得内存池
		virtual IAllocMemoryPool* getMemoryPool();

		// 取得指定结点的数据区
		LPVOID getElementBuffer(XMDXHANDLE handle);

		// 取得跟节点读取器
		IXmdxElementReader* rootElement();

	protected:
		IRandomReadStream*	_file;

		XMDXHEADER			_header;		/**< 文件头 */

		LPXMDXNODE			_nodes;			/**< 结点头 */
		LPBYTE				_elements;		/**< 结点数据 */
		BOOL				_closeFreeNodes;/**< 关闭时是否释放 _nodes、_elements */

		CString				_lastError;
		CAllocMemoryPool	_memoryPool;

		CObjectFastNew<CXmdxElementReader>	_ElementFactory;
		CObjectFastNew<XMDXFIND>			_FindHanleFactory;
	};
}