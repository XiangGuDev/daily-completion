#include "stdafx.h"
#include "PropertyBandCtrl.h"
#include "BCGPPropEx.h"
#include "BCGPPropListPro.h"

using namespace PropEx;

IMPLEMENT_DYNAMIC(CPropertyBandCtrl, CBCGPWnd)

BEGIN_MESSAGE_MAP(CPropertyBandCtrl, CBCGPWnd)
	ON_WM_SIZE()
END_MESSAGE_MAP()

IMPL_SINGLE_CLASS(CPropertyBandCtrl)

CPropertyBandCtrl::CPropertyBandCtrl():/*_pProerty(nullptr),*/ _pStyle(nullptr), _propList(nullptr)
{
	_pStyle = new PROPERTYSTYLE();
	m_bVisualManagerStyle = TRUE;
	_nID = 1;
}

CPropertyBandCtrl::~CPropertyBandCtrl()
{
	Release();
	SAFE_DELETE(_pStyle);
	_pStyle = nullptr;
	SAFE_DELETE(_propList);
	_propList = nullptr;
}

BOOL CPropertyBandCtrl::CreatePropertyCtrl(CWnd* pParentWnd, UINT nID, const CRect &rt)
{
	CString strWndName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW);

	if (!Create(strWndName, L"属性面板", WS_CHILD | WS_VISIBLE, rt, pParentWnd, nID))
		return FALSE;

	CRect client;
	GetWindowRect(client);
	ScreenToClient(client);

	_propList = new CBCGPPropListPro();
	if (_propList->Create(WS_VISIBLE | WS_CHILD | WS_BORDER | WS_CLIPCHILDREN, client, this, 3091))
	{
		_nID = 0;
		EnableStyle();
		return TRUE;
	}

	return FALSE;
}

void CPropertyBandCtrl::SetPropertyData(const std::vector<IPropertyObjectData*> &datas)
{
	if (!_datas.empty())
	{
		Release();
	}

	_datas.reserve(datas.size());
	_datas.assign(datas.begin(), datas.end());
}

void CPropertyBandCtrl::UpdatePropertyPanel(bool bUpdate)
{
	if (_datas.empty())
	{
		_propList->SetRedraw();
		_propList->AdjustLayout();
		return;
	}

	// 重新刷新
	if (bUpdate)
	{
		_propList->RemoveAll();
		_itemMap.clear();
	}

	for (auto data : _datas)
	{
		Collect(data, nullptr);
	}
	_propList->SetRedraw();
	_propList->AdjustLayout();
}

void CPropertyBandCtrl::SetPropertyStyle(HPROPERTYSTYLE pStyle)
{
	if (nullptr == pStyle)
		return;

	SAFE_DELETE(_pStyle);
	_pStyle = pStyle;

	EnableStyle();

	_propList->AdjustLayout();
}

HPROPERTYSTYLE CPropertyBandCtrl::GetPropertyStyle()
{
	return _pStyle;
}

void CPropertyBandCtrl::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	CRect client;
	GetWindowRect(client);
	ScreenToClient(client);

	if (_propList && _propList->GetSafeHwnd())
	{
		::MoveWindow(_propList->GetSafeHwnd(), 0, 0, client.Width(), client.Height(), TRUE);
	}	
}

void CPropertyBandCtrl::Release()
{
	if (_datas.empty())
		return;

	int nCount = _propList->GetPropertyCount();
	for (int i = nCount - 1; i >= 0; --i)
	{
		CBCGPProp* pro = _propList->GetProperty(i);
		int nItemCount = pro->GetSubItemsCount();
		for (int j = nItemCount - 1; j >= 0; --j)
		{
			CBCGPPropEx* itemPro = (CBCGPPropEx*)pro->GetSubItem(j);
			if (!itemPro)
				continue;
			if (itemPro->IsSelected() && itemPro->IsInPlaceEditing())
			{
				itemPro->Show(false, true);
				continue;
			}
			if (itemPro->IsDelete())
			{
				itemPro = nullptr;
				continue;
			}
			pro->RemoveSubItem((CBCGPProp*&)itemPro);
			itemPro = nullptr;
		}
		//_propList->RemoveAllProperty();
		if (((CBCGPPropEx*)pro)->IsDelete())
			continue;
		_propList->DeleteProperty(pro, FALSE, FALSE);
		pro = nullptr;
	}

	_propList->AdjustLayout();
	
	std::for_each(_itemMap.begin(), _itemMap.end(), [](auto pair) 
	{
		IPropertyObjectData *pData = pair.first;
		SAFE_DELETE(pData);
		pData = nullptr;
	});

	_itemMap.clear();
	_datas.clear();
	_nID = 0;
}

CBCGPProp* CPropertyBandCtrl::GetOrCreateBCGPProp(IPropertyObjectData *pItem, CBCGPProp *pParentGroup, PROPERTYITEM &item)
{
	item.CopyFrom();
	pItem->GetItemConfig(&item);
//	_propList->SetNameAlign(item.nNameAlign,FALSE);

	// 不显示
	if (!item.bShow)
	{
		// 不显示也记录下来，用于最后内存回收
		_itemMap.insert(ItemPropMap::value_type(pItem, nullptr));
		return nullptr;
	}
		
	CBCGPProp *pPropItem = nullptr;
	auto itemIter = _itemMap.find(pItem);

	if (_itemMap.end() != itemIter && nullptr != itemIter->second)
	{
		pPropItem = (CBCGPPropEx *)itemIter->second;
		pPropItem->SetName(item.strItemName,FALSE);

		CBCGPPropEx *pPropItemEx = dynamic_cast<CBCGPPropEx*>(pPropItem);
		if (pPropItemEx)
		{
			pPropItemEx->SetItemData(pItem);
		}
	}
	else
	{
		if (IsLeaf(pItem, item))
		{
			pPropItem = CreateLeafProp(pItem, item);

			if (pParentGroup)
				pParentGroup->AddSubItem(pPropItem);
			else
				_propList->AddProperty(pPropItem, FALSE, FALSE);
		}
		else
		{
			pPropItem = new CBCGPPropEx(item.strItemName);

			if (pParentGroup)
				pParentGroup->AddSubItem(pPropItem);
			else
				_propList->AddProperty(pPropItem, FALSE, FALSE);
		}
			
		_itemMap.insert(ItemPropMap::value_type(pItem, pPropItem));
	}

	SetPropItem(pPropItem, item);
	return pPropItem;
}


void CPropertyBandCtrl::Collect(IPropertyObjectData *pItem, CBCGPProp *pParentGroup)
{
	size_t nCount = pItem->GetPropertyCount();
	PROPERTYITEM item;
	CBCGPProp *pNewProp = GetOrCreateBCGPProp(pItem, pParentGroup, item);

	for (size_t i = 0; i < nCount; ++i)
	{
		IPropertyObjectData *pChild = pItem->GetPropertyItem(i);
		if (nullptr == pChild)
			continue;

		if (pNewProp)
		{
			Collect(pChild, pNewProp);
		}
		else
		{
			Collect(pChild, pParentGroup);
		}
	}

	if (pNewProp && pNewProp->IsGroup())
	{
		pNewProp->Expand(item.bExpand);
	}	
}

bool CPropertyBandCtrl::IsLeaf(IPropertyObjectData *pItem, const PROPERTYITEM &item)
{
	bool bLeaf = false;

	switch (item.cate)
	{
	case Property::eGroup:
	{
		bLeaf = false;
		break;
	}
	case Property::eSimple:
	{
		bLeaf = true;
		break;
	}
	default:
	{
		pItem->GetPropertyCount() > 0 ? bLeaf = false : bLeaf = true;;
		break;
	}
	}

	return bLeaf;
}

void CPropertyBandCtrl::SetPropItem(CBCGPProp *pPropItem, const PROPERTYITEM & item)
{
	CBCGPPropEx *pPropItemEx = dynamic_cast<CBCGPPropEx*>(pPropItem);
	if (!pPropItemEx)
		return;

	pPropItemEx->SetPropertyCfg(item);
	pPropItemEx->AllowEdit(!item.bReadOnly);
}

CBCGPProp *CPropertyBandCtrl::CreateLeafProp(IPropertyObjectData *pItem, PROPERTYITEM & item)
{
	CAny val = pItem->GetItemValue();
	EAnyType anyType = val.GetType();
	CBCGPProp *pPropItem = nullptr;
	_variant_t var;

	switch (anyType)
	{
	case ANY_BOOL:
	{
		bool bVal = (bool)val;
		var = bVal;
		pPropItem = new CBCGPPropEx(item.strItemName, _nID++, (_variant_t)bVal);
		_propList->SetBoolLabels(item.strTrue, item.strFalse);
		break;
	}
	case ANY_INT:
	{
		int nVal = (int)val;
		var = nVal;
		pPropItem = new CBCGPPropEx(item.strItemName, _nID++, nVal, _T(""));
		break;
	}
	case ANY_DOUBLE:
	{
		double dVal = (double)val;
		var = dVal;
		pPropItem = new CBCGPPropEx(item.strItemName, _nID++, dVal, _T(""));
		break;
	}
	case ANY_STRING:
	{
		CString strVal = (CString)val;
		var = strVal;
		pPropItem = new CBCGPPropEx(item.strItemName, _nID++, _T(""), _T(""));
		break;
	}
	default:
		var = (CString)val;
		pPropItem = new CBCGPPropEx(item.strItemName, _nID++, _T(""), _T(""));
		break;
	}
	
	if (item.valueClr != -1)
		pPropItem->SetValueTextColor(item.valueClr,FALSE);

	CBCGPPropEx *pPropItemEx = dynamic_cast<CBCGPPropEx*>(pPropItem);
	if (pPropItemEx)
	{
		pPropItemEx->SetItemData(pItem);
	}

	if (Property::eComboBox == item.type)
	{
		pPropItem->RemoveAllOptions();
	
		std::for_each(item.options.begin(), item.options.end(), [&](auto option)
		{
			pPropItem->AddOption(option);
		});

		if (pPropItem->GetOptionCount() > item.nSel)
		{
			pPropItem->SelectOption(item.nSel);
		}
		pPropItem->SetValue(var);
	}
	else
	{
		pPropItem->SetValue(var);
	}

	return pPropItem;
}

void CPropertyBandCtrl::EnableStyle()
{
	if (nullptr == _propList || nullptr == _pStyle)
		return;

	// A-Z排序
	_propList->EnableToolBar(_pStyle->bEnableToolbar);

	// 搜索框
	_propList->EnableSearchBox(_pStyle->bEnableSearch, _pStyle->strSearchPrompt);

	// 属性列头
	_propList->EnableHeaderCtrl(_pStyle->bEnableHeader, _pStyle->strLeftName, _pStyle->strRightName);

	// 面板最下方显示选中项列名
	_propList->EnableDesciptionArea(_pStyle->bEnableDesc);

	// vs风格
	_propList->SetVSDotNetLook();

	// 关闭属性值修改字体加粗
	_propList->MarkModifiedProperties(_pStyle->bEnableModify);

	// 属性名列右侧有个小按钮
	_propList->EnableContextMenu(_pStyle->bEnableMenu);
}

void CPropertyBandCtrl::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	__super::PostNcDestroy();
	CPropertyBandCtrl::ResetInstance();
}