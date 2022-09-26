#include "stdafx.h"
#include "InputEdit.h"
#define EDIT_CTRL_ID 110
#define TIME_ID 1024

BEGIN_MESSAGE_MAP(CInputEdit, CWnd)
	ON_WM_SIZE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

CInputEdit::CInputEdit():m_Edit(this)
{
}

CInputEdit::~CInputEdit()
{
}

void CInputEdit::ShowDefText(LPCTSTR szDefTxt)
{
	ShowWindow(SW_SHOW);
	SetActiveWindow();
	m_Edit.SetFocus();
	CString finalTxt = PreShowText(szDefTxt);
	m_Edit.SetWindowText(finalTxt);
	m_Edit.SetSel(0, _tcslen(finalTxt));
}

CString CInputEdit::PreShowText(LPCTSTR szText)
{
	return CString(szText);
}

CString CInputEdit::AppEditText(LPCTSTR szEditedText)
{
	return CString(szEditedText);
}

void CInputEdit::EndEdit()
{
	if (!IsWindowVisible())
		return;
	ShowWindow(SW_HIDE);
	CString txt;
	m_Edit.GetWindowText(txt);
	txt = AppEditText(txt);
	SetWindowText(txt);
	auto parent = GetOwner();
	parent->SendMessage(WM_END_EDIT);
}

void CInputEdit::CreateWnd(CWnd *pParent)
{
	CRect rtWnd(0, 0, 100, 100);
	CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, ::LoadCursor(NULL, IDC_ARROW));
	CreateEx(0, className, L"", WS_POPUP, rtWnd, pParent, 0);
	SetOwner(pParent);
	m_Edit.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL, rtWnd, this, EDIT_CTRL_ID);
}

void CInputEdit::SetEditorFont(CFont *pFnt)
{
	m_Edit.SetFont(pFnt);
}

void CInputEdit::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	if (::IsWindow(m_Edit.GetSafeHwnd()))
	{
		CRect rtWnd;
		GetClientRect(rtWnd);
		m_Edit.MoveWindow(rtWnd);
	}
}

void CInputEdit::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	if (nState == WA_INACTIVE)
	{
		EndEdit();
	}
}

bool CInputEdit::CheckCharactorValidator(LPCTSTR info, CString &errInfo)
{
	errInfo = L"";
	return true;
}

BEGIN_MESSAGE_MAP(CInnerEdit, CEdit)
	ON_WM_SIZE()
	ON_WM_CHAR()
	ON_WM_TIMER()
END_MESSAGE_MAP()

CInnerEdit::CInnerEdit(IValidatorChecker * checker):m_pValidator(checker)
{
	m_bInPaste = false;
}

void CInnerEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// 按回车和ESC退出
	if (!m_pValidator || nChar == 0x0d || nChar == 0x1b)
	{
		__super::OnChar(nChar, nRepCnt, nFlags);
		GetParent()->GetParent()->ActivateTopParent();
		return;
	}
	ASSERT(m_pValidator);
	// 支持back删除
	if (!m_pValidator || nChar == 0x08)
	{
		__super::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	CString curTxt, tempTxt, errBuff;
	GetWindowText(curTxt);

	// 获取鼠标选中内容
	int nStart = -1, nEnd = -1;
	this->GetSel(nStart, nEnd);
	if (nStart < nEnd)
	{
		curTxt.Delete(nStart, nEnd - nStart);
	}
	for (int i = 0; i < (int)nRepCnt; ++i)
		tempTxt += (TCHAR)nChar;

	curTxt.Insert(nStart, tempTxt);

	if (m_pValidator->CheckCharactorValidator(curTxt, errBuff))
	{
		__super::OnChar(nChar, nRepCnt, nFlags);
	}

	ShowBalloonTip(L"错误", errBuff, NULL, 500);
}

void CInnerEdit::OnTimer(UINT_PTR timer)
{
	if (timer == TIME_ID)
	{
		KillTimer(TIME_ID);
		Edit_HideBalloonTip(m_hWnd);
	}
}

void CInnerEdit::ShowBalloonTip(const CString& strTitle,
	const CString& strContent, int tipIcon, DWORD dwTimeOut)
{
	EDITBALLOONTIP info = { 0 };
	info.cbStruct = sizeof(EDITBALLOONTIP);
	info.pszTitle = strTitle;
	info.pszText = strContent;
	info.ttiIcon = tipIcon;

	// 显示气泡提示
	Edit_ShowBalloonTip(m_hWnd, &info);

	// 开启关闭提示
	if (dwTimeOut != 0)
	{
		SetTimer(TIME_ID, dwTimeOut, NULL);
	}
}