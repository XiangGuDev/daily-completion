//*****************************************************
//    
//    @copyright      变电三维设计组
//    @version        v4.0
//    @file           KNode.H
//    @author         JZL
//    @date           2017/12/7 16:01
//    @brief          树结构节点类
//
//*****************************************************

#pragma once
#include "Any.h"
#include "Guid.h"
#include <map>
#include <vector>

namespace CommBase
{
	class KNode;

	// 节点返回错误标记
	#define KPOS_TAIL (size_t(-1))	 // 队列最后面位置
	#define KPOS_HEAD (size_t(-2))	 // 队列最前面位置
	#define KNODE_ERROR (size_t(-3)) // KNODE操作错误
	#define KNODE_NONE_TAG (-1)		 // KNODE无标签

	// 子节点列表
	typedef std::vector<KNode*> KNodeList;
	typedef KNodeList::iterator KNodeListIter;
	typedef KNodeList::const_iterator KNodeListCIter;


	// 节点数据MAP
	typedef std::map<CString, CAny> KNodeDataMap;
	typedef KNodeDataMap::iterator KNodeDataMapIter;
	typedef KNodeDataMap::const_iterator KNodeDataMapCIter;

	typedef KNodeDataMap::reverse_iterator	KNodeDataMapRIter;

	// 节点MAP
	typedef std::map<CGuid, KNode*> KNodeMap;
	typedef KNodeMap::iterator KNodeMapIter;
	typedef KNodeMap::const_iterator KNodeMapCIter;

	/**
	*  @class    树型节点撤销接口类
	*
	*  @brief
	*/
	class COMMBASE_API KNodeUndo
	{
	public:
		virtual ~KNodeUndo() {}

	public:
		/**
		*  @class    数据删除
		*
		*  @param    KNode* pNode 删除数据的节点
		*  @param    LPCWSTR lpKey 数据名称
		*  @param    const CAny& oldValue 原数据值
		*/
		virtual void OnDataDelete(KNode* pNode, LPCWSTR lpKey, const CAny& oldValue) = 0;

		/**
		*  @class    数据修改
		*
		*  @param    KNode* pNode 修改数据的节点
		*  @param    LPCWSTR lpKey 数据名称
		*  @param    const CAny& oldValue 原数据值
		*  @param    const CAny& newValue 新数据值
		*/
		virtual void OnDataModify(KNode* pNode, LPCWSTR lpKey, const CAny& oldValue, const CAny& newValue) = 0;

		/**
		*  @class    标识修改
		*
		*  @param    KNode* pNode 修改数据的节点
		*  @param    int oldTag 原数据值
		*  @param    int newTag 新数据值
		*/
		virtual void OnTagModify(KNode* pNode, int oldTag, int newTag) = 0;

		/**
		*  @class    数据添加
		*
		*  @param    KNode* pNode 添加数据的节点
		*  @param    LPCWSTR lpKey 数据名称
		*  @param    const CAny& newValue 新数据值
		*/
		virtual void OnDataInsert(KNode* pNode, LPCWSTR lpKey, const CAny& newValue) = 0;

		/**
		*  @class    节点添加
		*
		*  @param    KNode* pParent 父亲节点
		*  @param    KNode* pNode 新插入节点
		*  @param    size_t nIndex 新插入节点索引
		*/
		virtual void OnNodeInsert(KNode* pParent, KNode* pNode, size_t nIndex) = 0;

		/**
		*  @class    节点交换
		*
		*  @param    KNode* pSrcParent 移动节点父亲
		*  @param    size_t nSrcIndex 移动节点索引
		*  @param    KNode* pDstParent 目的节点父亲
		*  @param    size_t nDstIndex 目的节点索引
		*/
		virtual void OnNodeSwap(KNode* pSrcParent, size_t nSrcIndex, KNode* pDstParent, size_t nDstIndex) = 0;

		/**
		*  @class    节点删除
		*
		*  @param    KNode* pParent 父亲节点
		*  @param    KNode* pNode 删除节点
		*  @param    size_t nIndex 删除节点索引
		*/
		virtual void OnNodeDelete(KNode* pParent, KNode* pNode, size_t nIndex) = 0;
	};

	/**
	*  @class    树型节点类
	*
	*  @brief
	*/
	class COMMBASE_API KNode
	{
	public:
		KNode();
		KNode(const CGuid& guid);
		KNode(const KNode& value);
		KNode& operator = (const KNode& value);

	public:
		virtual ~KNode();

	public:
		/**
		*  @brief    获取标签
		*
		*  @return   返回标签
		*/
		virtual int GetTag() const;

		/**
		*  @brief    设置标签
		*
		*  @param    int nTag 标签
		*/
		virtual void SetTag(int nTag);

		/**
		*  @brief    获取ID
		*
		*  @return   CGuid 返回Guid
		*/
		virtual const CGuid& GetId() const;

		/**
		*  @brief	设置ID
		*
		*  @param	const CGuid & id
		*  @return	void
		*/
		virtual void SetId(const CGuid& id);

	public:
		/**
		*  @brief    删除属性
		*
		*  @param    LPCWSTR lpKey 关键字
		*  @return   bool 存在返回true
		*/
		virtual bool RemoveData(LPCWSTR lpKey);

		/**
		*  @brief    删除所有属性
		*
		*/
		virtual void RemoveAllData();

		/**
		*  @brief    获取属性
		*
		*  @param    LPCWSTR lpKey 关键字
		*  @param    const CAny& defData 没有找到的默认值
		*								 (设置此参数后将默认添加到属性中)
		*  @return   const CAny& 返回属性对象,如果没有则为空对象
		*/
		virtual const CAny& GetData(LPCWSTR lpKey, const CAny& defData = CAny());

		/**
		*  @brief    设置属性
		*
		*  @param    LPCWSTR lpKey 关键字
		*  @param    cosnt CAny& data 数据
		*  @param    bool bAttach 是否把data的值直接转移(提高赋值性能)
		*  @remark	 如果不存关键字则创建对应属性
		*/
		virtual void SetData(LPCWSTR lpKey, const CAny& data, bool bAttach = false);

		/**
		*  @brief    是否存指定属性
		*
		*  @param    LPCWSTR lpKey 关键字
		*  @return   bool 存在返回true
		*/
		virtual bool HasData(LPCWSTR lpKey) const;

	public:
		/**
		*  @brief    从父节点中移除
		*
		*  @param    bool bDelete 是否删除自身
		*  @return   返回删除节点索引
		*  @remark	 当前节点内存需要自行释放
		*/
		virtual size_t RemoveFromParent(bool bDelete = true);

		/**
		*  @brief    获取父节点
		*
		*  @return   KNode* 返回父节点
		*/
		virtual KNode* GetParent() const;

		/**
		*  @brief    获取指定类型的父节点
		*
		*  @param    int nTag
		*  @return   PlatformKernel::KNode*
		*/
		virtual KNode* GetParent(int nTag) const;

		/**
		*  @brief    是否有子节
		*
		*  @return   bool
		*/
		virtual bool HasChildNode() const;

		std::vector<KNode*> GetChildNode(const CString& strChildNodeName);

	public:
		/**
		*  @brief    删除所有子节点
		*
		*  @param    bool bDelete 是否删除所有子节点内存
		*/
		virtual void RemoveAllNode(bool bDelete = true);

		/**
		*  @brief    删除指定索引位置的节点
		*
		*  @param    size_t nRemIndex 删除节点的索引(KPOS_TAIL标识移除最后一个节点)
		*											(KPOS_HEAD标识移除最前一个节点)
		*  @param    bool bDelete 是否删除节点内存
		*  @return   KNode* 返回删除的节点，没找到则返回NULL，删除内存返回NULL
		*/
		virtual KNode* RemoveNode(size_t nRemIndex = KPOS_TAIL, bool bDelete = true);

		/**
		*  @brief    删除指定的节点(删除的节点内存自行释放)
		*
		*  @param   KNode* pRemNode 删除的节点
		*  @param    bool bDelete 是否删除节点内存
		*  @return  size_t 返回删除的节点的索引，没找到返回KNODE_ERROR
		*/
		virtual size_t RemoveNode(KNode* pRemNode, bool bDelete = true);

		/**
		*  @brief    交换当前节点与参数节点位置
		*
		*  @param   KNode* pNode 移动的节点
		*  @return  bool true 成功交换两个节点位置
		*/
		virtual bool SwapNode(KNode* pNode);

		/**
		*  @brief    插入子节点在指定索引之后(插入的节点内存自行释放)
		*
		*  @param    KNode* pInsNode 新插入的节点
		*  @param    size_t nInsIndex 新插入节点索引位置，KPOS_TAIL则直接插入到最后
		*												  KPOS_HEAD则直接插入到最前
		*  @return   size_t 插入成功返回新插入节点基于0开始索引位置
		*				    如果节点已插入到其他节点下则返回KNODE_ERROR
		*				    如果传入错误索引则返回KNODE_ERROR
		*/
		virtual size_t InsertNode(KNode* pInsNode, size_t nInsIndex = KPOS_TAIL);

		/**
		*  @brief    插入子节点在指定节点之后(插入的节点内存自行释放)
		*
		*  @param    KNode* pInsNode 新插入的节点
		*  @param    KNode* pRefNode 相对节点
		*  @param    KNode* pRefNode 相对节点
		*  @param    bool bAfterIns true插入到相对节点后面,false插入到相对节点前面
		*  @return   size_t 插入成功返回新插入节点基于0开始索引位置
		*				    如果节点已插入到其他节点下则返回KNODE_ERROR
		*				    如果pAfterNode节点没找到则返回KNODE_ERROR
		*/
		virtual size_t InsertNode(KNode* pInsNode, KNode* pRefNode, bool bAfterIns = true);

	public:
		/**
		*  @brief    复制节点数据以及子节点
		*			 (如果拷贝子节点则当前节点下的子节点内存会全部释放)
		*
		*  @param    KNode* pSrcNode 原始数据的节点
		*  @param    bool bRecursion 是否复制子节点与数据
		*/
		virtual void CopyFrom(const KNode* pSrcNode, bool bRecursion = true);

	public:
		/**
		*  @brief    获取子节列表(不包括自身)
		*
		*  @return   const KNodeList& 返回子节点列表(没有子节点返回NULL)
		*/
		virtual const KNodeList& GetNodeList() const;

		/**
		*  @brief    根据Tag获取所有子节点(不包括自身)
		*
		*  @param    KNodeList& childNodes* 返回所有制定TAG子节点列表
		*  @param    int nTag KNODE_NONE_TAG
		*/
		virtual void GetAllNode(KNodeList& childNodes, int nTag = KNODE_NONE_TAG) const;

		/**
		*  @brief    获取所有属性
		*
		*  @return   const KNodeDataMap& 返回所有属性(没有属性返回NULL)
		*/
		virtual const KNodeDataMap& GetDataMap() const;

		/**
		*  @brief    根据Tag获取所有子节点(不包括自身)
		*
		*  @param    KNodeList& childNodes* 返回所有制定TAG子节点列表
		*  @param    int nTag KNODE_NONE_TAG
		*/
		virtual void GetAllNode(KNodeMap& childNodes, int nTag = KNODE_NONE_TAG) const;

	public:
		/**
		*  @brief    从XML节点中加载数据
		*
		*  @param    CXmlElement* pXmlElm XML节点指针
		*  @param    bool bRecursion 是否递归
		*  @return   bool 成功返回true
		*/
		bool LoadFromXmlElm(void* pXmlElm, bool bRecursion = true);

		/**
		*  @brief    保存数据到XML节点
		*
		*  @param    CXmlElement* pXmlElm XML节点指针
		*  @param    bool bRecursion 是否递归
		*  @return   bool 成功返回true
		*/
		bool SaveToXmlElm(void* pXmlElm, bool bRecursion = true) const;

		/**
		*  @brief    加载分卷XMDX大文件数据
		*
		*  @param    const CString& strFile 加载的文件名
		*  @param    bool bRecursion 是否递归
		*  @return   bool 成功返回true
		*/
		bool LoadFromLargeXmdx(const CString& strFile, bool bRecursion = true);

		/*
		* @brief     保存数据到XMDX文件
		*
		*  @param    const CString& strFile 保存的文件名
		*  @param    bool bRecursion 是否递归
		*  @return   bool 成功返回true
		*/
		bool SaveToLargeXmdx(const CString& strFile, bool bRecursion = true) const;

		/**
		*  @brief    从IXmdxElementReader加载数据
		*
		*  @param    IXmdxElementReader* pElmReader 从ELM中加载数据
		*  @param    bool bRecursion 是否递归
		*  @return   bool 成功返回true
		*/
		bool LoadFromXmdxElm(void* pElmReader, bool bRecursion = true);

		/*
		* @brief     保存数据到IXmdxElementWriter中
		*
		*  @param    IXmdxElementWriter* pElmWriter 保存数据ELM中
		*  @param    bool bRecursion 是否递归
		*  @return   bool 成功返回true
		*/
		bool SaveToXmdxElm(void* pElmWriter, bool bRecursion = true) const;

		/**
		*  @brief    关联撤销接口
		*
		*  @param    KNodeUndo* pUndo 撤销接口指针(外部释放)
		*/
		void AttachUndo(KNodeUndo* pUndo);

	protected:
		CGuid			_id;					///< 唯一标识
		int				_nTag;					///< 类型标识
		CString			_strElementName;		///< xml加载记录节点名称
		KNode*			_pParent;				///< 父节点指针
		KNodeList*		_pNodes;				///< 子节点列表 
		KNodeDataMap*	_pDataMap;				///< 属性集合
		KNodeUndo*		_pUndo;					///< 节点撤销
	};
}