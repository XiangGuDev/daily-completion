#pragma once

#define THUMLBTNCLICKED	(WM_USER+1024)
#define THUMRBTNCLICKED	(WM_USER+1025)

class COMMONCONTROLS_API CThumbnailButton : public CWnd
{
public:
	CThumbnailButton(CWnd* pWnd, const CString& sPath, const int cx, const int cy);
	virtual ~CThumbnailButton();
	BOOL IsValid(void) const;
	const CString& GetFullpath(void) const;

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

	BOOL RegisterWindowClass(void);
	void  ResetTrackFlag(void);

private:
	BOOL    m_bTracking;
	BOOL    m_bMouseClicked;
	CString m_sFilename;
	CBitmap m_bmp;
	int     m_cX;     // Image Width
	int     m_cY;     // Image Height
	CString m_sFullpath;
	static CBrush  m_bkBrush;
	CWnd* m_pWnd; // 用于通知事件的窗口
};