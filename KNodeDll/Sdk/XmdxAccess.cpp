#include "StdAfx.h"
#include "XmdxAccess.h"
#include "../Tool/FileTool.h"
#include "../Type/Assert.h"

namespace CommBase
{
	CXmdxElmWriter::CXmdxElmWriter()
	{
		_element = NULL;
	}

	CXmdxElmWriter::CXmdxElmWriter(IXmdxElementWriter* pWriter)
	{
		_element = pWriter;
	}

	CXmdxElmWriter::CXmdxElmWriter(CXmdxElmWriter& pElmWriter)
	{
		Submit();
		_element = pElmWriter.Detach();
	}

	CXmdxElmWriter::~CXmdxElmWriter()
	{
		Submit();
	}

	CXmdxElmWriter& CXmdxElmWriter::operator =(CXmdxElmWriter& pElmWriter)
	{
		if (this == &pElmWriter)
			return *this;

		Submit();
		_element = pElmWriter.Detach();
		return *this;
	}

	IXmdxElementWriter* CXmdxElmWriter::Get()
	{
		return _element;
	}

	IXmdxElementWriter* CXmdxElmWriter::Detach()
	{
		IXmdxElementWriter* pTmp = _element;
		_element = NULL;
		return pTmp;
	}

	CXmdxElmWriter CXmdxElmWriter::NewElement(LPCSTR name)
	{
		if (_element)
		{
			return _element->getWriter()->newElement(_element, name);
		}

		return CXmdxElmWriter();
	}

	LPCSTR CXmdxElmWriter::GetName()
	{
		if (_element)
			return _element->getName();
		return "";
	}
	void CXmdxElmWriter::SetName(LPCSTR value)
	{
		if (_element)
			return _element->setName(value);
	}

	LPCSTR CXmdxElmWriter::GetValue()
	{
		if (_element)
		{
			size_t size = 0;
			LPVOID pResult = _element->getValue(&size);
			return (LPCSTR)pResult;
		}
		return "";
	}
	void CXmdxElmWriter::SetValue(LPCSTR value)
	{
		if (_element)
		{
			return _element->setValue(value, strlen(value) + 1);
		}
	}

	bool CXmdxElmWriter::AddTextAttr(LPCSTR name, LPCSTR value)
	{
		if (_element)
		{
			return _element->addAttrText(name, value) != 0;
		}

		return false;
	}

	bool CXmdxElmWriter::AddTextAttr(LPCSTR name, LPCWSTR value)
	{
		if (_element)
		{
			return _element->addAttrText(name, value) != 0;
		}

		return false;
	}

	bool CXmdxElmWriter::AddDoubleAttr(LPCSTR name, double value)
	{
		if (_element)
		{
			return _element->addAttrDouble(name, value) != 0;
		}

		return false;
	}

	bool CXmdxElmWriter::AddIntAttr(LPCSTR name, int value)
	{
		if (_element)
		{
			return _element->addAttrLong(name, value) != 0;
		}

		return false;
	}

	bool CXmdxElmWriter::AddShortAttr(LPCSTR name, short value)
	{
		if (_element)
		{
			return _element->addAttrShort(name, value) != 0;
		}

		return false;
	}

	bool CXmdxElmWriter::AddByteAttr(LPCSTR name, byte value)
	{
		if (_element)
		{
			return _element->addAttrByte(name, value) != 0;
		}

		return false;
	}

	bool CXmdxElmWriter::AddAttrBuffer(LPCSTR name, BYTE* value, int length)
	{
		if (_element)
		{
			return _element->addAttrBuffer(name, value, length) != 0;
		}

		return false;
	}

	bool CXmdxElmWriter::AddGuidAttr(LPCSTR name, const CGuid& guid)
	{
		return AddVariantAttr(name, guid);
	}

	bool CXmdxElmWriter::AddVariantAttr(LPCSTR name, const CAny& varData)
	{
		bool bResult = true;
		EAnyType vdType = varData.GetType();

		switch (vdType)
		{
		case ANY_GUID:
		{
			WORD bufType = ANY_GUID;
			GUID guid = varData;

			// 逐个计算防止字节对齐问题
			size_t nSizeType = sizeof(bufType);
			size_t nSizeData1 = sizeof(guid.Data1);
			size_t nSizeData2 = sizeof(guid.Data2);
			size_t nSizeData3 = sizeof(guid.Data3);
			size_t nSizeData4 = sizeof(guid.Data4);

			int nLength = (int)(nSizeType + nSizeData1 +
				nSizeData2 + nSizeData3 + nSizeData4);

			BYTE pBuffer[128] = { 0 };
			BYTE* pWriteBuffer = pBuffer;

			memcpy(pWriteBuffer, &bufType, nSizeType);
			pWriteBuffer += nSizeType;
			memcpy(pWriteBuffer, &guid.Data1, nSizeData1);
			pWriteBuffer += nSizeData1;
			memcpy(pWriteBuffer, &guid.Data2, nSizeData2);
			pWriteBuffer += nSizeData2;
			memcpy(pWriteBuffer, &guid.Data3, nSizeData3);
			pWriteBuffer += nSizeData3;
			memcpy(pWriteBuffer, &guid.Data4, nSizeData4);

			AddAttrBuffer(name, pBuffer, nLength);
		}
		break;
		case ANY_STREAM:
		{
			BYTE* pStream = NULL;
			int nLen = varData.GetStreamValue(pStream);

			if (pStream == NULL)
				break;

			// 此指针是从内部返回，前4个字符用于内部记录大小
			// 在这里借用一下前2个字节，减少一次内存拷贝
			pStream -= sizeof(WORD);

			// 记录原值
			WORD oldValue = 0;
			memcpy(&oldValue, pStream, sizeof(WORD));

			// 写入新值
			WORD newValue = ANY_STREAM;
			memcpy(pStream, &newValue, sizeof(WORD));

			AddAttrBuffer(name, pStream, nLen + sizeof(WORD));

			// 还原旧值
			memcpy(pStream, &oldValue, sizeof(WORD));
		}
		break;
		case ANY_INT64:
		{
			WORD bufType = ANY_INT64;

			INT64 value = varData;
			size_t nSizeType = sizeof(bufType);

			int nLength = (int)(nSizeType + sizeof(value));

			BYTE pBuffer[100] = { 0 };
			BYTE* pWriteBuffer = pBuffer;

			memcpy(pWriteBuffer, &bufType, nSizeType);
			pWriteBuffer += nSizeType;
			memcpy(pWriteBuffer, &value, sizeof(value));

			AddAttrBuffer(name, pBuffer, nLength);
		}
		break;
		case ANY_BOOL:
			AddByteAttr(name, (bool)varData ? 1 : 0);
			break;
		case ANY_INT:
			AddIntAttr(name, (int)varData);
			break;
		case ANY_DOUBLE:
			AddDoubleAttr(name, (double)varData);
			break;
		case ANY_STRING:
			AddTextAttr(name, (LPCWSTR)varData);
			break;
		default:
			bResult = false;
			break;
		}

		return bResult;
	}

	void CXmdxElmWriter::Submit()
	{
		// 不释放根节点,DOC内部会释放
		if (_element && _element == _element->getWriter()->rootElement())
		{
			_element = NULL;
		}

		if (_element)
		{
			_element->getWriter()->writeElement(_element);
			_element->getWriter()->freeElement(_element);
			_element = NULL;
		}
	}

	//******************** CXmdxElmReader ********************

	CXmdxElmReader::CXmdxElmReader()
	{
		_element = NULL;
	}

	CXmdxElmReader::CXmdxElmReader(IXmdxElementReader* pReader)
	{
		_element = pReader;
	}

	CXmdxElmReader::CXmdxElmReader(CXmdxElmReader& elmReader)
	{
		Submit();
		_element = elmReader.Detach();
	}

	CXmdxElmReader::CXmdxElmReader(CXmdxDocumentReader& docReader)
	{
		_element = docReader.readElement(docReader.rootNode());
	}

	CXmdxElmReader::~CXmdxElmReader()
	{
		Submit();
	}

	CXmdxElmReader& CXmdxElmReader::operator = (CXmdxElmReader& pElmReader)
	{
		if (this == &pElmReader)
			return *this;

		Submit();
		_element = pElmReader.Detach();
		return *this;
	}

	IXmdxElementReader* CXmdxElmReader::Get()
	{
		return _element;
	}

	IXmdxElementReader* CXmdxElmReader::Detach()
	{
		IXmdxElementReader* pTmp = _element;
		_element = NULL;
		return pTmp;
	}

	bool CXmdxElmReader::IsNone()
	{
		return (_element == NULL);
	}

	CXmdxElmReader CXmdxElmReader::FindChildElement(LPCSTR name)
	{
		for (CXmdxElmFind find(*this); find.FindNext();)
		{
			if (!strcmp(name, find->GetName()))
				return find.DetachReader();
		}

		return CXmdxElmReader();
	}

	LPCSTR CXmdxElmReader::GetName()
	{
		if (_element)
		{
			return _element->getName();
		}
		return "";
	}

	LPCSTR CXmdxElmReader::GetValue()
	{
		if (_element)
		{
			return _element->getValue();
		}
		return "";
	}

	int CXmdxElmReader::GetAttrCount()
	{
		if (_element)
		{
			return int(_element->getAttrCount());
		}
		return 0;
	}

	BYTE CXmdxElmReader::GetAttrType(int index)
	{
		if (_element)
		{
			return _element->getAttrType(index);
		}
		return XMDXATTR_TEXT;
	}

	LPCSTR CXmdxElmReader::GetAttrName(int index)
	{
		if (_element)
		{
			return _element->getAttrName(index);
		}
		return "";
	}

	int CXmdxElmReader::GetAttrIndex(LPCSTR name)
	{
		if (!_element)
			return -1;

		for (size_t i = 0; i < _element->getAttrCount(); i++)
		{
			if (!strcmp(_element->getAttrName(int(i)), name))
			{
				return (int)i;
			}
		}
		return -1;
	}

	LPCSTR CXmdxElmReader::GetTextAttrA(int index, LPCSTR defVal /*= ""*/)
	{
		if (!_element)
			return defVal;

		XMDXATTR attr;
		if (_element->getAttrValue(index, &attr))
		{
			if (attr.type == XMDXATTR_TEXT)
				return attr.textVal;
		}
		return defVal;
	}

	LPCWSTR CXmdxElmReader::GetTextAttrW(int index, LPCWSTR defVal /*= L""*/)
	{
		if (!_element)
			return defVal;

		XMDXATTR attr;
		if (_element->getAttrValue(index, &attr))
		{
			if (attr.type == XMDXATTR_TEXTW)
				return attr.wTextVal;
		}
		return defVal;
	}

	double CXmdxElmReader::GetDoubleAttr(int index, double defVal /*= 0.0*/)
	{
		if (!_element)
			return defVal;

		XMDXATTR attr;
		if (_element->getAttrValue(index, &attr))
		{
			if (attr.type == XMDXATTR_FLOAT)
				return attr.floatVal;
		}
		return defVal;
	}

	int CXmdxElmReader::GetIntAttr(int index, DWORD defVal /*= 0*/)
	{
		if (!_element)
			return defVal;

		XMDXATTR attr;
		if (_element->getAttrValue(index, &attr))
		{
			if (attr.type == XMDXATTR_DWORD)
				return attr.longVal;
		}
		return defVal;
	}

	short CXmdxElmReader::GetShortAttr(int index, short defVal /*= 0*/)
	{
		if (!_element)
			return defVal;

		XMDXATTR attr;
		if (_element->getAttrValue(index, &attr))
		{
			if (attr.type == XMDXATTR_WORD)
				return attr.intVal;
		}
		return defVal;
	}

	byte CXmdxElmReader::GetByteAttr(int index, BYTE defVal /*= 0*/)
	{
		if (!_element)
			return defVal;

		XMDXATTR attr;
		if (_element->getAttrValue(index, &attr))
		{
			if (attr.type == XMDXATTR_BYTE)
				return attr.byteVal;
		}
		return defVal;
	}

	BYTE* CXmdxElmReader::GetAttrBuffer(int index, int &length, BYTE* defVal /*= 0*/)
	{
		if (!_element)
			return defVal;

		XMDXATTR attr;
		if (_element->getAttrValue(index, &attr))
		{
			if (attr.type == XMDXATTR_BUFFER)
			{
				length = *((int*)(attr.bufferVal));
				return attr.bufferVal + sizeof(int);
			}
		}
		return defVal;
	}

	CAny ParseBuffer(BYTE* pBuffer, int nLength, const CAny& defVal)
	{
		// 字节数不够
		if (NULL == pBuffer || nLength < 2)
			return defVal;

		// 读取两个字节的数据流类型
		WORD bufType = 0;
		memcpy(&bufType, pBuffer, sizeof(bufType));
		pBuffer += sizeof(bufType); // 跳过数据流类型
		nLength -= sizeof(bufType); // 跳过数据流类型大小 

		if (bufType == ANY_GUID)
		{
			GUID guid = { 0 };

			size_t nVarSize = sizeof(guid.Data1);
			memcpy(&guid.Data1, pBuffer, nVarSize);

			pBuffer += nVarSize;
			nVarSize = sizeof(guid.Data2);
			memcpy(&guid.Data2, pBuffer, nVarSize);

			pBuffer += nVarSize;
			nVarSize = sizeof(guid.Data3);
			memcpy(&guid.Data3, pBuffer, nVarSize);

			pBuffer += nVarSize;
			nVarSize = sizeof(guid.Data4);
			memcpy(&guid.Data4, pBuffer, nVarSize);

			return guid;
		}
		else if (bufType == ANY_STREAM)
		{
			return CAny(pBuffer, nLength);
		}
		else if (bufType == ANY_INT64)
		{
			INT64 value = 0;
			memcpy(&value, pBuffer, sizeof(value));
			return CAny(value);
		}

		return defVal;
	}

	CGuid CXmdxElmReader::GetAttrGuid(int index, const CGuid& defVal)
	{
		if (_element)
		{
			XMDXATTR attr;
			if (_element->getAttrValue(index, &attr))
			{
				if (attr.type == XMDXATTR_BUFFER)
				{
					int nLength = *((int*)(attr.bufferVal));
					return ParseBuffer(attr.bufferVal + sizeof(int), nLength, defVal);
				}
			}
		}
		return defVal;
	}

	CAny CXmdxElmReader::GetAttrVariant(int index, const CAny& defVal)
	{
		if (!_element)
			return defVal;

		XMDXATTR attr;
		if (!_element->getAttrValue(index, &attr))
			return defVal;

		switch (attr.type)
		{
		case XMDXATTR_BUFFER:
		{
			int nLength = *((int*)(attr.bufferVal));
			return ParseBuffer(attr.bufferVal + sizeof(int), nLength, defVal);
		}
		case XMDXATTR_FLOAT:
			return (double)attr.floatVal;
		case XMDXATTR_DWORD:
			return (int)attr.longVal;
		case XMDXATTR_BYTE:
			return (bool)(attr.byteVal != 0);
		case XMDXATTR_TEXTW:
			return (LPCWSTR)attr.wTextVal;
		}

		return defVal;
	}

	CGuid CXmdxElmReader::GetAttrGuid(LPCSTR name, const CGuid& defVal)
	{
		if (_element)
		{
			int length = 0;
			BYTE* pBuffer = _element->getAttrBuffer(name, length, NULL);
			return ParseBuffer(pBuffer, length, defVal);
		}

		return defVal;
	}

	BYTE* CXmdxElmReader::GetAttrBuffer(LPCSTR name, int &length, BYTE* defVal)
	{
		if (_element)
		{
			return _element->getAttrBuffer(name, length, defVal);
		}
		return defVal;
	}

	CString CXmdxElmReader::GetTextAttr(LPCSTR name, const CString& defVal)
	{
		if (!_element)
			return defVal;

		for (size_t i = 0; i < _element->getAttrCount(); i++)
		{
			XMDXATTR attr = { 0 };
			_element->getAttrValue(int(i), &attr);
			CString text;
			if (!strcmp(attr.name, name))
			{
				switch (attr.type)
				{
				case XMDXATTR_TEXTW:
					return attr.wTextVal;
					break;
				case XMDXATTR_TEXT:
					return CString(attr.textVal);
					break;
				case XMDXATTR_FLOAT:
					text.Format(_T("%f"), attr.floatVal);
					return text;
					break;
				case XMDXATTR_DWORD:
					text.Format(_T("%u"), (UINT)attr.longVal);
					return text;
					break;
				case XMDXATTR_WORD:
					text.Format(_T("%u"), attr.intVal);
					return text;
					break;
				case XMDXATTR_BYTE:
					text.Format(_T("%u"), attr.byteVal);
					return text;
					break;
				default:
					return defVal;
				}
			}
		}

		return defVal;
	}

	double CXmdxElmReader::GetDoubleAttr(LPCSTR name, double defVal)
	{
		if (_element)
		{
			return _element->getAttrDouble(name, defVal);
		}
		return defVal;
	}

	int CXmdxElmReader::GetIntAttr(LPCSTR name, DWORD defVal)
	{
		if (_element)
		{
			return _element->getAttrLong(name, defVal);
		}

		return defVal;
	}

	short CXmdxElmReader::GetShortAttr(LPCSTR name, short defVal)
	{
		if (_element)
		{
			return _element->getAttrShort(name, defVal);
		}
		return defVal;
	}

	byte CXmdxElmReader::GetByteAttr(LPCSTR name, byte defVal)
	{
		if (_element)
		{
			return _element->getAttrByte(name, defVal);
		}
		return defVal;
	}

	void CXmdxElmReader::Submit()
	{
		if (_element)
		{
			_element->getReader()->freeElement(_element);
			_element = NULL;
		}
	}

	/***************XMDX元素查找类********************/

	CXmdxElmFind::CXmdxElmFind(CXmdxElmReader& parentReader)
	{
		_index = 0;
		_handle = NULL;
		_pParentElm = NULL;
		_pDocReader = NULL;

		IXmdxElementReader* pReader = parentReader.Get();
		if (pReader)
		{
			_pParentElm = pReader->getNode();
			_pDocReader = pReader->getReader();
		}
	}

	CXmdxElmFind::~CXmdxElmFind()
	{
		if (_handle)
		{
			_pDocReader->closeFindHandle(_handle);
		}
	}

	bool CXmdxElmFind::FindNext()
	{
		if (!_pDocReader)
			return false;

		BOOL findOK = false;
		XMDXHANDLE findNode = NULL;
		if (!_handle)
		{
			_handle = _pDocReader->findFirstChild(_pParentElm, &findNode);
			findOK = (_handle != NULL);
		}
		else
		{
			findOK = _pDocReader->findNextChild(_handle, &findNode);
		}

		if (findOK)
		{
			_xmdReader = CXmdxElmReader(_pDocReader->readElement(findNode));
			_index++;
		}

		return (findOK != 0);
	}

	CXmdxElmReader* CXmdxElmFind::operator->()
	{
		return &_xmdReader;
	}

	IXmdxElementReader* CXmdxElmFind::DetachReader()
	{
		return _xmdReader.Detach();
	}

	WORD CXmdxElmFind::NextIndex()
	{
		return _index;
	}

	//////////////////////////////////////////////////////////////////////////////

	CLargXmdxFileSave::CLargXmdxFileSave(const CString& strFile)
	{
		_nVulume = 0;
		_pFile = NULL;
		_pStream = NULL;
		_pXmdxDoc = NULL;
		_nRecordIdx = 0;

		_strDir = CFileTool::GetFileDirectory(strFile);
		_strDir += L"\\";
		_strBaseName = CFileTool::GetFileBaseName(strFile);
		_strExtName = CFileTool::GetFileExtendName(strFile);

		// 清除分卷文件
		ClearVolumeFiles();
	}

	CLargXmdxFileSave::~CLargXmdxFileSave()
	{
		CloseXmdxDocument();
	}

	IXmdxElementWriter* CLargXmdxFileSave::GetWriter()
	{
		// 测试是否需要创建新的文档
		if (!CreateXmdxDocument())
			return NULL;

		if (_pXmdxDoc)
		{
			_nRecordIdx++;
			return _pXmdxDoc->newElement(_pXmdxDoc->rootElement(), "N");
		}

		return NULL;
	}

	void CLargXmdxFileSave::ClearVolumeFiles()
	{
		// 循环删除分卷文件
		int nVulume = _nVulume;
		do
		{
			CString strVolumeFile;

			if (0 != nVulume)
				strVolumeFile.Format(L"%s%s%d%s", _strDir, _strBaseName, nVulume, _strExtName);
			else
				strVolumeFile = _strDir + _strBaseName + _strExtName;

			nVulume++;

			if (!DeleteFile(strVolumeFile))
				break;

		} while (TRUE);
	}

	UINT CLargXmdxFileSave::CurRecordIndex()
	{
		return _nRecordIdx;
	}

	void CLargXmdxFileSave::CloseXmdxDocument()
	{
		if (NULL != _pXmdxDoc)
		{
			delete _pXmdxDoc;
			_pXmdxDoc = NULL;
		}

		if (NULL != _pStream)
		{
			delete _pStream;
			_pStream = NULL;
		}

		if (NULL != _pFile)
		{
			delete _pFile;
			_pFile = NULL;
		}
	}

	bool CLargXmdxFileSave::CreateXmdxDocument()
	{
		if (NULL != _pXmdxDoc && _pXmdxDoc->GetNodeCount() < 60000)
			return true;

		CloseXmdxDocument();

		CString strFile;
		if (0 != _nVulume)
			strFile.Format(L"%s%s%d%s", _strDir, _strBaseName, _nVulume, _strExtName);
		else
			strFile = _strDir + _strBaseName + _strExtName;

		_nVulume++;

		_pFile = new CAtlFile();
		if (_pFile->Create(strFile, GENERIC_WRITE, 0, CREATE_NEW ) != S_OK)
		{
			delete _pFile;
			_pFile = NULL;
			return false;
		}

		_pStream = new CFileDataWriteStream(*_pFile);
		_pXmdxDoc = new CXmdxDocumentWriter(_pStream);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////

	CLargXmdxFileLoad::CLargXmdxFileLoad(const CString& strFile)
	{
		_nVulume = 0;
		_pFile = NULL;
		_pStream = NULL;
		_pXmdxDoc = NULL;
		_pXmdxFind = NULL;

		_strDir = CFileTool::GetFileDirectory(strFile) + L"\\";
		_strBaseName = CFileTool::GetFileBaseName(strFile);
		_strExtName = CFileTool::GetFileExtendName(strFile);
	}

	CLargXmdxFileLoad::~CLargXmdxFileLoad()
	{
		CloseXmdxDocument();
	}

	IXmdxElementReader* CLargXmdxFileLoad::GetReader()
	{
		// 此处逻辑较为复杂，请不要随意修改
		int nRetry = 0;
		do
		{
			if (NULL != _pXmdxDoc)
			{
				BOOL bFound = FALSE;
				XMDXHANDLE findNode = NULL;

				if (NULL == _pXmdxFind)
				{
					_pXmdxFind = _pXmdxDoc->findFirstChild(_pXmdxDoc->rootNode(), &findNode);
					bFound = (NULL != _pXmdxFind);
				}
				else
				{
					bFound = _pXmdxDoc->findNextChild(_pXmdxFind, &findNode);
				}

				if (bFound)
					return _pXmdxDoc->readElement(findNode);
			}

			// 测试是否需要打开新的文档
			if (!OpenXmdxDocument())
				return (IXmdxElementReader*)-1;

		} while (nRetry++ < 1);

		return NULL;
	}

	void CLargXmdxFileLoad::CloseXmdxDocument()
	{
		if (NULL != _pXmdxFind)
		{
			_pXmdxDoc->closeFindHandle(_pXmdxFind);
			_pXmdxFind = NULL;
		}

		if (NULL != _pXmdxDoc)
		{
			delete _pXmdxDoc;
			_pXmdxDoc = NULL;
		}

		if (NULL != _pStream)
		{
			delete _pStream;
			_pStream = NULL;
		}

		if (NULL != _pFile)
		{
			delete _pFile;
			_pFile = NULL;
		}
	}

	bool CLargXmdxFileLoad::OpenXmdxDocument()
	{
		CloseXmdxDocument();

		CString strFile;
		if (0 != _nVulume)
			strFile.Format(L"%s%s%d%s", _strDir, _strBaseName, _nVulume, _strExtName);
		else
			strFile = _strDir + _strBaseName + _strExtName;

		// 已经读取完成所有分卷数据，正常退出
		if ((_nVulume != 0) && !CFileTool::FileExist(strFile))
			return true;

		_nVulume++;

		_pFile = new CAtlFile();
		if (_pFile->Create(strFile, GENERIC_READ, 0, OPEN_EXISTING ) != S_OK)
		{
			delete _pFile;
			_pFile = NULL;
			return false;
		}

		_pStream = new CFileDataReadStream(*_pFile);
		_pXmdxDoc = new CXmdxDocumentReader(_pStream);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////////

	CLargXmdxMemLoad::CLargXmdxMemLoad(const CString& strFile)
	{
		_nVulume = 0;
		_pXmdxDoc = NULL;
		_pXmdxFind = NULL;

		_strDir = CFileTool::GetFileDirectory(strFile);
		_strDir += L"\\";
		_strBaseName = CFileTool::GetFileBaseName(strFile);
		_strExtName = CFileTool::GetFileExtendName(strFile);
	}

	CLargXmdxMemLoad::~CLargXmdxMemLoad()
	{
		CloseXmdxDocument();
	}

	IXmdxElementReader* CLargXmdxMemLoad::GetReader()
	{
		// 此处逻辑较为复杂，请不要随意修改
		int nRetry = 0;
		do
		{
			if (NULL != _pXmdxDoc)
			{
				BOOL bFound = FALSE;
				XMDXHANDLE findNode = NULL;

				if (NULL == _pXmdxFind)
				{
					_pXmdxFind = _pXmdxDoc->findFirstChild(_pXmdxDoc->rootNode(), &findNode);
					bFound = (NULL != _pXmdxFind);
				}
				else
				{
					bFound = _pXmdxDoc->findNextChild(_pXmdxFind, &findNode);
				}

				if (bFound)
					return _pXmdxDoc->readElement(findNode);
			}

			// 测试是否需要打开新的文档
			if (!OpenXmdxDocument())
				return (IXmdxElementReader*)-1;

		} while (nRetry++ < 1);

		return NULL;
	}

	void CLargXmdxMemLoad::CloseXmdxDocument()
	{
		if (NULL != _pXmdxFind)
		{
			_pXmdxDoc->closeFindHandle(_pXmdxFind);
			_pXmdxFind = NULL;
		}

		if (NULL != _pXmdxDoc)
		{
			delete _pXmdxDoc;
			_pXmdxDoc = NULL;
		}

		for (FileMReadData& data : _memDataAray)
		{
			delete data.pFileDataStream;
		}
		_memDataAray.clear();
	}

	bool CLargXmdxMemLoad::OpenXmdxDocument()
	{
		CloseXmdxDocument();

		CString strFile;
		if (0 != _nVulume)
			strFile.Format(L"%s%s%d%s", _strDir, _strBaseName, _nVulume, _strExtName);
		else
			strFile = _strDir + _strBaseName + _strExtName;

		// 已经读取完成所有分卷数据，正常退出
		if ((_nVulume != 0) && !CFileTool::FileExist(strFile))
			return true;

		_nVulume++;

		FileMReadData mReadData;
		mReadData.strFileName = strFile;
		mReadData.pFileDataStream = new CMemoryDataReadStream(strFile);
		_memDataAray.push_back(mReadData);

		_pXmdxDoc = new CXmdxDocumentReader(mReadData.pFileDataStream);

		return true;
	}

	////////////////////////////////////////////////////////////////////////////

	CLargXmdxMemSave::CLargXmdxMemSave(const CString& strFile)
	{
		_nVulume = 0;
		_pXmdxDoc = NULL;
		_nRecordIdx = 0;

		_strDir = CFileTool::GetFileDirectory(strFile);
		_strDir += L"\\";
		_strBaseName = CFileTool::GetFileBaseName(strFile);
		_strExtName = CFileTool::GetFileExtendName(strFile);

		// 清除分卷文件
		ClearVolumeFiles();
	}

	CLargXmdxMemSave::~CLargXmdxMemSave()
	{
		CloseXmdxDocument();
	}

	IXmdxElementWriter* CLargXmdxMemSave::GetWriter()
	{
		// 测试是否需要创建新的文档
		if (!CreateXmdxDocument())
			return NULL;

		if (_pXmdxDoc)
		{
			_nRecordIdx++;
			return _pXmdxDoc->newElement(_pXmdxDoc->rootElement(), "N");
		}

		return NULL;
	}

	UINT CLargXmdxMemSave::CurRecordIndex()
	{
		return _nRecordIdx;
	}

	void CLargXmdxMemSave::CloseXmdxDocument()
	{
		if (NULL != _pXmdxDoc)
		{
			delete _pXmdxDoc;
			_pXmdxDoc = NULL;
		}

		// 遍历大文件内存片的内存写入到文件中
		for (FileMWriteData& data : _memDataAray)
		{
			if (!data.pFileDataStream->WriteToFile(data.strFileName))
			{
				KASSERT(L"LXMS保存数据失败,错误码:%d！", ::GetLastError());
				return;
			}
			delete data.pFileDataStream;
		}
		_memDataAray.clear();
	}

	bool CLargXmdxMemSave::CreateXmdxDocument()
	{
		if (NULL != _pXmdxDoc && _pXmdxDoc->GetNodeCount() < 60000)
			return true;

		CloseXmdxDocument();

		CString strFile;
		if (0 != _nVulume)
			strFile.Format(L"%s%s%d%s", _strDir, _strBaseName, _nVulume, _strExtName);
		else
			strFile = _strDir + _strBaseName + _strExtName;

		_nVulume++;

		FileMWriteData mWemData;
		mWemData.strFileName = strFile;
		mWemData.pFileDataStream = new CMemoryDataWriteStream(0x40000/*256K*/);
		_memDataAray.push_back(mWemData);

		_pXmdxDoc = new CXmdxDocumentWriter(mWemData.pFileDataStream);

		return true;
	}

	void CLargXmdxMemSave::ClearVolumeFiles()
	{
		// 循环删除分卷文件
		int nVulume = _nVulume;
		do
		{
			CString strVolumeFile;

			if (0 != nVulume)
				strVolumeFile.Format(L"%s%s%d%s", _strDir, _strBaseName, nVulume, _strExtName);
			else
				strVolumeFile = _strDir + _strBaseName + _strExtName;

			nVulume++;

			if (!DeleteFile(strVolumeFile))
				break;

		} while (TRUE);
	}
}