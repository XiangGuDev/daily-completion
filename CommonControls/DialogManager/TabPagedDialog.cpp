#include "stdafx.h"
#include "TabPagedDialog.h"
#include "TabDialog.h"

static LPCTSTR s_MainDialog = L"MainDialog";
static LPCTSTR s_Caption = L"Caption";
static LPCTSTR s_Pos = L"Pos";
static LPCTSTR s_Id = L"id";
extern IObject* createObject(LPCTSTR className);

ITabPageContext* CTabPagedDialog::CreatePageByKey(LPCTSTR szPageClassName)
{
	auto finder = s_mapFactory.find(szPageClassName);
	return finder == s_mapFactory.end() ? NULL : finder->second->CreateObject();
}

std::map<CString, ITabPageContextFactory*> CTabPagedDialog::s_mapFactory;

CTabPagedDialog::CTabPagedDialog()
{
	m_pImpl = NULL;
}


CTabPagedDialog::~CTabPagedDialog()
{
	delete m_pImpl;

	for (auto page : m_arrSubPages)
	{
		SAFE_DELETE(page);
	}
}

CTabPagedDialog* CTabPagedDialog::CreateDialogByXmlCfg(HWND hWnd, LPCTSTR szXmlCfg)
{
	// 读取xml
	CXmlDocument cfg;
	if (FALSE == cfg.LoadFile(szXmlCfg, fmtXML))
		return NULL;

	DialogSetting dlgSetting;
	auto root = cfg.GetElementRoot();
	CString strRootName = root->GetElementName();
	if (strRootName != s_MainDialog)
		return NULL;
	
	// 读取对话框配置
 	CTabPagedDialog *pDlgWillShow = new CTabPagedDialog();
	dlgSetting.strCaptionName = root->GetAttrValue(s_Caption);
	CString strPos = root->GetAttrValue(s_Pos);
	dlgSetting.posType = Text2DialogSetting_PosType(strPos);
	
	auto children = root->GetChildElements();
	for (int i = 0; i < children->GetCount(); ++i)
	{
		auto pageCfg = children->GetAt(i);
		CString pageId = pageCfg->GetAttrValue(s_Id);

		// 创建页面
		ITabPageContext *pPage = CreatePageByKey(pageId);
		if (pPage == NULL)
			continue;

		// 设置tab页面名称
		ITabPageContextWithCfgName *pNamedPage = dynamic_cast<ITabPageContextWithCfgName*>(pPage);
		if (pNamedPage)
		{
			CString strCaption = pageCfg->GetAttrValue(s_Caption);
			pNamedPage->SetCaptionName(strCaption);
		}
		
		pDlgWillShow->m_arrSubPages.push_back(pPage);
	}

	CTabDialog *pTabDlg = new CTabDialog(pDlgWillShow->m_arrSubPages, dlgSetting);
	pDlgWillShow->m_pImpl = pTabDlg;
	return pDlgWillShow;
}

void CTabPagedDialog::RegistPageFactory(LPCTSTR szPageClassName, ITabPageContextFactory *pFactory)
{
	s_mapFactory[szPageClassName] = pFactory;
}

int CTabPagedDialog::DoModal()
{
	d3s::views::view::IOsgViewPort *pActiveView = CGISCommonTool::GetActiveViewPort();
	auto oldFrameRate = pActiveView->GetViewer()->getRunMaxFrameRate();
	pActiveView->GetViewer()->setRunMaxFrameRate(2);
	INIT_RESHANDLE()
	auto retVal = m_pImpl->DoModal();
	pActiveView->GetViewer()->setRunMaxFrameRate(oldFrameRate);
	return retVal;
}

HWND CTabPagedDialog::ShowWnd()
{
	m_pImpl->ShowWindow(SW_SHOW);
	return m_pImpl->GetSafeHwnd();
}

void CTabPagedDialog::PutParam(LPCTSTR key, IObject *pObject)
{
	for (auto page : m_arrSubPages)
	{
		page->PutParam(key, pObject);
	}
}
