#pragma once

/**
*  @class    ≥¨¡¥Ω”¿‡
*
*  @brief
*/
class COMMONCONTROLS_API CHyperLink : public CStatic
{
public:
    CHyperLink();
    virtual ~CHyperLink();

public:

    void SetURL(CString strURL);
    CString GetURL() const;

    void SetColours(COLORREF crLinkColour, COLORREF crVisitedColour, 
                    COLORREF crHoverColour = -1);
    COLORREF GetLinkColour() const;
    COLORREF GetVisitedColour() const;
    COLORREF GetHoverColour() const;

    void SetVisited(BOOL bVisited = TRUE);
    BOOL GetVisited() const;

    void SetLinkCursor(HCURSOR hCursor);
    HCURSOR GetLinkCursor() const;

    void SetUnderline(BOOL bUnderline = TRUE);
    BOOL GetUnderline() const;

    void SetAutoSize(BOOL bAutoSize = TRUE);
    BOOL GetAutoSize() const;

public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    virtual void PreSubclassWindow();

protected:
    void PositionWindow();
    void SetDefaultCursor();

protected:
    COLORREF m_crLinkColour, m_crVisitedColour;     // Hyperlink colours
    COLORREF m_crHoverColour;                       // Hover colour
    BOOL     m_bOverControl;                        // cursor over control?
    BOOL     m_bVisited;                            // Has it been visited?
    BOOL     m_bUnderline;                          // underline hyperlink?
    BOOL     m_bAdjustToFit;                        // Adjust window size to fit text?
    CString  m_strURL;                              // hyperlink URL
    CFont    m_Font;                                // Underline font if necessary
    HCURSOR  m_hLinkCursor;                         // Cursor for hyperlink
    CToolTipCtrl m_ToolTip;                         // The tooltip

protected:
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnClicked();
    DECLARE_MESSAGE_MAP()
};