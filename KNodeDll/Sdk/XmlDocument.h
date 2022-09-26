//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           XmlDocument.h
//    @author         fanHong
//    @date           2017/11/3 11:16
//    @brief          对XML文档的操作
//
//*****************************************************

#pragma once

#include "IDataStream.h"
#include <atlcoll.h>

namespace CommBase
{
	// XML树节点类型
	enum xml_node_type
	{
		node_null,			// 空 (null) 节点句柄
		node_document,		// XML文档树的根节点
		node_element,		// 元素节点, 如 '<node/>'
		node_cdata,			// Character data, i.e. '<![CDATA[text]]>'
		node_comment,		// XML注释, 如 '<!-- text -->'
		node_pi,			// Processing instruction, i.e. '<?name?>'
		node_declaration,	// 文档声明, 如 '<?xml version="1.0"?>'
		node_doctype		// Document type declaration, i.e. '<!DOCTYPE doc>'
	};

	class COMMBASE_API CXmlAttribute;
	class COMMBASE_API CXmlElement;
	class COMMBASE_API CXmlDocument;

	//! 结点/属性容器
	template <class T, class CONTENT>
	class CXmlContainer
	{
	public:
		virtual ~CXmlContainer(void);

		/**
		* 结点/属性个数
		* @return 返回结点/属性的个数
		*/
		size_t GetCount(void);

		/**
		* 获取结点/属性对象
		* @param [in] index	结点/属性的索引
		* @return 返回结点/属性对象
		*/
		CONTENT* GetAt(size_t index);

		/**
		* 增加结点/属性
		* @return 返回增加后的类对象
		*/
		CONTENT* Add(void);

		/**
		* 移除结点/属性
		* @param [in] index	结点/属性的索引
		*/
		void RemoveAt(size_t index);

		/**
		* 移除所有结点/属性
		*/
		void RemoveAll(void);

		/**
		* 清空重置结点/属性容器
		*/
		void reset(void);
		CONTENT* InsertAt(size_t index);
	protected:

		/**
		* 插入结点/属性
		* @param [in] index		插入的位置
		* @param [in] content	结点/属性对象
		* @return 结点/属性对象
		*/
		CONTENT* InsertAt(size_t index, CONTENT* content);
	protected:
		/** 结点/属性容器对象 */
		CAtlArray<CONTENT*>		_array;
		CXmlElement*			_parent;
	};

	template <class T, class CONTENT>
	CXmlContainer<T, CONTENT>::~CXmlContainer(void)
	{
		T *pT = static_cast<T *>(this);
		pT->RemoveAll();
	}
	template <class T, class CONTENT>
	size_t CXmlContainer<T, CONTENT>::GetCount(void)
	{
		return _array.GetCount();
	}
	template <class T, class CONTENT>
	CONTENT* CXmlContainer<T, CONTENT>::GetAt(size_t index)
	{
		return _array.GetAt(index);
	}
	template <class T, class CONTENT>
	CONTENT* CXmlContainer<T, CONTENT>::Add(void)
	{
		T *pT = static_cast<T *>(this);
		CONTENT* content = pT->InsertAt(_array.GetCount());
		return content;
	}
	template <class T, class CONTENT>
	void CXmlContainer<T, CONTENT>::RemoveAt(size_t index)
	{
		ATLASSERT(FALSE);
	}
	template <class T, class CONTENT>
	void CXmlContainer<T, CONTENT>::RemoveAll(void)
	{
		T *pT = static_cast<T *>(this);
		for (int i = (int)_array.GetCount() - 1; i >= 0; i--)
			pT->RemoveAt(i);
	}
	template <class T, class CONTENT>
	void CXmlContainer<T, CONTENT>::reset(void)
	{
		RemoveAll();
	}
	template <class T, class CONTENT>
	CONTENT* CXmlContainer<T, CONTENT>::InsertAt(size_t index)
	{
		ATLASSERT(FALSE);
		return NULL;
	}
	template <class T, class CONTENT>
	CONTENT* CXmlContainer<T, CONTENT>::InsertAt(size_t index, CONTENT* content)
	{
		_array.InsertAt(index, content);
		content->_parent = _parent;
		return content;
	}

	//! CXmlElements
	class COMMBASE_API CXmlElements : public CXmlContainer < CXmlElements, CXmlElement >
	{
		friend class CXmlElement;
	public:
		/**
		* 插入结点
		* @param [in] index		插入的位置
		* @return 结点对象
		*/
		CXmlElement* InsertAt(size_t index);

		/**
		* 移除结点
		* @param [in] index		结点索引
		*/
		void RemoveAt(size_t index);
	};

	//! CXmlAttributes
	class COMMBASE_API CXmlAttributes : public CXmlContainer < CXmlAttributes, CXmlAttribute >
	{
		friend class CXmlElement;
	public:

		/**
		* 插入属性
		* @param [in] index		插入的位置
		* @return 属性对象
		*/
		CXmlAttribute* InsertAt(size_t index);

		/**
		* 移除属性
		* @param [in] index		属性索引
		*/
		void RemoveAt(size_t index);
	};

	//! 结点属性
	class COMMBASE_API CXmlAttribute
	{
		friend class CXmlContainer < CXmlAttributes, CXmlAttribute >;
	public:
		CXmlAttribute(void);
		/**
		* 设置属性值
		* @param [in] value	int类型属性值
		*/
		void SetAttrValue(int value);

		/**
		* 设置属性值
		* @param [in] value	long类型属性值
		*/
		void SetAttrValue(long value);

		/**
		* 设置属性值
		* @param [in] value	double类型属性值
		*/
		void SetAttrValue(double value);

		/**
		* 设置属性值
		* @param [in] value	LPCTSTR类型属性值
		*/
		void SetAttrValue(LPCTSTR value);

		/**
		* 设置属性名称
		* @param [in] name	属性名称
		*/
		void SetAttrName(LPCTSTR name);

		/**
		* 获取属性值
		* @return 返回int类型属性值
		*/
		int GetIntValue(void);

		/**
		* 获取属性值
		* @return 返回long类型属性值
		*/
		long GetLngValue(void);

		/**
		* 获取属性值
		* @return 返回double类型属性值
		*/
		double GetDblValue(void);

		/**
		* 获取属性值
		* @return 返回LPCTSTR类型属性值
		*/
		LPCTSTR GetStrValue(void);

		/**
		* 获取属性值
		* @return 返回LPCTSTR类型名称
		*/
		LPCTSTR GetAttrName(void);

		/**
		* 重置属性信息
		*/
		void reset(void);

	protected:
		/** 属性名称 */
		LPTSTR _name;
		/** 属性值 */
		LPTSTR _value;

		CXmlElement *_parent;
	};

	//! CXmlElement
	class COMMBASE_API CXmlElement
	{
		friend class CXmlDocument;
		friend class CXmlContainer < CXmlElements, CXmlElement >;
		friend class CXmlElements;
		friend struct xml_parser;
	public:
		CXmlElement(void);
		virtual ~CXmlElement(void);

		/**
		* 取得子结点容器数量
		* @return	返回子结点容器数量
		*/
		int GetChildElementCount(void);

		/**
		* 取得子结点容器
		* @param [in] NotExistCreate	不存在的结点是否创建 （默认为true）
		* @return	返回子结点容器
		*/
		CXmlElements* GetChildElements(BOOL NotExistCreate = true);

		/**
		* 取得子结点
		* @param [in] name				结点名称
		* @param [in] NotExistCreate	不存在的结点是否创建（默认为true）
		* @return	返回子结点
		*/
		CXmlElement* GetChildElementAt(LPCTSTR name, BOOL NotExistCreate = true);

		/**
		* 取得属性容器数量
		* @return	返回属性容器数量
		*/
		int GetAttributeCount(void);

		/**
		* 取得属性容器
		* @param [in] NotExistCreate	不存在的属性是否创建（默认为true）
		* @return	返回属性容器
		*/
		CXmlAttributes* GetAttributes(BOOL NotExistCreate = true);

		/**
		* 取得属性
		* @param [in] name				属性名称
		* @param [in] NotExistCreate	不存在的属性是否创建（默认为true）
		* @return	返回属性
		*/
		CXmlAttribute* GetAttributeAt(LPCTSTR name, BOOL NotExistCreate = true);

		/**
		* 取得属性值
		* @return	返回属性值
		*/
		LPCTSTR GetElementText(void);

		/**
		* 设置属性值
		* @param [in] text				属性值
		*/
		void SetElementText(LPCTSTR text);

		/**
		* 取得属性名称
		* @return	返回属性名称
		*/
		LPCTSTR GetElementName(void);

		/**
		* 设置属性名称
		* @param [in] name				属性名称
		*/
		void SetElementName(LPCTSTR name);

		/**
		* 设置属性
		* @param [in] AttrName			属性名称
		* @param [in] AttrValue			属性值
		*/
		void SetAttrValue(LPCTSTR AttrName, LPCTSTR AttrValue);

		/**
		* 添加属性
		* @param [in] AttrName			属性名称
		* @param [in] AttrValue			属性默认值
		*/
		void AddAttrValue(LPCTSTR AttrName, LPCTSTR AttrValue);

		/**
		* 读取属性
		* @param [in] AttrName			属性名称
		* @param [in] DefaultValue		属性默认值（默认为空）
		*/
		LPCTSTR GetAttrValue(LPCTSTR AttrName, LPCTSTR DefaultValue = _T(""));

		/**
		* 查找属性
		* @param [in] AttrName			属性名称
		* @param [out] index			属性索引
		* @return	返回是否找到属性
		*/
		BOOL FindAttribute(LPCTSTR AttrName, int& index);

		/**
		* 复制结点
		* @param [in] root			复制的结点
		*/
		void CopyFrom(CXmlElement* root);

		/**
		* 重置所有数据
		*/
		void reset(void);

		/**
		* 获取当前节点是否为声明结点
		* @return	返回当前节点是否为声明结点
		* - TRUE 是， FALSE 否
		*/
		BOOL GetIsDeclaration()
		{
			return _Type == node_declaration;
		};

		/**
		* 设置当前节点是否为声明结点
		* @param [in] bDeclaration		是否为声明结点（默认为false）
		*/
		void SetIsDeclaration(BOOL bDeclaration = false);

		/**
		* 获取文本两端是否增加双引号
		* @return	返回文本两端是否增加双引号
		* - TRUE 是， FALSE 否
		*/
		BOOL GetIsAddDoubleQuot()
		{
			return _IsAddDoubleQuot;
		}

		/**
		* 设置文本两端是否增加双引号
		* @param [in] bAdd		是否增加双引号（默认为true）
		*/
		void SetIsAddDoubleQuot(BOOL bAdd = true)
		{
			_IsAddDoubleQuot = bAdd;
		}

		/**
		* 取得 XML 字符串
		* @param [out] xml		XML字符串
		* @param [in] error
		* @return	返回取得 XML 字符串是否成功
		*/
		BOOL GetXmlString(CString& xml, CString* error = NULL);

		xml_node_type GetType()
		{
			return _Type;
		}
		void SetType(xml_node_type type)
		{
			_Type = type;
		}

		CXmlElement *GetParent()
		{
			return _parent;
		}

		void * GetAllocator()
		{
			return _alloc;
		}
	protected:

		/**
		* 为字符串增加双引号
		* @param [in] str	字符串
		* @return	返回增加双引号后的字符串
		*/
		CString AddDoubleQuot(LPCTSTR str);

		/**
		* 删除字符串双引号
		* @param [in] str	字符串
		* @return	返回删除双引号后的字符串
		*/
		CString DelDoubleQuot(LPCTSTR str);

		/**
		* 把二进制XML文件转换成Element
		* @param [in] pStream	数据流读取对象
		* @return	返回TRUE
		*/
		BOOL ParseXmdElement(IDataReadStream *pStream);

		/**
		* 把Element转换成二进度XML文件
		* @param [in] pStream	数据流写入对象
		* @param [in] dwFlags 格式标记 XMD_FLAGS_SAVEFORMAT_UNICODE 和 XMD_FLAGS_SAVEFORMAT_ANSI
		* @return	返回TRUE
		*/
		BOOL CreateXmdElement(IDataWriteStream *pStream, DWORD dwFlags);

	protected:
		/** 结点操作对象 */
		CXmlElements*		_ElementContainer;
		/** 属性操作对象 */
		CXmlAttributes*		_AttributeContainer;

		/** 结点内容 */
		LPTSTR				_ElementText;
		/** 结点名称 */
		LPTSTR				_ElementName;

		/** _ElementText保存时是否自动添加双引号 */
		BOOL				_IsAddDoubleQuot;

		CXmlElement*		_parent;

		xml_node_type		_Type;

		void*				_alloc;
	};

	//--------------------------------------------------------
	// CXmlDocument
	//--------------------------------------------------------

	/** 定义文档格式枚举类型　*/
	enum DocFormatEnum
	{
		fmtXML,	/**< XML 文本格式 */
		fmtXMD,	/**< XML 二进制格式 */
		fmtXMLUTF8, /**< UTF-8类型的文本格式(目前只支持读取UTF-8类型，不支持保存成此格式 */
		fmtXMLANSI,		/**< 将XML文件保存成ANSI格式*/
		fmtXMDANSI,		/**< XML 二进制格式(字符用ANSI格式) */
	};

	// 保存成UNICODE格式（<XMD 1.0>默认格式）
	#define XMD_FLAGS_SAVEFORMAT_UNICODE			0x00
	// 保存成ANSI格式
	#define XMD_FLAGS_SAVEFORMAT_ANSI				0x01

	//! 文档处理
	class COMMBASE_API CXmlDocument
	{
		friend struct xml_parser;
	private:
		CXmlDocument(CXmlDocument&);
	public:
		CXmlDocument(void);
		~CXmlDocument(void);

		/**
		* 加载 XML 文件
		* @param [in] XmlFile			XML文件
		* @param [in] format			XML格式枚举
		* @return	返回加载 XML 文件是否成功
		* - TRUE 加载成功， FALSE 加载失败
		*/
		BOOL LoadFile(LPCTSTR XmlFile, DocFormatEnum format);

		/**
		* 加载 XML 文件
		* @param [in] FileHandle		XML文件句柄
		* @return	返回加载 XML 文件是否成功
		* - TRUE 加载成功， FALSE 加载失败
		*/
		BOOL LoadXmlFile(HANDLE FileHandle);
		/**
		* 加载 XMD 文件
		* @param [in] FileHandle		XMD文件句柄
		* @return	返回加载 XML 文件是否成功
		* - TRUE 加载成功， FALSE 加载失败
		*/
		BOOL LoadXmdFile(HANDLE FileHandle);

		/**
		* 加载 UTF-8码文件
		* @param [in] FileHandle		UTF-8码文件句柄
		* @return	返回加载 XML 文件是否成功
		* - TRUE 加载成功， FALSE 加载失败
		*/
		BOOL LoadXmlUtf8File(HANDLE FileHandle);

		/**
		* 保存 XML 文件
		* @param [in] XmlFile			XML文件
		* @param [in] format			XML格式枚举
		* @return	返回保存 XML 文件是否成功
		* - TRUE 保存成功， FALSE 保存失败
		*/
		BOOL SaveFile(LPCTSTR XmlFile, DocFormatEnum format);

		/**
		* 保存 XML 文件
		* @param [in] FileHandle		XML文件句柄
		* @return	返回保存 XML 文件是否成功
		* - TRUE 保存成功， FALSE 保存失败
		*/
		BOOL SaveXmlFile(HANDLE FileHandle);
		/**
		* 保存 XMD 文件
		* @param [in] FileHandle		XMD文件句柄
		* @param [in] dwFlags		格式标记 XMD_FLAGS_SAVEFORMAT_UNICODE 和 XMD_FLAGS_SAVEFORMAT_ANSI
		* @return	返回保存 XMD 文件是否成功
		* - TRUE 保存成功， FALSE 保存失败
		*/
		BOOL SaveXmdFile(HANDLE FileHandle, DWORD dwFlags = XMD_FLAGS_SAVEFORMAT_UNICODE);

		/**
		* 保存 XML ANSI码文件
		* @param [in] FileHandle		XML文件句柄
		* @return	返回保存 XML 文件是否成功
		*/
		BOOL SaveXmlANSIFile(HANDLE FileHandle);

		/**
		* 保存 XML UTF-8码文件
		* @param [in] FileHandle		XML文件句柄
		* @return	返回保存 XML 文件是否成功
		* - TRUE 保存成功， FALSE 保存失败
		*/
		BOOL SaveXmlUtf8File(HANDLE FileHandle);

		/**
		* 加载 XML 字符串
		* @param [in] xml		XML字符串
		* @param [in] error		错误对象结构（默认为NULL）
		* @return	返回加载 XML 字符串是否成功
		* - TRUE 加载成功， FALSE 加载失败
		*/
		BOOL SetXmlString(LPCTSTR xml);

		/**
		* 取得 XML 字符串
		* @param [out] xml		XML字符串
		* @param [in] error		错误对象结构（默认为NULL）
		* @return	返回取得 XML 字符串是否成功
		* - TRUE 获取成功， FALSE 获取失败
		*/
		BOOL GetXmlString(CString& xml, CString* error = NULL);

		/**
		* 从流中加载XMD数据
		* @param [in] pDataStream	数据流读取对象
		* @return	返回加载XMD数据是否成功
		* - TRUE 获取成功， FALSE 获取失败
		*/
		BOOL LoadXmdFromStream(IDataReadStream *pDataStream);

		/**
		* 保存XMD数据到流对象中
		* @param [in] pDataStream	数据流写入对象
		* @param [in] dwFlags	格式标记 XMD_FLAGS_SAVEFORMAT_UNICODE 和 XMD_FLAGS_SAVEFORMAT_ANSI
		* @return	返回保存XMD数据是否成功
		* - TRUE 保存成功， FALSE 保存失败
		*/
		BOOL SaveXmdToStream(IDataWriteStream *pDataStream, DWORD dwFlags = XMD_FLAGS_SAVEFORMAT_UNICODE);

		/**
		* 取得 XML 根结点
		* @return	返回XML 根结点
		*/
		CXmlElement* GetElementRoot(void);

		CXmlElement* GetDocumentElement(void);

		/**
		* 删除数据
		*/
		void Clear();

		/**
		* 取得最近一次错误信息
		* @return	返回最近一次错误信息
		*/
		LPCTSTR GetLastError(void);

		/**
		* 保存时是否格式化文本
		* @return	返回是否格式化文本
		*/
		BOOL GetIsFormatSave(void);

		/**
		* 设置是否格式化保存文本
		* @param [in] bFormat	是否格式化保存文本（默认为true）
		*/
		void SetIsFormatSave(BOOL bFormat = true);

		void *alloc;
	protected:

		bool load_buffer_impl(void* contents, size_t size, unsigned int options, int encode, bool is_mutable, bool own);

		CXmlElement		*_Doc_node;

		/** 根节点 */
		CXmlElement		*_ElementRoot;

		/** 最后一次错误文本 */
		CString			_LastError;
		/** 保存时是否格式化文本 (能提供比较好的可读性) */
		DWORD			_Option;
	};
}