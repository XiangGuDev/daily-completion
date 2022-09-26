/*******************************************************************
* 文件名称：CustomizedControlsConstructor.cpp
* 作者姓名：刘庆仙
* 文件描述：创建自定义控件
* 需求版本：
* 创建时间：2018-4-16
* Copyright (C) 江西博微新技术有限公司 All rights reserved
*******************************************************************/

#include "stdafx.h"
#include "BCGPRibbonConstructorEx.h"
#include "BCGPRibbonComboBox.h"
#include "include/CustomizedControlsFactory.h"
#include "include/ICustomizedControlsCreator.h"
#include "include/BCGPRibbonBarEx.h"
#include "BCGPRibbonMainPanel.h"
#include "BCGPRibbonPanelMenuEx.h"

using namespace controls;

CBCGPRibbonConstructorEx::CBCGPRibbonConstructorEx(const CBCGPRibbonInfo& info)
	: CBCGPRibbonConstructor(info)
{
}

// 创建控件
CBCGPBaseRibbonElement* CBCGPRibbonConstructorEx::CreateElement (const CBCGPRibbonInfo::XElement& info) const
{
	CBCGPBaseRibbonElement* pElement = NULL;
	CString strID = info.m_ID.m_Name;
	CString strType;
	if (IsCustomElement(strID, strType))
	{
		// 创建控件
		controls::ICustomizedControlCreator* pCreator =
			controls::CCustomizedControlFactory::GetInstance()->GetCustomizedControlCreator(strType);

		ATLASSERT(NULL != pCreator);
		if (NULL != pCreator)
		{
			pElement = pCreator->CreateControl(info, (CBCGPRibbonConstructorEx*)this);
		}
	}
// 	else if (info.GetElementName() == _T("Button"))
//  	{
// 		controls::ICustomizedControlCreator* pCreator =
// 			controls::CCustomizedControlFactory::GetInstance()->GetCustomizedControlCreator(_T("ButtonEx"));
// 
// 		ATLASSERT(NULL != pCreator);
// 		if (NULL != pCreator)
// 		{
// 			pElement = pCreator->CreateControl(info, (CBCGPRibbonConstructorEx*)this);
// 		}
//  	}
	else
	{
		pElement = __super::CreateElement(info);
	}

	return pElement;
}

void CBCGPRibbonConstructorEx::ConstructCategoryMain(CBCGPRibbonBar& bar, const CBCGPRibbonInfo::XCategoryMain& info) const
{
	CBCGPRibbonMainPanelEx* pPanel = (CBCGPRibbonMainPanelEx*)CreateCategoryMainEx(bar, info);
	ASSERT_VALID(pPanel);

	CBCGPRibbonCategory* pCategory = bar.GetMainCategory();
	ASSERT_VALID(pCategory);

	const_cast<CBCGPToolBarImages&>(info.m_SmallImages.m_Image).CopyTo(pCategory->GetSmallImages());
	const_cast<CBCGPToolBarImages&>(info.m_LargeImages.m_Image).CopyTo(pCategory->GetLargeImages());

	if (info.m_bSearchEnable)
	{
		pPanel->EnableCommandSearch(info.m_bSearchEnable, info.m_strSearchLabel, info.m_strSearchKeys, info.m_nSearchWidth);
	}

	int i = 0;
	for (i = 0; i < info.m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElement =
			CreateElement(*(const CBCGPRibbonInfo::XElement*)info.m_arElements[i]);

		if (pElement != NULL)
		{
			ASSERT_VALID(pElement);

			if (info.m_arElements[i]->GetElementName().Compare(CBCGPRibbonInfo::s_szButton_MainPanel) == 0)
			{
				pPanel->AddToBottom((CBCGPRibbonMainPanelButton*)pElement);
			}
			else
			{
				pPanel->Add(pElement);
			}
		}
	}

	if (info.m_bRecentListEnable)
	{
		pPanel->AddRecentFilesList(info.m_strRecentListLabel, info.m_nRecentListWidth, info.m_bRecentListShowPins);
	}
}


CBCGPRibbonMainButton* CBCGPRibbonConstructorEx::CreateMainButton(CBCGPRibbonBar& bar) const
{
	//bar.m_bAutoDestroyMainButton = TRUE;
	CBCGPRibbonBarEx *ribEx = dynamic_cast<CBCGPRibbonBarEx*>(&bar);
	ribEx->SetmAutoDestroy(TRUE);
	bar.SetMainButton(new CBCGPRibbonMainButtonEx, CSize(45, 45));

	return bar.GetMainButton();
}

CBCGPRibbonMainPanelEx* CBCGPRibbonConstructorEx::CreateCategoryMainEx(CBCGPRibbonBar& bar, const CBCGPRibbonInfo::XCategoryMain& info) const
{
	CBCGPRibbonBarEx *ribEx = dynamic_cast<CBCGPRibbonBarEx*>(&bar);
	return ribEx->AddMainCategoryEx(info.m_strName, 0, 0,
		GetInfo().GetImageSize(CBCGPRibbonInfo::e_ImagesSmall),
		GetInfo().GetImageSize(CBCGPRibbonInfo::e_ImagesLarge));
}

BOOL CBCGPRibbonConstructorEx::IsCustomElement(const CString& strID,
											   CString& strEleType) const
{
	if (strID.IsEmpty())
	{
		return FALSE;
	}

	int nIndex = strID.Find(_T("#"));
	if (nIndex < 0)
	{
		return FALSE;
	}

	strEleType = strID.Right(strID.GetLength() - nIndex - 1);
	return TRUE;
}