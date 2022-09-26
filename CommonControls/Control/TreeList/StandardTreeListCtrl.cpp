//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           StandardTreeListCtrl.CPP
//    @author         zy
//    @data           2015/6/29 15:29
//    @brief          标准数据源的树形列表控件实现
//*****************************************************

#include "StdAfx.h"
#include "StandardTreeListCtrl.h"


CStandardDataSource::CStandardDataSource(CStandardDataSource* pParentNode, bool bCellOwnCfg)
{
	_pParentNode = pParentNode;
	_bCellOwnCfg = bCellOwnCfg;
}

CStandardDataSource::~CStandardDataSource()
{
}

size_t CStandardDataSource::GetCount(CTreeListConfig* pConfig)
{
	// 返回子节点数目
	return _subNodeList.size();
}

IListDataSource* CStandardDataSource::GetItem(size_t nIndex, bool& bShowItem, 
	HITEMDATA hItemData, CTreeListConfig* pConfig)
{
	// 拿到子节点
	if (nIndex >= _subNodeList.size())
	{
		return NULL;
	}

	return _subNodeList.at(nIndex);
}

void CStandardDataSource::GetCellData(size_t nCol, size_t nRow, 
	const HCOLUMNDATA hColumnData, HCELLINFO hCellData, HEDITCONFIG hEditConfig, CTreeListConfig* pConfig)
{
	if (_bCellOwnCfg)
	{
		// 单元格独立配置
		*hEditConfig = _cellConfig[(int)nCol];
	}
	else
	{
		// 单元格随列格式
		hEditConfig->CopyFrom(hColumnData->editConfig);
	}
}

CString CStandardDataSource::GetCellText(size_t nCol, size_t nRow, 
	const HEDITCONFIG hEditConfig, CTreeListConfig* pConfig)
{
	if (!_strDataArr.count((int)nCol))
	{
		return _T("");
	}

	// 获取当前节点数据
	return _strDataArr[(int)nCol];
}

CString CStandardDataSource::GetCellText(size_t nCol)
{
	// 获取当前文字
	return _strDataArr[(int)nCol];
}

bool CStandardDataSource::SetCellText(size_t nCol, size_t nRow, 
	const HEDITCONFIG hEditConfig, const CString& strNewValue, CTreeListConfig* pConfig)
{
	return SetCellText(nCol, strNewValue);
}

bool CStandardDataSource::SetCellText(size_t nCol, const CString& strNewValue)
{
	// 设置当前节点数据
	_strDataArr[(int)nCol] = strNewValue;

	return true;
}

size_t CStandardDataSource::GetKey()
{
	// 获取唯一标识
	return size_t(this);
}

CStandardDataSource * CStandardDataSource::InsertChild(size_t nIndex)
{
	// 设置序号不大于列表当前数量
	if (nIndex > _subNodeList.size())
	{
		return NULL;
	}

	CStandardDataSource *pChildNode = new CStandardDataSource(this, _bCellOwnCfg);

	// 插入到中间或尾巴
	if (nIndex < _subNodeList.size())
	{
		_subNodeList.insert(_subNodeList.begin() + nIndex, pChildNode);
	}
	else
	{
		_subNodeList.push_back(pChildNode);
	}	

	return pChildNode;
}

SubNodeList &CStandardDataSource::GetSubNodeList()
{
	return _subNodeList;
}

void CStandardDataSource::SetCellConfig(size_t nColumn, const EDITCONFIG& eCellConfig)
{
	int nCol = (int)nColumn;

	// 设置单元格配置
	_cellConfig[nCol].comboDataFile = eCellConfig.comboDataFile;
	_cellConfig[nCol].image = eCellConfig.image;
	_cellConfig[nCol].dotNum = eCellConfig.dotNum;
	_cellConfig[nCol].option = eCellConfig.option;
	_cellConfig[nCol].field = eCellConfig.field;
	_cellConfig[nCol].enumerate = eCellConfig.enumerate;
	_cellConfig[nCol].comboList = eCellConfig.comboList;
}

CStandardDataSource * CStandardDataSource::GetParentNode()
{
	// 返回父节点
	return _pParentNode;
}

void CStandardDataSource::SetCfgType(bool bCellOwnCfg)
{
	_bCellOwnCfg = bCellOwnCfg;
}

EDITCONFIG CStandardDataSource::GetCellCfg(size_t nColumn)
{
	return _cellConfig[(int)nColumn];
}





CStandardTreeListCtrl::CStandardTreeListCtrl(bool bCellOwnCfg)
{
	// 创建标准数据源并设置
	_bCellOwnCfg = bCellOwnCfg;
	_pRootDataSource = new CStandardDataSource(NULL, bCellOwnCfg);
	SetDataSource(_pRootDataSource, false);
}

CStandardTreeListCtrl::~CStandardTreeListCtrl()
{
	if (_pRootDataSource != NULL)
	{
		delete _pRootDataSource;
	}	
}

HCOLUMNDATA CStandardTreeListCtrl::InsertColumn()
{
	// 防止插入列时释放数据源指针
	_listDataSource.clear();
	
	// 插入到列尾
	HCOLUMNDATA colInfo = CTreeListCtrl::InsertColumn(_columnVector.size());

	return colInfo;
}

CStandardDataSource* CStandardTreeListCtrl::InsertRow(size_t nIndex, const CString& strText,
	CStandardDataSource* pParentNode /*= NULL*/)
{
	CStandardDataSource* pTmpNode = NULL;

	if (pParentNode == NULL)
	{
		// 缺省父节点是根节点
		pTmpNode = _pRootDataSource;
	}
	else
	{
		// 指定的父节点是否在根节点内
		pTmpNode = pParentNode;
		if (!FindChildNode(_pRootDataSource, pParentNode))
		{
			return NULL;
		}
	}

	// 在指定序列插入子节点
	CStandardDataSource* pChildNode = pTmpNode->InsertChild(nIndex);
	_ASSERTE(pChildNode != NULL);

	// 设置手列数据
	pChildNode->SetCellText(0, strText);

	return pChildNode;
}

void CStandardTreeListCtrl::DeleteRow(size_t nIndex, CStandardDataSource* pParentNode /*= NULL*/)
{
	CStandardDataSource* pTmpNode = NULL;

	if (pParentNode == NULL)
	{
		// 缺省父节点是根节点
		pTmpNode = _pRootDataSource;
	}
	else
	{
		// 指定的父节点是否在根节点内
		pTmpNode = pParentNode;
		if (!FindChildNode(_pRootDataSource, pParentNode))
		{
			return;
		}
	}

	// 序号超出子节点数
	SubNodeList &nodeList = pTmpNode->GetSubNodeList();
	if (nodeList.size() <= nIndex)
	{
		return;
	}

	// 遍历子节点下并删除
	CStandardDataSource* pChildNode = nodeList.at(nIndex);
	SubNodeList &childNodeList = pChildNode->GetSubNodeList();
	while (childNodeList.size() > 0)
	{
		DeleteRow(childNodeList.size() - 1, pChildNode);
	}

	// 将子节点列表中移除相应节点
	nodeList.erase(nodeList.begin() + nIndex);
	delete pChildNode;
}

void CStandardTreeListCtrl::DeleteAllRow()
{
	// 把根节点所有数据源删除
	SubNodeList &nodeList = _pRootDataSource->GetSubNodeList();

	while (nodeList.size() > 0)
	{
		DeleteRow(nodeList.size() - 1);
	}
}

void CStandardTreeListCtrl::SetCellText(size_t nRow, size_t nColumn, const CString& strCellText)
{
	// 获取数据源并设置单元格文字
	CStandardDataSource* pDataSource = GetDataSource(nRow, nColumn);

	pDataSource->SetCellText(nColumn, strCellText);
}

void CStandardTreeListCtrl::UpdateList()
{
	// 清空控件数据源容器
	// 防止数据节点刷新后被删除
	_listDataSource.clear();
	CTreeListCtrl::UpdateList(true);
}

void CStandardTreeListCtrl::SetCellConfig(size_t nRow, size_t nColumn, const EDITCONFIG& eCellConfig)
{
	// 获取数据源并设置单元格样式
	CStandardDataSource* pDataSource = GetDataSource(nRow, nColumn);

	pDataSource->SetCellConfig(nColumn, eCellConfig);
}

bool CStandardTreeListCtrl::FindChildNode(CStandardDataSource* pParentNode, CStandardDataSource* pChildNode)
{
	// 递归遍历所有的节点
	SubNodeList nodeList = pParentNode->GetSubNodeList();
	SubNodeListIter nodeIter = nodeList.begin();
	SubNodeListIter endter = nodeList.end();

	for (; nodeIter != endter; ++nodeIter)
	{
		// 找到相同的节点
		if (pChildNode == (*nodeIter))
		{
			return true;
		}

		// 进入子节点查找
		if (FindChildNode((*nodeIter), pChildNode))
		{
			return true;
		}
	}

	return false;
}

CStandardDataSource* CStandardTreeListCtrl::GetDataSource(size_t nRow, size_t nColumn)
{
	// 获取越界断言
 	_ASSERTE(nColumn < _columnVector.size() && nRow < _listDataSource.size());

	// 找到容器对应迭代器
	ListDataSourceIter dataIter = _listDataSource.begin();	
	size_t nIndex = nRow;
	while (nIndex--)
	{
		++dataIter;
	}

	// 通过控件数据源容器得到实际数据源
	CStandardDataSource* pDataSource =
		dynamic_cast<CStandardDataSource*>(dataIter->second->GetDataSource());
	_ASSERTE(pDataSource != NULL);

	return pDataSource;
}

size_t CStandardTreeListCtrl::GetRowsNum() const
{
	return _listDataSource.size();
}

CString CStandardTreeListCtrl::GetCellText(size_t nRow, size_t nColumn)
{
	// 获取数据源
	CStandardDataSource* pDataSource = GetDataSource(nRow, nColumn);

	return pDataSource->GetCellText(nColumn);
}

HCOLUMNDATA CStandardTreeListCtrl::GetColInfo(size_t nColumn)
{
	return _columnVector.at(nColumn);
}

bool CStandardTreeListCtrl::SelectCellText(size_t nRow, size_t nColumn, 
	const CString& strCellText, int nComboIndex)
{	
	CStringVector comboList;

	if (_bCellOwnCfg)
	{
		CStandardDataSource* pDataSource = GetDataSource(nRow, nColumn);
		comboList = pDataSource->GetCellCfg(nColumn).comboList;
	}
	else
	{
		HCOLUMNDATA colInfo = _columnVector.at(nColumn);
		comboList = colInfo->editConfig.comboList;
	}
	
	CStringVectorIter textIter = std::find(comboList.begin(), comboList.end(), (LPCTSTR)strCellText);
	if (textIter == comboList.end())
	{
		if (nComboIndex >= 0 && nComboIndex < (int)comboList.size())
		{
			SetCellText(nRow, nColumn, comboList.at(nComboIndex));
			return true;
		}
		else
			return false;
	}
	else
	{
		SetCellText(nRow, nColumn, strCellText);
	}
	
	return true;
}

void CStandardTreeListCtrl::SetTreeListCfgType(bool bCellOwnCfg)
{
	_bCellOwnCfg = bCellOwnCfg;
	_pRootDataSource->SetCfgType(bCellOwnCfg);
}
