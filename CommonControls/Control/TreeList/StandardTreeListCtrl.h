//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           StandardTreeListCtrl.H
//    @author         zy
//    @data           2015/6/29 15:30
//    @brief          标准数据源的树形列表控件
//*****************************************************

#pragma once

#include "TreeListCtrl.h"

// 数据源子节点列表类型
class CStandardDataSource;
typedef std::vector<CStandardDataSource*> SubNodeList;
typedef SubNodeList::iterator SubNodeListIter;

// 数据源存放数据的数组类型
typedef std::map<int, CString> DataSourceStrVec;
typedef DataSourceStrVec::iterator DataSourceStrVecIter;

// 数据源单元格配置数据类型
typedef std::map<int, EDITCONFIG> ConfigArr;
typedef ConfigArr::iterator ConfigArrIter;

/**
*  @class	标准数据源
*
*  @brief	支持控件用
*/
class CStandardDataSource : public IListDataSource
{
public:
	/**
	*  @brief    构造函数
	*
	*  @param    CStandardDataSource * pParentNode	设置父节点 
	*/
	CStandardDataSource(CStandardDataSource* pParentNode, bool bCellOwnCfg);
	~CStandardDataSource();

public:
	/**
	*  @brief    设置配置类型
	*
	*  @param    bool bCellOwnCfg	单元格配置
	*  @return   void
	*/
	void SetCfgType(bool bCellOwnCfg);

	/**
	*  @brief    获取子项数目
	*
	*  @param    CTreeListConfig*& pConfig 树列表控件配置
	*  @return   size_t 返回子项数目
	*/
	virtual size_t GetCount(CTreeListConfig* pConfig);

	/**
	*  @brief    获取指定子项数据
	*
	*  @param    size_t nIndex 子项索引
	*  @param    bool bShowItem 设置true则此条数据将显示在控件上（用于过滤不需要显示的中间节点）
	*  @param    HITEMDATA hItemData 列表行数据(只有第一次设置才有用)
	*  @param    CTreeListConfig*& pConfig 树列表控件配置
	*  @return   IListDataSource*& pListDataSource 当前获取到的数据源
	*/
	virtual IListDataSource* GetItem(size_t nIndex, bool& bShowItem,
		HITEMDATA hItemData, CTreeListConfig* pConfig);

	/**
	*  @brief    获取单元格数据信息
	*
	*  @param    size_t nCol 单元格所在列
	*  @param    size_t nRow 单元格所在行
	*  @param    const HCOLUMNDATA hColumnData 列头信息
	*  @param    HCELLDATA hCellData 返回单元格数据数
	*  @param    HEDITCONFIG hEditConfig 返回编辑配置数据
	*  @param    CTreeListConfig*& pConfig 树列表控件配置
	*/
	virtual void GetCellData(size_t nCol, size_t nRow,
		const HCOLUMNDATA hColumnData, HCELLINFO hCellData, HEDITCONFIG hEditConfig, CTreeListConfig* pConfig);

	/**
	*  @brief    获取单元格文本数据
	*
	*  @param    size_t nCol 单元格所在列
	*  @param    size_t nRow 单元格所在行
	*  @param    const HEDITCONFIG hEditConfig 编辑配置数据
	*  @param    CTreeListConfig*& pConfig 树列表控件配置
	*  @return   CString 返回单元格文本数据
	*/
	virtual CString GetCellText(size_t nCol, size_t nRow,
		const HEDITCONFIG hEditConfig, CTreeListConfig* pConfig);

	/**
	*  @brief    设置单元格文本数据
	*
	*  @param    size_t nCol	单元格所在列
	*  @param    const CString& strNewValue	设置的值
	*  @return   bool	返回true则修改成功
	*/
	virtual CString GetCellText(size_t nCol);

	/**
	*  @brief    设置单元格文本数据
	*
	*  @param    size_t nCol 单元格所在列
	*  @param    size_t nRow 单元格所在行
	*  @param    const HEDITCONFIG hEditConfig 编辑配置数据
	*  @param    const CString& strNewValueg 当前设置的值
	*  @param    CTreeListConfig*& pConfig 树列表控件配置
	*  @return   bool 返回true则修改成功
	*/
	virtual bool SetCellText(size_t nCol, size_t nRow,
		const HEDITCONFIG hEditConfig, const CString& strNewValue, CTreeListConfig* pConfig);

	/**
	*  @brief    设置单元格文本数据
	*
	*  @param    size_t nCol	单元格所在列
	*  @param    const CString& strNewValue	设置的值
	*  @return   bool	返回true则修改成功
	*/
	virtual bool SetCellText(size_t nCol, const CString& strNewValue);

	/**
	*  @brief    插入子数据源
	*
	*  @param    size_t nIndex	子节点在父节点下位置，0插入到最前，-1插入到最后
	*  @return   CStandardDataSource *	返回子节点
	*/
	CStandardDataSource *InsertChild(size_t nIndex);

	/**
	*  @brief    获取子节点列表
	*
	*  @return   SubNodeList &	子节点列表
	*/
	SubNodeList &GetSubNodeList();

	/**
	*  @brief    设置当前数据源某列单元格样式
	*
	*  @param    size_t nColumn	列数
	*  @param    const EDITCONFIG & eCellConfig	格式结构体
	*/
	void SetCellConfig(size_t nColumn, const EDITCONFIG& eCellConfig);

	/**
	*  @brief    获取当前数据源的父节点
	*
	*  @return   CStandardDataSource *	父节点指针
	*/
	CStandardDataSource *GetParentNode();

	/**
	*  @brief    获取单元格配置
	*
	*  @param    size_t nColumn	列号
	*  @return   EDITCONFIG	配置
	*/
	EDITCONFIG GetCellCfg(size_t nColumn);

private:
	/**
	*  @brief    获取数据源唯一标识(只能控件调用)
	*
	*  @return   size_t 返回数据源唯一标识
	*/
	virtual size_t GetKey();

private:
	DataSourceStrVec _strDataArr;		///< 数据源数据容器
	SubNodeList _subNodeList;			///< 子节点容器
	CStandardDataSource *_pParentNode;	///< 父节点
	ConfigArr _cellConfig;				///< 单元格配置容器
	bool _bCellOwnCfg;					///< 单元格独立标志位
};


/**
*  @class	无需数据源的树形列表控件
*
*  @brief	内含标准数据源
*/
class COMMONCONTROLS_API CStandardTreeListCtrl : public CTreeListCtrl
{
public:
	CStandardTreeListCtrl(bool bCellOwnCfg = false);
	~CStandardTreeListCtrl();

public:
	/**
	*  @brief    设置控件配置类型
	*
	*  @param    bool bCellOwnCfg	单元格配置
	*  @return   void
	*/
	void SetTreeListCfgType(bool bCellOwnCfg);

	/**
	*  @brief    在列尾插入新列
	*
	*  @return   HCOLUMNDATA	返回列配置
	*/
	HCOLUMNDATA InsertColumn();

	/**
	*  @brief    获取列信息
	*
	*  @param    size_t nColumn	列号
	*  @return   HCOLUMNDATA	信息结构
	*/
	HCOLUMNDATA GetColInfo(size_t nColumn);

	/**
	*  @brief    插入行
	*
	*  @param    size_t nIndex	序列
	*  @param    const CString & strText	第一列文字
	*  @param    CStandardDataSource * pParentNode	父节点
	*  @return   CStandardDataSource*	传出子节点，如果插入失败为NULL
	*/
	CStandardDataSource* InsertRow(size_t nIndex, const CString& strText, CStandardDataSource* pParentNode = NULL);

	/**
	*  @brief    删除行
	*
	*  @param    size_t nIndex	序号
	*  @param    IModelNode * pParentNode	删除行的父节点
	*/
	void DeleteRow(size_t nIndex, CStandardDataSource* pParentNode = NULL);

	/**
	*  @brief    删除所有行
	*/
	void DeleteAllRow();

	/**
	*  @brief    设置单元格文字
	*
	*  @param    size_t nRow	行数
	*  @param    size_t nColumn	列数
	*  @param    const CString & strCellText	文字
	*/
	void SetCellText(size_t nRow, size_t nColumn, const CString& strCellText);

	/**
	*  @brief    设置下拉单元格文字
	*
	*  @param    size_t nRow	行数
	*  @param    size_t nColumn	列数
	*  @param    const CString & strCellText	待搜索下拉的文字
	*  @param    int nComboIndex	下拉序号，默认不处理
	*  @return   bool	设置结果
	*/
	bool SelectCellText(size_t nRow, size_t nColumn, const CString& strCellText, int nComboIndex = -1);

	/**
	*  @brief    设置单元格样式
	*
	*  @param    size_t nRow	行数
	*  @param    size_t nColumn	列数
	*  @param    const EDITCONFIG & eCellConfig	样式结构体
	*/
	void SetCellConfig(size_t nRow, size_t nColumn, const EDITCONFIG& eCellConfig);

	/**
	*  @brief    获取行数
	*
	*  @return   size_t	返回行数
	*/
	size_t GetRowsNum() const;

	/**
	*  @brief    获取单元格文字
	*
	*  @param    size_t nRow	行数
	*  @param    size_t nColumn	列数
	*  @return   CString	返回文字
	*/
	CString GetCellText(size_t nRow, size_t nColumn);

	/**
	*  @brief    更新数据
	*
	*/
	void UpdateList();

protected:
	/**
	*  @brief    递归遍历找到子节点
	*
	*  @param    CStandardDataSource * pParentNode	父节点
	*  @param    CStandardDataSource * pChildNode	待找的子节点
	*  @return   bool	查找结果
	*/
	bool FindChildNode(CStandardDataSource* pParentNode, CStandardDataSource* pChildNode);

	/**
	*  @brief    获取数据源节点
	*
	*  @param    size_t nRow	行数
	*  @param    size_t nColumn	列数
	*  @return   CStandardDataSource*	返回数据源
	*/
	CStandardDataSource* GetDataSource(size_t nRow, size_t nColumn);

protected:
	CStandardDataSource *_pRootDataSource;	///< 根数据源
	bool	_bCellOwnCfg;					///< 单元格数据配置
};
