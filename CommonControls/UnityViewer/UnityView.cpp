#include "stdafx.h"
#include "UnityView.h"

using d3s::designfile::document::DesignFile;


/*-- class CUnityView --*/


CUnityView::CUnityView(d3s::designfile::model::DesignModel *pModel, d3s::share_ptr<d3s::views::view::CViewInfo> pInfo)
	:_pUnityDlg(NULL)
	, _bMouseTracking(false)
{
	SetViewTitle(L"Unity视图");
	SetToolbarConfig(L"UnityViewToolBar.xml");
}


CUnityView::~CUnityView()
{

}


bool CUnityView::CreateViewport(HWND parent)
{
	d3s::views::view::IViewPort::CreateViewport(parent);

	if (!CView::Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), CWnd::FromHandle(parent), AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create welcome view window\n");
		return false;
	}
	ModifyStyleEx(WS_EX_CLIENTEDGE, WS_CHILD | SS_NOTIFY);

	INIT_RESHANDLE();
	_pUnityDlg = CUnityViewPort::Instance();

	if (!_pUnityDlg)
	{
		return false;
	}

	if (_pUnityDlg->GetSafeHwnd() == NULL)
	{
		_pUnityDlg->Create(CUnityViewPort::IDD, this);
	}

	_pUnityDlg->ShowWindow(SW_SHOW);
	_pUnityDlg->SetParent(this);

	return true;
}


CString CUnityView::GetToolbarConfig()
{
	CString strPath = CPathConfig::GetConfigPath() + _strToolConfigBarFile;
	return strPath;
}

void CUnityView::SetToolbarConfig(CString strToolBarFile)
{
	ASSERT(m_hWnd == NULL);
	_strToolConfigBarFile = strToolBarFile;
}


int CUnityView::GetThreadID()
{
	return d3s::platform::frame::CFrameManager::GetMainFrame()->GetThreadID();
}


void CUnityView::PostNcDestroy()
{
}


void CUnityView::OnDraw(CDC* pDC)
{
}

CString CUnityView::GetViewName()
{
	return _T("UnityView");
}

HWND CUnityView::GetHwnd()
{
	return m_hWnd;
}

CString CUnityView::GetViewTitle()
{
	return _strTitle;
}

void CUnityView::SetViewTitle(CString strViewTitle)
{
	_strTitle = strViewTitle;
}

void CUnityView::OnSize(UINT nFlags, int cx, int cy)
{
	CView::OnSize(nFlags, cx, cy);
	if (_pUnityDlg->GetSafeHwnd() != NULL)
	{
		_pUnityDlg->SetWindowPos(&wndTop, 0, 0, cx, cy, 0);
	}
}


void CUnityView::OnDestroy()
{
	// 设置活动的视图
	d3s::views::view::CViewManager::GetManager().MouseEnterView(NULL);
}

BEGIN_MESSAGE_MAP(CUnityView, CView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



void CUnityView::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	_bMouseTracking = FALSE;
	if (_pUnityDlg)
	{
		_pUnityDlg->SendMessage(WM_KILLFOCUS);
	}
	__super::OnMouseLeave();
}


void CUnityView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!_bMouseTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;

		_bMouseTracking = _TrackMouseEvent(&tme);
	}

	__super::OnMouseMove(nFlags, point);
}
