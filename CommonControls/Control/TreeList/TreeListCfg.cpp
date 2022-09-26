#include "StdAfx.h"
#include "TreeListCfg.h"
#include <algorithm>
#include "TreeListCtrl.h"

BEGIN_ENUMTYPENAME_CONVERSION(EFormatType)
{
	{ FORMAT_LEFT, _T("左对齐") },
	{ FORMAT_MIDDLE, _T("居中对齐") },
	{ FORMAT_RIGHT, _T("右对齐") },
	{ NULL, NULL}
}
END_ENUMTYPENAME_CONVERSION(EFormatType);

BEGIN_ENUMTYPENAME_CONVERSION(ETreeListEditFlag)
{
	{ eNoneEdit, _T("无编辑") },
	{ eNormalEdit, _T("普通编辑") },
	{ eValidEdit, _T("检验编辑") },
	{ eFileNameEdit, _T("文件名编辑") },
	{ eDateTimeEdit, _T("日期编辑框") },
	{ eDropListCobEdit, _T("下拉选择框") },
	{ eDropDownCobEdit, _T("可编辑的下拉选择框") },
	{ eCheckBoxEdit, _T("复选框") },
	{ eCheckBox3StateEdit, _T("三态复选框") },
	{ eHyperLinkEdit, _T("超链接") },
	{ eAngleEdit, _T("角度") },
	{ eTreeComboEdit, _T("树下拉框") },
	{ eProgressEdit, _T("进度条") },
	{ eTreeCheckComboEdit, _T("下拉树多选框") },
	{ eRangelEdit, _T("范围编辑控件") },
	{ eAutoSearchCombo, _T("自动搜索组合框") },
	{ eTreeComboManualEdit, _T("可编辑的树下拉框") },
	{ eTreeCheckComboManualEdit, _T("可编辑的树下拉多选框") },
	{ NULL, NULL }
}
END_ENUMTYPENAME_CONVERSION(ETreeListEditFlag);

BEGIN_ENUMTYPENAME_CONVERSION(EValidEditControlFlag)
{
	{ eVeDecimal, _T("允许输入小数") },
	{ eVeNegative, _T("允许输入负数") },
	{ eVeDotNum, _T("限制输入小数位数") },
	{ eVeNoZero, _T("禁止输入0") },
	{ eVeNoEmpty, _T("不允许为空") },
	{ eVePreZero, _T("允许前置零") },
	{ NULL, NULL }
}
END_ENUMTYPENAME_CONVERSION(EValidEditControlFlag);

BEGIN_ENUMTYPENAME_CONVERSION(ETreeComboEditControlFlag)
{
	{ eTreeHasSelAll, _T("自动添加全选") },
	{ eTreeHasNoLimit, _T("自动添加不限") },
	{ eTreeAutoWidth, _T("自动调整下拉宽度") },
	{ eTreeSelNotByText, _T("不通过界面值进行选中") },
	{ NULL, NULL }
}
END_ENUMTYPENAME_CONVERSION(ETreeComboEditControlFlag);


BEGIN_ENUMTYPENAME_CONVERSION(EDateTimeEditFlag)
{
	{ eDateSlashYMD, _T("斜杠年月日") },
	{ eDateChineseYMD, _T("中文年月日") },
	{ eDateDashYMD, _T("破折年月日") },
	{ NULL, NULL }
}
END_ENUMTYPENAME_CONVERSION(EDateTimeEditFlag);

BEGIN_ENUMTYPENAME_CONVERSION(EProgressControlFlag)
{
	{ ePrgShowText, _T("显示进度文字") },
	{ NULL, NULL }
}
END_ENUMTYPENAME_CONVERSION(EProgressControlFlag);

CTreeListConfig::CTreeListConfig(CTreeListCtrl* pTreeListCtrl)
{
	_colWdithScale = 1;
	_pTreeListCtrl = pTreeListCtrl;
}

CTreeListConfig::~CTreeListConfig()
{
	ClearCellConfigVectors();
}

void CTreeListConfig::ClearCellConfigVectors()
{
	size_t nRowCount = _cellCfgVectors.size();
	for (size_t nRow = 0; nRow < nRowCount; ++nRow)
	{
		size_t nColCount = _cellCfgVectors[nRow].size();
		for (size_t nCol = 0; nCol < nColCount; ++nCol)
		{
			delete _cellCfgVectors[nRow][nCol];
		}
		_cellCfgVectors[nRow].clear();
	}

	_cellCfgVectors.clear();
}

CTreeListCtrl* CTreeListConfig::GetTreeListCtrl()
{
	return _pTreeListCtrl;
}

bool CTreeListConfig::LoadConfig(const CString& strCfgFile, const CString& strCfgName)
{
	CXmlDocument xmlDoc;

	// 加载配置文件
	if (!xmlDoc.LoadFile(strCfgFile, fmtXML))
		return false;

	CXmlElement* pRoot = xmlDoc.GetElementRoot();
	if (NULL == pRoot || 0 == pRoot->GetChildElementCount())
		return false;

	// 如果没有指定配置名则加载第一个配置
	CXmlElement* pTreeConfig = strCfgName.IsEmpty() ?
		pRoot->GetChildElements()->GetAt(0) : pRoot->GetChildElementAt(strCfgName);
	if (NULL == pTreeConfig)
		return false;

	// 获取字体缩放比例
	_colWdithScale = _pTreeListCtrl->GetFontScale();

	// 获取表头节点并加载
	CXmlElement* pListHeader = pTreeConfig->GetChildElementAt(L"ListHeader", FALSE);
	if (!LoadColumn(pListHeader, NULL))
		return false;

	// 加载行过滤器
	CXmlElement* pRowFilter = pTreeConfig->GetChildElementAt(L"RowFilter", FALSE);
	if (NULL != pRowFilter)
	{
		// 加载类型过滤器
		LoadTypeFilter(pRowFilter->GetChildElementAt(L"TypeFilter"));
	}

	// 加载自由行数据
	CXmlElement* pListData = pTreeConfig->GetChildElementAt(L"ListData", FALSE);
	if (NULL != pListData)
	{
		LoadListData(pListData);
	}

	return true;
}

bool CTreeListConfig::IsTypeInFilter(int nType)
{
	if (_typeFilter.empty())
		return true;

	return (_typeFilter.end() != std::find(_typeFilter.begin(), _typeFilter.end(), nType));
}

const CellConfigVectors& CTreeListConfig::GetCellConfigVectors()
{
	return _cellCfgVectors;
}

bool CTreeListConfig::GetCellConfigByRowCol(size_t nRow, size_t nCol, HCELLCONFIG& pCellCfg)
{
	pCellCfg = NULL;

	if (nRow >= _cellCfgVectors.size())
		return false;

	// 获取单元格的配置信息;
	const CellConfigVector& cellRow = *(_cellCfgVectors.begin() + nRow);
	if (nCol >= cellRow.size())
		return false;

	pCellCfg = *(cellRow.begin() + nCol);

	return true;
}

bool CTreeListConfig::LoadColumn(CXmlElement* pColumnCfg, HCOLUMNDATA hParentColumn)
{
	if (NULL == pColumnCfg)
		return false;

	CXmlElements* pHeaderList = pColumnCfg->GetChildElements(FALSE);
	if (NULL == pHeaderList)
		return false;

	if (NULL == hParentColumn)
	{
		// 清除所有列
		_pTreeListCtrl->DeleteAllColumn();

		// 加载列高度
		CString strHeight = pColumnCfg->GetAttrValue(L"Height", L"0");
		_pTreeListCtrl->SetHeadItemHeight(_ttoi(strHeight));

		CString strWordWarp = pColumnCfg->GetAttrValue(L"WordWarp", L"0");
		_pTreeListCtrl->SetColumnWordWarp(!!_ttoi(strWordWarp));
	}

	// 遍历表头
	size_t nColIndex = 0;
	size_t nCount = pHeaderList->GetCount();
	for (size_t i = 0; i < nCount; ++i)
	{
		CString strText;
		CXmlElement* pHeader = pHeaderList->GetAt(i);

		CString strElmName = pHeader->GetElementName();
		if (strElmName == L"CellCfg" && hParentColumn)
		{
			// 加载表头单元格配置信息
			LoadEditConfig(pHeader, &hParentColumn->editConfig);
			continue;
		}

		// 列头不显示(默认显示)
		strText = pHeader->GetAttrValue(L"Show", L"1");
		if (strText.IsEmpty() || strText == L"0")
			continue;

		// 添加一个新表头
		HCOLUMNDATA hColumn = _pTreeListCtrl->InsertColumn(nColIndex, hParentColumn);

		strText = pHeader->GetAttrValue(L"Fixed");
		if (!strText.IsEmpty())
			hColumn->fixed = !!_ttoi(strText);

		strText = pHeader->GetAttrValue(L"Image");
		if (!strText.IsEmpty())
			hColumn->image = _ttoi(strText);

		strText = pHeader->GetAttrValue(L"Width");
		if (!strText.IsEmpty())
		{
			// 处理125%字体问题
			int nWidth = int(_ttoi(strText) * _colWdithScale);
			hColumn->SetWidth(nWidth);
		}

		strText = pHeader->GetAttrValue(L"HeaderFormat");
		if (!strText.IsEmpty())
			hColumn->headerformat = GetFormatEnum(strText);

		strText = pHeader->GetAttrValue(L"Format");
		if (!strText.IsEmpty())
			hColumn->format = GetFormatEnum(strText);

		strText = pHeader->GetAttrValue(L"TextColor");
		if (!strText.IsEmpty())
			hColumn->textColor = GetStringColor(strText);

		strText = pHeader->GetAttrValue(L"Title");
		if (!strText.IsEmpty())
			hColumn->title = strText;

		strText = pHeader->GetAttrValue(L"SortType");
		if (!strText.IsEmpty())
			hColumn->sortType = _ttoi(strText);

		// 设置树形展现列;
		strText = pHeader->GetAttrValue(L"IsTreeCol");
		if (!strText.IsEmpty() && !!_ttoi(strText))
			_pTreeListCtrl->SetTreeColumn((int)nColIndex);

		// 加载子列头或者列配置数据
		LoadColumn(pHeader, hColumn);

		// 递增列
		nColIndex++;
	}

	return true;
}

void CTreeListConfig::LoadListData(CXmlElement* pListData)
{
	CXmlElements* pRowList = pListData->GetChildElements(FALSE);
	if (NULL == pRowList)
		return;

	ClearCellConfigVectors();

	// 遍历表头
	size_t nRowCount = pRowList->GetCount();
	for (size_t nRow = 0; nRow < nRowCount; ++nRow)
	{
		CXmlElement* pRow = pRowList->GetAt(nRow);
		CXmlElements* pCellList = pRow->GetChildElements();

		_cellCfgVectors.push_back(CellConfigVector());
		CellConfigVector& rowList = _cellCfgVectors.back();

		size_t nCellCount = pCellList->GetCount();
		for (size_t nCell = 0; nCell < nCellCount; ++nCell)
		{
			CXmlElement* pClell = pCellList->GetAt(nCell);

			CString strText;
			HCELLCONFIG hCellCfg = new CELLCONFIG();

			// 加载单元格配置数据
			strText = pClell->GetAttrValue(L"Text");
			if (!strText.IsEmpty())
				hCellCfg->cellText = strText;

			strText = pClell->GetAttrValue(L"Readonly");
			if (!strText.IsEmpty())
				hCellCfg->cellInfo.readonly = (_ttoi(strText) != 0);

			strText = pClell->GetAttrValue(L"TextColor");
			if (!strText.IsEmpty())
				hCellCfg->cellInfo.textColor = GetStringColor(strText);

			strText = pClell->GetAttrValue(L"BackColor");
			if (!strText.IsEmpty())
				hCellCfg->cellInfo.backColor = GetStringColor(strText);

			HCOLUMNDATA hColumnData = _pTreeListCtrl->GetColumn(nCell);

			// 加载编辑数据
			CXmlElement* pCellCfg = pClell->GetChildElementAt(L"CellCfg", FALSE);
			if (NULL != pCellCfg)
			{
				LoadEditConfig(pCellCfg, &(hCellCfg->editConfig), &(hColumnData->editConfig));
			}
			else
			{
				hCellCfg->editConfig = hColumnData->editConfig;
			}

			rowList.push_back(hCellCfg);
		}
	}
}

void CTreeListConfig::LoadTypeFilter(CXmlElement* pTypeFilter)
{
	if (NULL == pTypeFilter)
		return;

	CString strTypeList = pTypeFilter->GetAttrValue(L"TypeList");
	if (strTypeList.IsEmpty())
		return;

	// 读取过滤节点类型
	int nWord = CStringToolkit::CountWord((LPCTSTR)strTypeList, L',');
	for (int i = 1; i <= nWord; ++i)
	{
		int nType = _wtoi(CStringToolkit::ReadWord((LPCTSTR)strTypeList, i, L','));
		_typeFilter.push_back(nType);
	}
}

void CTreeListConfig::LoadEditConfig(CXmlElement* pCellCfg, HEDITCONFIG hEditConfig,
	HEDITCONFIG hColEditConfig)
{
	CString strText;

	strText = pCellCfg->GetAttrValue(L"Image");
	if (!strText.IsEmpty())
		hEditConfig->image = _ttoi(strText);
	else if (NULL != hColEditConfig)
		hEditConfig->image = hColEditConfig->image;

	strText = pCellCfg->GetAttrValue(L"DotNum");
	if (!strText.IsEmpty())
		hEditConfig->dotNum = _ttoi(strText);
	else if (NULL != hColEditConfig)
		hEditConfig->dotNum = hColEditConfig->dotNum;

	strText = pCellCfg->GetAttrValue(L"Option");
	if (!strText.IsEmpty())
		hEditConfig->option = GetOptionEnum(strText);
	else if (NULL != hColEditConfig)
		hEditConfig->option = hColEditConfig->option;

	strText = pCellCfg->GetAttrValue(L"Field");
	if (!strText.IsEmpty())
		hEditConfig->field = strText;
	else if (NULL != hColEditConfig)
		hEditConfig->field = hColEditConfig->field;

	strText = pCellCfg->GetAttrValue(L"SplitChar");
	if (!strText.IsEmpty())
		hEditConfig->splitChar = strText;
	else if (NULL != hColEditConfig)
		hEditConfig->splitChar = hColEditConfig->splitChar;

	strText = pCellCfg->GetAttrValue(L"Enumerate");
	if (!strText.IsEmpty())
		hEditConfig->enumerate = strText;
	else if (NULL != hColEditConfig)
		hEditConfig->enumerate = hColEditConfig->enumerate;

	strText = pCellCfg->GetAttrValue(L"EventHandle");
	if (!strText.IsEmpty())
		hEditConfig->eventHandle = strText;
	else if (NULL != hColEditConfig)
		hEditConfig->eventHandle = hColEditConfig->eventHandle;

	// 下拉框数据
	CXmlElement* pComboxList = pCellCfg->GetChildElementAt(L"ComboBoxList", FALSE);
	if (NULL != pComboxList)
	{
		hEditConfig->comboDataFile = pComboxList->GetAttrValue(L"DataFile");

		CXmlElements* pChilds = pComboxList->GetChildElements();
		size_t nCount = pChilds->GetCount();
		for (size_t i = 0; i < nCount; ++i)
		{
			hEditConfig->comboList.push_back(pChilds->GetAt(i)->GetElementText());
		}
	}
	else if (NULL != hColEditConfig)
	{
		hEditConfig->comboList = hColEditConfig->comboList;
	}
}

COLORREF CTreeListConfig::GetStringColor(const CString& strColor)
{
	COLORREF color = RGB(0, 0, 0);

	int nFirst = strColor.Find(L",");
	if (-1 == nFirst)
		return color;

	int nSecond = strColor.Find(L",", nFirst + 1);
	if (-1 == nSecond)
		return color;

	return RGB(_ttoi(strColor.Mid(0, nFirst)),
		_ttoi(strColor.Mid(nFirst + 1, nSecond - nFirst - 1)),
		_ttoi(strColor.Mid(nSecond + 1, strColor.GetLength() - nSecond - 1)));
}

int CTreeListConfig::GetFormatEnum(const CString& str)
{
	return TextToEFormatType(str);
}

int CTreeListConfig::GetOptionEnum(const CString& str)
{
	//当Option关键字对应值为0或为空时
	if (str == L"0" || str == L"")
	{
		return 0;
	}

	std::vector<CString> ArrStr;
	CStringToolkitEx::SplitString((LPCTSTR)str, ArrStr, L"|");

	//获取第一个关键字
	int nEditFlag = TextToETreeListEditFlag(ArrStr.at(0));
	int nSubFlag = 0;
	for (int i = 1; i < (int)ArrStr.size(); ++i)
	{
		switch (nEditFlag)
		{
		//若类型为编辑框
		case eNoneEdit:
		case eValidEdit:
		case eRangelEdit:
		{
			//继续分析编辑框专用标志位
			nSubFlag += TextToEValidEditControlFlag(ArrStr.at(i));
			break;
		}
		//若类型为树下拉框
		case eTreeComboEdit:
		case eTreeComboManualEdit:
		{
			//继续分析下拉框专用标志位
			nSubFlag += TextToETreeComboEditControlFlag(ArrStr.at(i));
			break;
		}
		//若类型为日期编辑框
		case eDateTimeEdit:
		{
			//继续分析下拉框专用标志位
			nSubFlag += TextToEDateTimeEditFlag(ArrStr.at(i));
			break;
		}
		//若类型为进度条
		case eProgressEdit:
		{
			//继续分析进度条专用标志位
			nSubFlag += TextToEProgressControlFlag(ArrStr.at(i));
			break;
		}
		case eTreeCheckComboEdit:
		{
			//继续分析下拉树多选框标志位
			nSubFlag += TextToETreeComboEditControlFlag(ArrStr.at(i));
			break;
		}
		default:
			break;
		}
	}
	return nEditFlag + nSubFlag;
}

void CTreeListConfig::SetTempData(const CString& strTmpData)
{
	_strTmpData = strTmpData;
}

const CString& CTreeListConfig::GetTempData()
{
	return _strTmpData;
}
