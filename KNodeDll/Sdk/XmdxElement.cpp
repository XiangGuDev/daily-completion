#include "StdAfx.h"
#include "XmdxElement.h"
#include "XmdxDocument.h"

namespace CommBase
{
	/************************************************************************/
	/* CXmdxElementWriter                                                   */
	/************************************************************************/
	CXmdxElementWriter::CXmdxElementWriter()
	{
		_name[0] = 0;
		_attachNode = NULL;
		_writer = NULL;

		_valLength = 0;
		_value = NULL;
	}

	CXmdxElementWriter::~CXmdxElementWriter()
	{
		clear();
	}

	// 读写名称
	LPCSTR CXmdxElementWriter::getName()
	{
		return _name;
	}

	void CXmdxElementWriter::setName(LPCSTR name)
	{
		_name[0] = 0;
		if (name)
			strncpy_s(_name, MAX_PATH, name, MAX_PATH - 1);
	}

	void CXmdxElementWriter::setNode(XMDXHANDLE node)
	{
		_attachNode = node;
	}

	XMDXHANDLE CXmdxElementWriter::getNode()
	{
		return _attachNode;
	}

	void CXmdxElementWriter::setWriter(IXmdxDocumentWriter* writer)
	{
		_writer = writer;
	}

	IXmdxDocumentWriter* CXmdxElementWriter::getWriter()
	{
		return _writer;
	}

	// 读写结点值
	LPVOID CXmdxElementWriter::getValue(size_t* readBytes)
	{
		if (readBytes)
			*readBytes = _valLength;

		return _value;
	}

	void CXmdxElementWriter::setValue(LPCSTR value, size_t length)
	{
		if (value == NULL || length == 0)
			return;

		if (_value)
		{
			_free(_value);
		}

		if (length)
			_value = _malloc(length, 16);
		else
			_value = NULL;

		if (_value)
			memcpy_s(_value, length, value, length);

		_valLength = WORD(_value ? length : 0);
	}

	// 设置属性值
	BOOL CXmdxElementWriter::addAttrText(LPCSTR name, LPCSTR value)
	{
		if (strlen(value) == 0)
			return FALSE;

		LPXMDXATTR attr = _resetOrAddAttr(name);
		if (!attr) return FALSE;

		attr->type = XMDXATTR_TEXT;
		size_t len = strlen(value) + 1;
		LPSTR desc = (LPSTR)_malloc(len, 512);
		memcpy(desc, value, len);
		attr->textVal = desc;
		return TRUE;
	}

	BOOL CXmdxElementWriter::addAttrText(LPCSTR name, LPCWSTR value)
	{
		if (wcslen(value) == 0)
			return FALSE;

		LPXMDXATTR attr = _resetOrAddAttr(name);
		if (!attr) return FALSE;

		attr->type = XMDXATTR_TEXTW;
		size_t len = (wcslen(value) + 1) * 2;
		LPWSTR desc = (LPWSTR)_malloc(len, 512);
		memcpy(desc, value, len);
		attr->wTextVal = desc;
		return TRUE;
	}

	BOOL CXmdxElementWriter::addAttrDouble(LPCSTR name, double value)
	{
		LPXMDXATTR attr = _resetOrAddAttr(name);
		if (!attr) return FALSE;

		attr->type = XMDXATTR_FLOAT;
		attr->floatVal = value;
		return TRUE;
	}

	BOOL CXmdxElementWriter::addAttrLong(LPCSTR name, DWORD value)
	{
		LPXMDXATTR attr = _resetOrAddAttr(name);
		if (!attr) return FALSE;

		attr->type = XMDXATTR_DWORD;
		attr->longVal = value;
		return TRUE;
	}

	BOOL CXmdxElementWriter::addAttrShort(LPCSTR name, WORD value)
	{
		LPXMDXATTR attr = _resetOrAddAttr(name);
		if (!attr) return FALSE;

		attr->type = XMDXATTR_WORD;
		attr->longVal = value;
		return TRUE;
	}

	BOOL CXmdxElementWriter::addAttrByte(LPCSTR name, BYTE value)
	{
		LPXMDXATTR attr = _resetOrAddAttr(name);
		if (!attr) return FALSE;

		attr->type = XMDXATTR_BYTE;
		attr->byteVal = value;
		return TRUE;
	}

	BOOL CXmdxElementWriter::addAttrBuffer(LPCSTR name, byte* value, int length)
	{
		if (length == 0)
			return FALSE;

		LPXMDXATTR attr = _resetOrAddAttr(name);
		if (!attr) return FALSE;

		attr->type = XMDXATTR_BUFFER;
		byte* desc = (byte*)_malloc(sizeof(length) + length, 512);
		memcpy(desc, &length, sizeof(length));
		memcpy(desc + sizeof(length), value, length);
		attr->bufferVal = desc;
		return TRUE;
	}

	// 读写结点数据，返回读写的字节数
	BOOL CXmdxElementWriter::write(IRandomWriteStream* stream)
	{
		if (stream == NULL)
			return FALSE;

		// 更新结点信息
		LPXMDXELMTINFO elmtHeader = &_attachNode->entryInfo;
		_attachNode->entryAddress = stream->GetPosition();
		elmtHeader->attrCount = BYTE(_attrArray.GetCount());

		// 写入结点名称及结点值
		_writeText(stream, _name);
		stream->Write(&_valLength, sizeof(WORD));
		if (_valLength) stream->Write(_value, _valLength);

		// 循环写入属性头
		WORD datOffset = 0;
		XMDXATTRHEADER attrHeader;

		for (size_t i = 0; i < _attrArray.GetCount(); i++)
		{
			LPXMDXATTR attr = _attrArray[i];

			attrHeader.type = attr->type;
			attrHeader.nameLen = WORD(strlen(attr->name) + 1);
			attrHeader.valueLen = 0;

			switch (attr->type)
			{
			case XMDXATTR_BYTE:
				attrHeader.valueLen = sizeof(BYTE); break;
			case XMDXATTR_WORD:
				attrHeader.valueLen = sizeof(WORD); break;
			case XMDXATTR_DWORD:
				attrHeader.valueLen = sizeof(DWORD); break;
			case XMDXATTR_FLOAT:
				attrHeader.valueLen = sizeof(DOUBLE); break;
			case XMDXATTR_TEXT:
				attrHeader.valueLen = WORD(strlen(attr->textVal) + 1); break;
			case XMDXATTR_TEXTW:
				attrHeader.valueLen = WORD(2 * (wcslen(attr->wTextVal) + 1)); break;
			case XMDXATTR_BUFFER:
				attrHeader.valueLen = WORD(*((int*)(attr->bufferVal)) + sizeof(int)); break;
			default:
				ATLASSERT(FALSE);
			}

			attrHeader.datOffset = datOffset;
			datOffset += attrHeader.nameLen + attrHeader.valueLen;

			stream->Write(&attrHeader, sizeof(XMDXATTRHEADER));
		}

		// 循环写入所有属性值
		for (size_t i = 0; i < _attrArray.GetCount(); i++)
		{
			LPXMDXATTR attr = _attrArray[i];

			// 写入属性名称
			stream->Write(attr->name, DWORD(strlen(attr->name) + 1));

			// 写入属性值
			switch (attr->type)
			{
			case XMDXATTR_BYTE:
				stream->Write(&attr->byteVal, sizeof(BYTE)); break;
			case XMDXATTR_WORD:
				stream->Write(&attr->intVal, sizeof(WORD)); break;
			case XMDXATTR_DWORD:
				stream->Write(&attr->longVal, sizeof(DWORD)); break;
			case XMDXATTR_FLOAT:
				stream->Write(&attr->floatVal, sizeof(DOUBLE)); break;
			case XMDXATTR_TEXT:
				stream->Write(attr->textVal, DWORD(strlen(attr->textVal) + 1)); break;
			case XMDXATTR_TEXTW:
				stream->Write(attr->wTextVal, DWORD(2 * (wcslen(attr->wTextVal) + 1))); break;
			case XMDXATTR_BUFFER:
				stream->Write(attr->bufferVal, DWORD(*((int*)(attr->bufferVal)) + sizeof(int))); break;
			default:
				ATLASSERT(FALSE);
			}
		}

		// 更新结点内容长度
		elmtHeader->elementBytes = stream->GetPosition() - _attachNode->entryAddress;

		return TRUE;
	}

	void CXmdxElementWriter::clear()
	{
		for (size_t i = 0; i < _attrArray.GetCount(); i++)
		{
			LPXMDXATTR attr = _attrArray[i];
			if (attr->type == XMDXATTR_TEXT && attr->textVal)
				_free(attr->textVal);
			if (attr->type == XMDXATTR_TEXTW && attr->wTextVal)
				_free(attr->wTextVal);
			if (attr->type == XMDXATTR_BUFFER && attr->bufferVal)
				_free(attr->bufferVal);
			_free(attr);
		}

		_attrArray.RemoveAll();
		_name[0] = 0;

		if (_value)
		{
			_free(_value);
			_value = NULL;
		}
		_valLength = 0;
	}

	// 写入字符串
	BOOL CXmdxElementWriter::_writeText(IDataWriteStream* stream, LPCSTR text)
	{
		// 写入长度（包括结尾\0）
		WORD length = WORD(strlen(text) + 1);
		if (!stream->Write(&length, sizeof(WORD)))
			return FALSE;

		// 写入内容（包括结尾\0）
		return stream->Write(text, length);
	}

	// 如果属性已存在则直接返回，否则添加一个新的
	LPXMDXATTR CXmdxElementWriter::_resetOrAddAttr(LPCSTR name)
	{
		if (name == NULL || strlen(name) == 0)
		{
			return NULL;
		}
		// 分配空间
		if (_attrArray.GetCount() >= MAXBYTE)
		{
			return NULL;
		}

		LPXMDXATTR attr = (LPXMDXATTR)_malloc(sizeof(XMDXATTR), 512);
		memset(attr, NULL, sizeof(XMDXATTR));
		_attrArray.Add(attr);

		strncpy_s(attr->name, MAX_PATH, name, MAX_PATH - 1);
		return attr;
	}

	// 分配内存
	__inline LPVOID CXmdxElementWriter::_malloc(size_t size, size_t nGrowBy)
	{
		IAllocMemoryPool* pool = _writer ? _writer->getMemoryPool() : NULL;

		if (pool)
			return pool->Alloc(size, nGrowBy);
		else
			return ::malloc(size);
	}

	__inline void CXmdxElementWriter::_free(LPVOID buffer)
	{
		IAllocMemoryPool* pool = _writer ? _writer->getMemoryPool() : NULL;

		if (pool)
			pool->Free(buffer);
		else
			::free(buffer);
	}


	/************************************************************************/
	/* CXmdxElementReader                                                   */
	/************************************************************************/
	CXmdxElementReader::CXmdxElementReader()
	{
		_attachNode = NULL;
		_reader = NULL;

		clear();
	}

	CXmdxElementReader::~CXmdxElementReader()
	{
		clear();
	}

	void CXmdxElementReader::clear()
	{
		_buffer = NULL;
		_name = NULL;
		_nameLength = 0;
		_valLength = 0;
		_value = NULL;

		_attrHeadEntry = NULL;
		_attrValueEntry = NULL;
	}

	void CXmdxElementReader::setNode(XMDXHANDLE node)
	{
		_attachNode = node;
	}

	XMDXHANDLE CXmdxElementReader::getNode()
	{
		return _attachNode;
	}

	void CXmdxElementReader::setReader(IXmdxDocumentReader* reader)
	{
		_reader = reader;
	}

	IXmdxDocumentReader* CXmdxElementReader::getReader()
	{
		return _reader;
	}

	// 读写名称
	LPCSTR CXmdxElementReader::getName()
	{
		return _name;
	}

	// 读写结点值
	size_t CXmdxElementReader::getValueLength()
	{
		return _valLength;
	}

	LPCSTR CXmdxElementReader::getValue()
	{
		return _value;
	}

	size_t CXmdxElementReader::getAttrCount()
	{
		return _attachNode->entryInfo.attrCount;
	}

	LPCSTR CXmdxElementReader::getAttrName(int index)
	{
		if (index < 0 || index >= (int)getAttrCount())
			return "";

		LPCSTR namePtr = NULL, valuePtr = NULL;
		_lookupAttr(index, &namePtr, &valuePtr);

		return namePtr;
	}

	BYTE CXmdxElementReader::getAttrType(int index)
	{
		if (index < 0 || index >= (int)getAttrCount())
			return XMDXATTR_FLOAT;

		return _attrHeadEntry[index].type;
	}

	// 取属性值
	BOOL CXmdxElementReader::getAttrValue(int index, LPXMDXATTR attr)
	{
		if (index < 0 || index >= _attachNode->entryInfo.attrCount)
			return FALSE;

		_lookupAttr(index, attr);
		return TRUE;
	}

	LPCSTR CXmdxElementReader::getAttrTextA(LPCSTR name, LPCSTR defVal)
	{
		XMDXATTR attr = { 0 };
		if (_lookupAttr(name, &attr))
		{
			if (attr.type == XMDXATTR_TEXT)
				return attr.textVal;
		}
		return defVal;
	}

	LPCWSTR CXmdxElementReader::getAttrTextW(LPCSTR name, LPCWSTR defVal)
	{
		XMDXATTR attr = { 0 };
		if (_lookupAttr(name, &attr))
		{
			if (attr.type == XMDXATTR_TEXTW)
				return attr.wTextVal;
		}
		return defVal;
	}



	double CXmdxElementReader::getAttrDouble(LPCSTR name, double defVal)
	{
		XMDXATTR attr = { 0 };
		if (_lookupAttr(name, &attr))
		{
			switch (attr.type)
			{
			case XMDXATTR_FLOAT:
				return attr.floatVal;
			case XMDXATTR_DWORD:
				return attr.longVal;
			case XMDXATTR_WORD:
				return attr.intVal;
			case XMDXATTR_BYTE:
				return attr.byteVal;
			case XMDXATTR_TEXT:
				return atof(attr.textVal);
			case XMDXATTR_TEXTW:
				return _wtol(attr.wTextVal);
			default:
				ATLASSERT(FALSE);
			}
		}

		return defVal;
	}

	DWORD CXmdxElementReader::getAttrLong(LPCSTR name, DWORD defVal)
	{
		XMDXATTR attr = { 0 };
		if (_lookupAttr(name, &attr))
		{
			switch (attr.type)
			{
			case XMDXATTR_DWORD:
				return attr.longVal;
			case XMDXATTR_WORD:
				return attr.intVal;
			case XMDXATTR_BYTE:
				return attr.byteVal;
			case XMDXATTR_FLOAT:
				return (DWORD)attr.floatVal;
			case XMDXATTR_TEXT:
				return atol(attr.textVal);
			case XMDXATTR_TEXTW:
				return _wtol(attr.wTextVal);
			default:
				ATLASSERT(FALSE);
			}
		}

		return defVal;
	}

	WORD CXmdxElementReader::getAttrShort(LPCSTR name, WORD defVal)
	{
		XMDXATTR attr = { 0 };
		if (_lookupAttr(name, &attr))
		{
			switch (attr.type)
			{
			case XMDXATTR_WORD:
				return attr.intVal;
			case XMDXATTR_DWORD:
				return (WORD)attr.longVal;
			case XMDXATTR_BYTE:
				return attr.byteVal;
			case XMDXATTR_FLOAT:
				return (WORD)attr.floatVal;
			case XMDXATTR_TEXT:
				return (WORD)atol(attr.textVal);
			case XMDXATTR_TEXTW:
				return (WORD)_wtol(attr.wTextVal);
			default:
				ATLASSERT(FALSE);
			}
		}

		return defVal;
	}

	BYTE CXmdxElementReader::getAttrByte(LPCSTR name, BYTE defVal)
	{
		XMDXATTR attr = { 0 };
		if (_lookupAttr(name, &attr))
		{
			switch (attr.type)
			{
			case XMDXATTR_BYTE:
				return attr.byteVal;
			case XMDXATTR_WORD:
				return (BYTE)attr.intVal;
			case XMDXATTR_DWORD:
				return (BYTE)attr.longVal;
			case XMDXATTR_FLOAT:
				return (BYTE)attr.floatVal;
			case XMDXATTR_TEXT:
				return (BYTE)atol(attr.textVal);
			case XMDXATTR_TEXTW:
				return (BYTE)_wtol(attr.wTextVal);
			default:
				ATLASSERT(FALSE);
			}
		}

		return defVal;
	}

	byte* CXmdxElementReader::getAttrBuffer(LPCSTR name, int &length, byte* defVal /*= 0*/)
	{
		XMDXATTR attr = { 0 };
		if (_lookupAttr(name, &attr))
		{
			if (attr.type == XMDXATTR_BUFFER)
			{
				length = *((int*)(attr.bufferVal));
				return attr.bufferVal + sizeof(int);
			}
		}
		return defVal;
	}

	BOOL CXmdxElementReader::read(IRandomReadStream* stream)
	{
		// 读取数据区
		CXmdxDocumentReader* reader = dynamic_cast<CXmdxDocumentReader*> (_reader);
		if (!_attachNode || !reader)
			return FALSE;

		_buffer = (LPCSTR)reader->getElementBuffer(_attachNode);
		if (!_buffer) return FALSE;

		DWORD index = 0;
		// 读取名称（头两字节是长度，后面是名称）
		_nameLength = *((WORD*)&_buffer[index]);
		index += sizeof(WORD);
		_name = (LPCSTR)&_buffer[index];
		index += _nameLength;

		// 读取值
		_valLength = *((WORD*)&_buffer[index]);
		index += sizeof(WORD);
		if (_valLength)
			_value = &_buffer[index];
		else
			_value = NULL;
		index += _valLength;

		// 读取属性头
		_attrHeadEntry = (LPXMDXATTRHEADER)&_buffer[index];
		index += sizeof(XMDXATTRHEADER) * _attachNode->entryInfo.attrCount;

		// 读取属性内容
		_attrValueEntry = &_buffer[index];

		return TRUE;
	}

	__inline void CXmdxElementReader::_lookupAttr(int index, LPCSTR* namePtr, LPCSTR* valuePtr)
	{
		if (index < 0 || index >= (int)getAttrCount())
			return;

		WORD datOffset = _attrHeadEntry[index].datOffset;
		*namePtr = &_attrValueEntry[datOffset];
		datOffset += _attrHeadEntry[index].nameLen;

		*valuePtr = &_attrValueEntry[datOffset];
	}

	// 查找指定名称的属性
	BOOL CXmdxElementReader::_lookupAttr(LPCSTR name, LPXMDXATTR findAttr)
	{
		if (name == NULL)
			return FALSE;

		// 查找指定名称的属性
		LPCSTR namePtr = NULL, valuePtr = NULL;
		for (int i = 0; i < _attachNode->entryInfo.attrCount; i++)
		{
			_lookupAttr(i, &namePtr, &valuePtr);

			if (!strcmp(namePtr, name))
			{
				_lookupAttr(i, findAttr);
				return TRUE;
			}
		}

		return FALSE;
	}

	void CXmdxElementReader::_lookupAttr(int index, LPXMDXATTR findAttr)
	{
		LPCSTR namePtr = NULL, valuePtr = NULL;
		_lookupAttr(index, &namePtr, &valuePtr);

		// 加载属性值
		LPXMDXATTRHEADER attrHeader = &_attrHeadEntry[index];
		strcpy_s(findAttr->name, MAX_PATH, namePtr);
		findAttr->type = attrHeader->type;

		switch (attrHeader->type)
		{
		case XMDXATTR_TEXT:
			findAttr->textVal = (LPSTR)valuePtr;
			break;
		case XMDXATTR_TEXTW:
			findAttr->wTextVal = (LPWSTR)valuePtr;
			break;
		case XMDXATTR_DWORD:
			findAttr->longVal = *((DWORD*)valuePtr);
			break;
		case XMDXATTR_FLOAT:
			findAttr->floatVal = *((DOUBLE*)valuePtr);
			break;
		case XMDXATTR_WORD:
			findAttr->intVal = *((WORD*)valuePtr);
			break;
		case XMDXATTR_BYTE:
			findAttr->byteVal = *((BYTE*)valuePtr);
			break;
		case XMDXATTR_BUFFER:
			findAttr->bufferVal = (BYTE*)valuePtr;
			break;
		}
	}
}