//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           XmdxDefines.H
//    @author         fanHong
//    @date           2017/11/3 11:21
//    @brief          XMDX访问类
//					  节点名、节点值、属性名只支持使用ANSI字符集
//					  字符串类型的属性值同时支持ANSI和Unicode，根据设置属性值时传入的参数类型自动判断
//
//*****************************************************

#pragma once

#include "XmdxDefines.h"
#include "Type/Any.h"
#include "XmdxDocument.h"
#include "FileDataStream.h"
#include "Type/KNode.h"

namespace CommBase
{
	/**
	*  @class    XMD节点写入器，自动管理释放
	*
	*  @brief
	*/
	class COMMBASE_API CXmdxElmWriter
	{
	public:
		CXmdxElmWriter();

		CXmdxElmWriter(IXmdxElementWriter* pWriter);

		CXmdxElmWriter(CXmdxElmWriter& pElmWriter);

		~CXmdxElmWriter();

		CXmdxElmWriter& operator =(CXmdxElmWriter& pElmWriter);

		IXmdxElementWriter* Get();

		// 用于传入子节点时调用
		IXmdxElementWriter* Detach();

		// 新建子节点
		CXmdxElmWriter NewElement(LPCSTR name = NULL);

		// 读写元素名称
		LPCSTR GetName();
		void SetName(LPCSTR name);

		// 读写元素值
		LPCSTR GetValue();
		void SetValue(LPCSTR value);

		// 设置属性值
		bool AddTextAttr(LPCSTR name, LPCSTR value);	// XMDXATTR_TEXT
		bool AddTextAttr(LPCSTR name, LPCWSTR value);	// XMDXATTR_TEXTW
		bool AddDoubleAttr(LPCSTR name, double value);	// XMDXATTR_FLOAT
		bool AddIntAttr(LPCSTR name, int value);		// XMDXATTR_DWORD
		bool AddShortAttr(LPCSTR name, short value);	// XMDXATTR_WORD
		bool AddByteAttr(LPCSTR name, BYTE value);		// XMDXATTR_BYTE
		bool AddAttrBuffer(LPCSTR name, BYTE* value, int length);	// XMDXATTR_BUFFER
		bool AddVariantAttr(LPCSTR name, const CAny& varData);
		bool AddGuidAttr(LPCSTR name, const CGuid& guid);

	public:
		void Submit();

	protected:
		IXmdxElementWriter*	_element;
	};

	//XMD节点读取器，自动管理释放
	class COMMBASE_API CXmdxElmReader
	{
	public:
		CXmdxElmReader();
		CXmdxElmReader(IXmdxElementReader* pReader);
		CXmdxElmReader(CXmdxElmReader& elmReader);
		CXmdxElmReader(CXmdxDocumentReader& docReader);

		~CXmdxElmReader();

		CXmdxElmReader& operator =(CXmdxElmReader& pElmReader);
		IXmdxElementReader* Get();

		// 用于传入子节点时调用
		IXmdxElementReader* Detach();
		bool IsNone();

		// 查找子元素, 没有查找到使用IsNone判断
		CXmdxElmReader FindChildElement(LPCSTR name);

		// 读元素名称
		LPCSTR GetName();

		// 读元素值
		LPCSTR GetValue();

		// 读元素属性值
		int GetAttrCount();
		LPCSTR GetAttrName(int index);
		BYTE GetAttrType(int index);

		// 获取属性索引，不存在时返回-1
		int GetAttrIndex(LPCSTR name);

		// 通过索引读取属性值，不进行类型转换
		LPCSTR GetTextAttrA(int index, LPCSTR defVal = "");		// XMDXATTR_TEXT
		LPCWSTR GetTextAttrW(int index, LPCWSTR defVal = L"");		// XMDXATTR_TEXTW
		double GetDoubleAttr(int index, double defVal = 0.0);		// XMDXATTR_FLOAT
		int GetIntAttr(int index, DWORD defVal = 0);				// XMDXATTR_DWORD
		short GetShortAttr(int index, short defVal = 0);			// XMDXATTR_WORD
		BYTE GetByteAttr(int index, BYTE defVal = 0);				// XMDXATTR_BYTE
		BYTE* GetAttrBuffer(int index, int &length, BYTE* defVal = 0);
		CGuid GetAttrGuid(int index, const CGuid& defVal = CGuid());
		CAny GetAttrVariant(int index, const CAny& defVal = CAny());

		// 通过属性名读取属性值，会自动进行类型转换
		CString GetTextAttr(LPCSTR name, const CString& defVal = _T(""));
		double GetDoubleAttr(LPCSTR name, double defVal = 0.0);
		int GetIntAttr(LPCSTR name, DWORD defVal = 0);
		short GetShortAttr(LPCSTR name, short defVal = 0);
		BYTE GetByteAttr(LPCSTR name, BYTE defVal = 0);
		BYTE* GetAttrBuffer(LPCSTR name, int &length, BYTE* defVal = 0);
		CGuid GetAttrGuid(LPCSTR name, const CGuid& defVal = CGuid());

	public:
		void Submit();

	protected:
		IXmdxElementReader* _element;
	};

	class COMMBASE_API CXmdxElmFind
	{
	public:
		CXmdxElmFind(CXmdxElmReader& parentReader);
		~CXmdxElmFind();

		bool FindNext();

		// 索引从1开始
		WORD NextIndex();
		CXmdxElmReader* operator->();

		// 脱离读指针，需要手动释放,调用此方法后，再调用->将无效
		IXmdxElementReader* DetachReader();

	protected:
		WORD _index;
		FINDHANDLE _handle;
		XMDXHANDLE _pParentElm;
		IXmdxDocumentReader* _pDocReader;
		CXmdxElmReader _xmdReader;
	};

	class COMMBASE_API ILargXmdxSave
	{
	public:
		virtual ~ILargXmdxSave()
		{
		}

		/**
		*  @brief    取得节点
		*
		*  @return   IXmdxElementWriter* xmdx节点
		*			 在调用此方法时需要保证上一次的节点进行了submit
		*/
		virtual IXmdxElementWriter* GetWriter() = 0;

		/**
		*  @brief    关闭文档对象
		*
		*/
		virtual void CloseXmdxDocument() = 0;

		/**
		*  @brief    获取当前记录索引
		*
		*/
		virtual UINT CurRecordIndex() = 0;
	};

	/**
	*	@class	CLargXmdxFileSave
	*
	*	@brief 根据文件存储大小自动分割成多个小文件
	*		   所有数据是线形存储到每个小文件中的根节点下面
	*/
	class COMMBASE_API CLargXmdxFileSave : public ILargXmdxSave
	{
	public:
		/**
		*  @brief    构造函数
		*
		*/
		CLargXmdxFileSave(const CString& strFile);

		/**
		*  @brief    析构函数
		*
		*/
		~CLargXmdxFileSave();

	public:
		/**
		*  @brief    取得节点
		*
		*  @return   IXmdxElementWriter* xmdx节点
		*			 在调用此方法时需要保证上一次的节点进行了submit
		*/
		IXmdxElementWriter* GetWriter();

		/**
		*  @brief    关闭文档对象
		*
		*/
		void CloseXmdxDocument();

		/**
		*  @brief    获取当前记录索引
		*
		*/
		UINT CurRecordIndex();

	protected:
		/**
		*  @brief    创建文档对象
		*
		*/
		bool CreateXmdxDocument();

		/**
		*  @brief    清除分卷文件
		*
		*/
		void ClearVolumeFiles();

	protected:
		UINT					_nVulume;
		CString					_strDir;
		CString					_strBaseName;
		CString					_strExtName;
		CAtlFile				*_pFile;
		CFileDataWriteStream	*_pStream;
		CXmdxDocumentWriter		*_pXmdxDoc;
		UINT					_nRecordIdx;
	};

	/**
	*	@class	CLargXmdxMemSave
	*
	*/
	class COMMBASE_API CLargXmdxMemSave : public ILargXmdxSave
	{
	public:
		/**
		*  @brief    构造函数
		*
		*/
		CLargXmdxMemSave(const CString& strFile);

		/**
		*  @brief    析构函数
		*
		*/
		~CLargXmdxMemSave();

	public:
		/**
		*  @brief    取得节点
		*
		*  @return   IXmdxElementWriter* xmdx节点
		*			 在调用此方法时需要保证上一次的节点进行了submit
		*/
		IXmdxElementWriter* GetWriter();

		/**
		*  @brief    关闭文档对象
		*
		*/
		void CloseXmdxDocument();

		/**
		*  @brief    获取当前记录索引
		*
		*/
		UINT CurRecordIndex();

	protected:
		/**
		*  @brief    清除分卷文件
		*
		*/
		void ClearVolumeFiles();

		/**
		*  @brief    创建文档对象
		*
		*/
		bool CreateXmdxDocument();

	protected:
		UINT					_nVulume;
		CString					_strDir;
		CString					_strBaseName;
		CString					_strExtName;
		FileMWriteDataArray		_memDataAray;
		CXmdxDocumentWriter		*_pXmdxDoc;
		UINT					_nRecordIdx;
	};

	class COMMBASE_API ILargXmdxLoad
	{
	public:
		virtual ~ILargXmdxLoad()
		{
		}

		/**
		*  @brief    取得根节点
		*
		*  @return   IXmdxElementReader* xmdx节点
		*			 在调用此方法时需要保证上一次的节点进行了submit
		*/
		virtual IXmdxElementReader* GetReader() = 0;

		/**
		*  @brief    关闭文档对象
		*
		*/
		virtual void CloseXmdxDocument() = 0;
	};

	/**
	*	@class	CLargXmdxFileLoad
	*
	*	@brief 根据文件存储大小自动分割成多个小文件
	*		   所有数据是线形从到每个小文件中的根节点下面读取
	*/
	class COMMBASE_API CLargXmdxFileLoad : public ILargXmdxLoad
	{
	public:
		/**
		*  @brief    构造函数
		*
		*/
		CLargXmdxFileLoad(const CString& strFile);

		/**
		*  @brief    析构函数
		*
		*/
		~CLargXmdxFileLoad();

	public:
		/**
		*  @brief    取得根节点
		*
		*  @return   IXmdxElementReader* xmdx节点
		*			 在调用此方法时需要保证上一次的节点进行了submit
		*/
		IXmdxElementReader* GetReader();

		/**
		*  @brief    关闭文档对象
		*
		*/
		void CloseXmdxDocument();

	protected:
		/**
		*  @brief    打开文档对象
		*
		*/
		bool OpenXmdxDocument();

	protected:
		UINT					_nVulume;
		CString					_strDir;
		CString					_strBaseName;
		CString					_strExtName;
		CAtlFile				*_pFile;
		CFileDataReadStream		*_pStream;
		CXmdxDocumentReader		*_pXmdxDoc;
		FINDHANDLE				_pXmdxFind;
	};

	/**
	*	@class	CLargXmdxMemLoad
	*
	*/
	class COMMBASE_API CLargXmdxMemLoad : public ILargXmdxLoad
	{
	public:
		/**
		*  @brief    构造函数
		*
		*/
		CLargXmdxMemLoad(const CString& strFile);

		/**
		*  @brief    析构函数
		*
		*/
		~CLargXmdxMemLoad();

	public:
		/**
		*  @brief    取得根节点
		*
		*  @return   IXmdxElementReader* xmdx节点
		*			 在调用此方法时需要保证上一次的节点进行了submit
		*/
		IXmdxElementReader* GetReader();

		/**
		*  @brief    关闭文档对象
		*
		*/
		void CloseXmdxDocument();

	protected:
		/**
		*  @brief    打开文档对象
		*
		*/
		bool OpenXmdxDocument();

	protected:
		UINT					_nVulume;
		CString					_strDir;
		CString					_strBaseName;
		CString					_strExtName;
		FileMReadDataArray		_memDataAray;
		CXmdxDocumentReader		*_pXmdxDoc;
		FINDHANDLE				_pXmdxFind;
	};
}