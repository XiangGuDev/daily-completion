#include "stdafx.h"
#include "HyperLink.h"

#define TOOLTIP_ID 1

CHyperLink::CHyperLink()
{
    m_hLinkCursor       = NULL;                 // No cursor as yet
    m_crLinkColour      = RGB(  0,   0, 238);   // Blue
    m_crVisitedColour   = RGB( 85,  26, 139);   // Purple
    m_crHoverColour     = ::GetSysColor(COLOR_HIGHLIGHT);
    m_bOverControl      = FALSE;                // Cursor not yet over control
    m_bVisited          = FALSE;                // Hasn't been visited yet.
    m_bUnderline        = TRUE;                 // Underline the link?
    m_bAdjustToFit      = TRUE;                 // Resize the window to fit the text?
    m_strURL.Empty();
}

CHyperLink::~CHyperLink()
{
    m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CHyperLink, CStatic)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_SETCURSOR()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

BOOL CHyperLink::PreTranslateMessage(MSG* pMsg) 
{
    m_ToolTip.RelayEvent(pMsg);
    return CStatic::PreTranslateMessage(pMsg);
}

HBRUSH CHyperLink::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    ASSERT(nCtlColor == CTLCOLOR_STATIC);

    if (m_bOverControl)
        pDC->SetTextColor(m_crHoverColour);
    else if (m_bVisited)
        pDC->SetTextColor(m_crVisitedColour);
    else
        pDC->SetTextColor(m_crLinkColour);

    // transparent text.
    pDC->SetBkMode(TRANSPARENT);
    return (HBRUSH)GetStockObject(NULL_BRUSH);
}

void CHyperLink::OnMouseMove(UINT nFlags, CPoint point) 
{
    CStatic::OnMouseMove(nFlags, point);

    if (m_bOverControl)        // Cursor is currently over control
    {
        CRect rect;
        GetClientRect(rect);

        if (!rect.PtInRect(point))
        {
            m_bOverControl = FALSE;
            ReleaseCapture();
            RedrawWindow();
            return;
        }
    }
    else                      // Cursor has just moved over control
    {
        m_bOverControl = TRUE;
        RedrawWindow();
        SetCapture();
    }
}

BOOL CHyperLink::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/) 
{
    if (m_hLinkCursor)
    {
        ::SetCursor(m_hLinkCursor);
        return TRUE;
    }
    return FALSE;
}

void CHyperLink::PreSubclassWindow() 
{
    // We want to get mouse clicks via STN_CLICKED
    DWORD dwStyle = GetStyle();
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
    
    // Set the URL as the window text
    if (m_strURL.IsEmpty())
        GetWindowText(m_strURL);

    // Check that the window text isn't empty. If it is, set it as the URL.
    CString strWndText;
    GetWindowText(strWndText);
    if (strWndText.IsEmpty()) {
        ASSERT(!m_strURL.IsEmpty());    // Window and URL both NULL. DUH!
        SetWindowText(m_strURL);
    }

    // Create the font
    LOGFONT lf;
    GetFont()->GetLogFont(&lf);
    lf.lfUnderline = m_bUnderline;
    m_Font.CreateFontIndirect(&lf);
    SetFont(&m_Font);

    PositionWindow();        // Adjust size of window to fit URL if necessary
    SetDefaultCursor();      // Try and load up a "hand" cursor

    // Create the tooltip
    CRect rect; 
    GetClientRect(rect);
    m_ToolTip.Create(this);
    m_ToolTip.AddTool(this, m_strURL, rect, TOOLTIP_ID);

    CStatic::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CHyperLink operations

void CHyperLink::SetURL(CString strURL)
{
    m_strURL = strURL;

    if (::IsWindow(GetSafeHwnd())) {
        PositionWindow();
        m_ToolTip.UpdateTipText(strURL, this, TOOLTIP_ID);
    }
}

CString CHyperLink::GetURL() const
{ 
    return m_strURL;   
}

void CHyperLink::SetColours(COLORREF crLinkColour, COLORREF crVisitedColour,
                            COLORREF crHoverColour /* = -1 */) 
{ 
    m_crLinkColour    = crLinkColour; 
    m_crVisitedColour = crVisitedColour;

	if (crHoverColour == -1)
		m_crHoverColour = ::GetSysColor(COLOR_HIGHLIGHT);
	else
		m_crHoverColour = crHoverColour;

    if (::IsWindow(m_hWnd))
        Invalidate(); 
}

COLORREF CHyperLink::GetLinkColour() const
{ 
    return m_crLinkColour; 
}

COLORREF CHyperLink::GetVisitedColour() const
{
    return m_crVisitedColour; 
}

COLORREF CHyperLink::GetHoverColour() const
{
    return m_crHoverColour;
}

void CHyperLink::SetVisited(BOOL bVisited /* = TRUE */) 
{ 
    m_bVisited = bVisited; 

    if (::IsWindow(GetSafeHwnd()))
        Invalidate(); 
}

BOOL CHyperLink::GetVisited() const
{ 
    return m_bVisited; 
}

void CHyperLink::SetLinkCursor(HCURSOR hCursor)
{ 
    m_hLinkCursor = hCursor;
    if (m_hLinkCursor == NULL)
        SetDefaultCursor();
}

HCURSOR CHyperLink::GetLinkCursor() const
{
    return m_hLinkCursor;
}

void CHyperLink::SetUnderline(BOOL bUnderline /* = TRUE */)
{
    m_bUnderline = bUnderline;

    if (::IsWindow(GetSafeHwnd()))
    {
        LOGFONT lf;
        GetFont()->GetLogFont(&lf);
        lf.lfUnderline = m_bUnderline;

        m_Font.DeleteObject();
        m_Font.CreateFontIndirect(&lf);
        SetFont(&m_Font);

        Invalidate(); 
    }
}

BOOL CHyperLink::GetUnderline() const
{ 
    return m_bUnderline; 
}

void CHyperLink::SetAutoSize(BOOL bAutoSize /* = TRUE */)
{
    m_bAdjustToFit = bAutoSize;

    if (::IsWindow(GetSafeHwnd()))
        PositionWindow();
}

BOOL CHyperLink::GetAutoSize() const
{ 
    return m_bAdjustToFit; 
}

void CHyperLink::PositionWindow()
{
	return;
    if (!::IsWindow(GetSafeHwnd()) || !m_bAdjustToFit) 
        return;

    // Get the current window position
    CRect rect;
    GetWindowRect(rect);

    CWnd* pParent = GetParent();
    if (pParent)
        pParent->ScreenToClient(rect);

    // Get the size of the window text
    CString strWndText;
    GetWindowText(strWndText);

    CDC* pDC = GetDC();
    CFont* pOldFont = pDC->SelectObject(&m_Font);
    CSize Extent = pDC->GetTextExtent(strWndText);
    pDC->SelectObject(pOldFont);
    ReleaseDC(pDC);

    // Get the text justification via the window style
    DWORD dwStyle = GetStyle();

    // Recalc the window size and position based on the text justification
    if (dwStyle & SS_CENTERIMAGE)
        rect.DeflateRect(0, (rect.Height() - Extent.cy)/2);
    else
        rect.bottom = rect.top + Extent.cy;

    if (dwStyle & SS_CENTER)   
        rect.DeflateRect((rect.Width() - Extent.cx)/2, 0);
    else if (dwStyle & SS_RIGHT) 
        rect.left  = rect.right - Extent.cx;
    else // SS_LEFT = 0, so we can't test for it explicitly 
        rect.right = rect.left + Extent.cx;

    // Move the window
    SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);
}

void CHyperLink::SetDefaultCursor()
{
    if (m_hLinkCursor == NULL)                // No cursor handle - load our own
    {
		m_hLinkCursor = LoadCursor(NULL, IDC_HAND);
        //// Get the windows directory
        //CString strWndDir;
        //GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
        //strWndDir.ReleaseBuffer();

        //strWndDir += _T("\\winhlp32.exe");
        //// This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
        //HMODULE hModule = LoadLibrary(strWndDir);
        //if (hModule) {
        //    HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
        //    if (hHandCursor)
        //        m_hLinkCursor = CopyCursor(hHandCursor);
        //}
        //FreeLibrary(hModule);
    }
}