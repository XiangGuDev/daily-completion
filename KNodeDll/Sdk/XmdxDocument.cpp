#include "StdAfx.h"
#include "XmdxDocument.h"
#include "../Sdk/MemoryDataStream.h"

namespace CommBase
{
	/************************************************************************/
	/* CXmdxNodeList                                                        */
	/************************************************************************/
	/*
	*	结点列表，用于保存所有结点信息
	*/
	CXmdxNodeList::CXmdxNodeList()
	{
		_blockSize = 512;	// sizeof(XMDXNODE)*512 = 5K
		_length = 0;
		_maxnodes = 0;
		_nodeListArray.SetCount(0, int(_blockSize));
	}

	CXmdxNodeList::~CXmdxNodeList()
	{
		clear();
	}

	BOOL CXmdxNodeList::getNewNode(LPXMDXNODE* nodePtr)
	{
		// 分配新的数据
		if (_length >= _maxnodes)
		{
			size_t newBytes = sizeof(XMDXNODE) *_blockSize;
			LPXMDXNODE nodeArray = (LPXMDXNODE) ::malloc(newBytes);
			memset(nodeArray, NULL, newBytes);

			_nodeListArray.Add(nodeArray);
			_maxnodes += _blockSize;
		}
		else
		{
			//TODO: 可以采用两个变量来代替getAt加快速度
		}

		// 设置值
		LPXMDXNODE node = getAt(_length);
		node->index = WORD(_length);
		*nodePtr = node;

		_length++;
		return TRUE;
	}

	size_t CXmdxNodeList::getCount()
	{
		return _length;
	}

	LPXMDXNODE CXmdxNodeList::getAt(size_t nIndex)
	{
		size_t arrIndex = nIndex / _blockSize;
		size_t posIndex = nIndex - arrIndex * _blockSize;

		LPXMDXNODE nodeArray = _nodeListArray[arrIndex];
		return &nodeArray[posIndex];
	}

	XMDXNODE& CXmdxNodeList::operator[] (size_t nIndex)
	{
		return *getAt(nIndex);
	}

	void CXmdxNodeList::clear()
	{
		for (size_t i = 0; i < _nodeListArray.GetCount(); i++)
		{
			::free(_nodeListArray[i]);
		}
		_nodeListArray.RemoveAll();

		_length = 0;
		_maxnodes = 0;
	}
	/************************************************************************/
	/* CXmdxDocumentWriter                                                          */
	/************************************************************************/
	CXmdxDocumentWriter::CXmdxDocumentWriter(IRandomWriteStream* file)
	{
		ATLASSERT(file != NULL);
		if (file == NULL)
		{
			_file = NULL;
			_root = NULL;
			return;
		}
		_file = file;

		// 生成并预写入头
		memset(&_header, NULL, sizeof(XMDXHEADER));
		strcpy_s(_header.tag, 5, XMDXHEADER_TAG);
		_header.version = XMDXFILE_VERSION;

		_headerAddress = file->GetPosition();
		file->Write(&_header, sizeof(XMDXHEADER));

		_header.ElementAddress = file->GetPosition();

		// 生成root
		_root = newElement(NULL, "xmdx");
	}

	CXmdxDocumentWriter::~CXmdxDocumentWriter()
	{
		close();
	}

	// 取得root结点
	IXmdxElementWriter* CXmdxDocumentWriter::rootElement()
	{
		return _root;
	}

	// 生成了一个新结点
	IXmdxElementWriter* CXmdxDocumentWriter::newElement(IXmdxElementWriter* parent, LPCSTR name)
	{
		if (_file == NULL)
		{
			_lastError.Format(_T("没有指定输出流对象。"));
			return NULL;
		}

		if (name == NULL)
		{
			_lastError.Format(_T("节点名称空指针。"));
			return NULL;
		}

		// 分配空间
		if (_nodes.getCount() >= MAXWORD)
		{
			_lastError.Format(_T("已达到支持的最大结点数量%d。"), MAXWORD);
			return NULL;
		}

		LPXMDXNODE node = NULL;
		if (!_nodes.getNewNode(&node))
		{
			_lastError.Format(_T("内存空间不足。"));
			return NULL;
		}

		// 设置节点关系
		if (parent)
		{
			// 设置当前结点的父结点
			node->parent = parent->getNode()->index;

			// 设置父结点的第一个子结点索引
			if (!parent->getNode()->firstChildren)
			{
				parent->getNode()->firstChildren = node->index;
			}
			else
			{
				// 更新上级点的兄弟结点为我
				DWORD prevSibling = parent->getNode()->lastChildren;
				_nodes.getAt(prevSibling)->nextSibling = node->index;
			}

			// 更新父结点的最后一个子结点索引为本结点
			parent->getNode()->lastChildren = node->index;
		}

		// 关联上结点对象
		CXmdxElementWriter* elmt = _ElementFactory.getnew();
		elmt->setNode(node);
		elmt->setName(name);
		elmt->setWriter(this);
		return elmt;
	}

	// 写入一个对象对数据源
	size_t CXmdxDocumentWriter::writeElement(IXmdxElementWriter* element)
	{
		if (element == NULL)
			return 0;

		// 避免重复写入（判断是否有地址数据）
		if (element->getNode()->entryAddress)
			return 0;

		return element->write(_file);
	}

	void CXmdxDocumentWriter::freeElement(IXmdxElementWriter* element)
	{
		if (element == NULL)
			return;

		CXmdxElementWriter* celmt = dynamic_cast<CXmdxElementWriter*> (element);
		if (celmt)
		{
			celmt->clear();
			_ElementFactory.recycle(celmt);
		}
		else
		{
			//delete celmt;  // SHL celmt一定为空指针
			delete element;
		}
	}

	// 关闭文件
	void CXmdxDocumentWriter::close()
	{
		if (!_root || !_file)
			return;

		// 写入并删除root结点
		_root->getWriter()->writeElement(_root);
		_root->getWriter()->freeElement(_root);

		// 更新头信息
		_header.NodeAddress = _file->GetPosition();
		_header.ElementBytes = _header.NodeAddress - _header.ElementAddress;
		_header.NodeCount = WORD(_nodes.getCount());

		// 写入结点
		for (size_t i = 0; i < _nodes.getCount(); i++)
		{
			LPXMDXNODE node = _nodes.getAt(i);
			_file->Write(node, sizeof(XMDXNODE));
		}

		DWORD curPosition = _file->GetPosition();
		_header.NodeBytes = curPosition - _header.NodeAddress;

		// 再次写入头
		_file->SetPosition(_headerAddress);
		_file->Write(&_header, sizeof(XMDXHEADER));
		_file->SetPosition(curPosition);

		// 结束
		_file = NULL;
		_root = NULL;
		_nodes.clear();
		_ElementFactory.clear();
		_memoryPool.ClearUp();
	}

	LPCTSTR CXmdxDocumentWriter::getLastError()
	{
		return _lastError;
	}

	// 取得内存池
	IAllocMemoryPool* CXmdxDocumentWriter::getMemoryPool()
	{
		return &_memoryPool;
	}

	size_t CXmdxDocumentWriter::GetNodeCount()
	{
		return _nodes.getCount();
	}

	/************************************************************************/
	/* CXmdxDocumentReader                                                  */
	/************************************************************************/
	CXmdxDocumentReader::CXmdxDocumentReader(IRandomReadStream* file)
	{
		_file = file;
		_nodes = NULL;
		_elements = NULL;

		// 读取文件头
		DWORD bytesRead = 0;

		// 如果流能转换成内存流，则可以不复制数据
		CMemoryDataReadStream* memoryStream = dynamic_cast<CMemoryDataReadStream*>(file);
		_closeFreeNodes = memoryStream ? FALSE : TRUE;
		_file->Read(&_header, sizeof(XMDXHEADER), bytesRead);
		if (bytesRead != sizeof(XMDXHEADER) || strcmp(_header.tag, XMDXHEADER_TAG))
		{
			_file = NULL;
		}
		else if (_header.version > XMDXFILE_VERSION)
		{
			_file = NULL;
		}

		// 读取结点数据
		if (_file && _header.NodeCount > 0)
		{
			DWORD allNodeBytes = _header.NodeCount * sizeof(XMDXNODE);
			if (_closeFreeNodes)
			{
				// 复制数据
				_nodes = (LPXMDXNODE) ::malloc(allNodeBytes);
				if (_nodes && _file->SetPosition(_header.NodeAddress))
				{
					_file->Read(_nodes, allNodeBytes, bytesRead);
				}
			}
			else
			{
				// 存指针
				LPBYTE buffer = (LPBYTE)memoryStream->getOriginalBuffer();
				_nodes = (LPXMDXNODE)(buffer + _header.NodeAddress);
				_file->SetPosition(_header.NodeAddress + allNodeBytes);
			}
		}

		// 读取内容数据
		if (_file && _header.NodeCount > 0)
		{
			if (_closeFreeNodes)
			{
				// 复制数据
				_elements = (LPBYTE) ::malloc(_header.ElementBytes);
				if (_elements && _file->SetPosition(_header.ElementAddress))
				{
					_file->Read(_elements, _header.ElementBytes, bytesRead);
				}
			}
			else
			{
				// 存指针
				LPBYTE buffer = (LPBYTE)memoryStream->getOriginalBuffer();
				_elements = buffer + _header.ElementAddress;
				_file->SetPosition(_header.ElementAddress + _header.ElementBytes);
			}
		}
	}

	CXmdxDocumentReader::~CXmdxDocumentReader()
	{
		close();
	}

	// 关闭读取文件
	void CXmdxDocumentReader::close()
	{
		// 把指针移动尾部
		if (_file)
		{
			_file = NULL;
		}

		if (_nodes)
		{
			if (_closeFreeNodes)
				::free(_nodes);
			_nodes = NULL;
		}
		if (_elements)
		{
			if (_closeFreeNodes)
				::free(_elements);
			_elements = NULL;
		}

		_memoryPool.ClearUp();
	}

	// 取得root结点
	XMDXHANDLE CXmdxDocumentReader::rootNode()
	{
		if (_nodes)
			return &_nodes[0];
		return NULL;
	}

	IXmdxElementReader* CXmdxDocumentReader::rootElement()
	{
		return readElement(rootNode());
	}

	// 从流中读取一个对象
	IXmdxElementReader* CXmdxDocumentReader::readElement(XMDXHANDLE handle)
	{
		if (_file && handle)
		{
			CXmdxElementReader* elmt = _ElementFactory.getnew();
			elmt->setNode(handle);
			elmt->setReader(this);
			elmt->read(_file);
			return elmt;
		}

		return NULL;
	}

	void CXmdxDocumentReader::freeElement(IXmdxElementReader* element)
	{
		if (element == NULL)
			return;

		CXmdxElementReader* celmt = dynamic_cast<CXmdxElementReader*> (element);
		if (celmt)
		{
			celmt->clear();
			_ElementFactory.recycle(celmt);
		}
		else
		{
			//delete celmt;
			delete element;
		}
	}

	// 查找结点的第一个子结点
	FINDHANDLE CXmdxDocumentReader::findFirstChild(XMDXHANDLE parent, XMDXHANDLE* findNode)
	{
		// 如果没有子结点则返回NULL，表示查找失败
		if (!parent || !parent->firstChildren)
			return NULL;

		// 创建查找结构
		LPXMDXFIND handle = _FindHanleFactory.getnew();

		handle->parent = parent;
		handle->index = parent->firstChildren;

		// 返回第一个子结点
		if (findNode)
		{
			*findNode = &_nodes[parent->firstChildren];
		}

		// 返回查找结构
		return (FINDHANDLE)handle;
	}

	// 查找结点的下一下子结点
	BOOL CXmdxDocumentReader::findNextChild(FINDHANDLE handle, XMDXHANDLE* findNode)
	{
		if (!handle) return FALSE;

		// 沿着当前查找结点为的nextSibling进行查找
		LPXMDXFIND findHandle = (LPXMDXFIND)handle;
		LPXMDXNODE curNode = &_nodes[findHandle->index];

		if (curNode->nextSibling)
		{
			findHandle->index = curNode->nextSibling;
			*findNode = &_nodes[curNode->nextSibling];
			return TRUE;
		}

		return FALSE;
	}

	// 关闭查找句柄
	void CXmdxDocumentReader::closeFindHandle(FINDHANDLE handle)
	{
		_FindHanleFactory.recycle((LPXMDXFIND)handle);
	}

	LPCTSTR CXmdxDocumentReader::getLastError()
	{
		return _lastError;
	}

	// 取得内存池
	IAllocMemoryPool* CXmdxDocumentReader::getMemoryPool()
	{
		return &_memoryPool;
	}

	// 取得指定结点的数据区
	LPVOID CXmdxDocumentReader::getElementBuffer(XMDXHANDLE handle)
	{
		if (handle == NULL)
			return NULL;

		DWORD index = handle->entryAddress - _header.ElementAddress;
		return &_elements[index];
	}
}