#pragma once

#include <vector>

#include "ThumbnailButton.h"

#define	DEFAULT_THUMBWIDTH 100
#define	DEFAULT_THUMBHEIGHT 100
#define	DEFAULT_SEPERATOR 4

class COMMONCONTROLS_API CThumbNailControl : public CWnd
{
public:
	CThumbNailControl();
	virtual ~CThumbNailControl();

	void Add(const CString& sPath);
	void Delete(CThumbnailButton* pBtn);
	void InitializeVariables(CWnd* pWnd, int cX = DEFAULT_THUMBWIDTH, int cY = DEFAULT_THUMBHEIGHT);
	void Clear();
	int GetSize();

protected:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	DECLARE_MESSAGE_MAP()

	int GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
	BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);
	void RecalScrollBars(void);
	BOOL RegisterWindowClass();
	void RecalButtonPos(void);

private:
	static CBrush     m_bkBrush;
	std::vector<CThumbnailButton*>         m_arPtrData;
	int m_iStartX;
	int m_iStartY;
	int m_iCol;
	int m_iRow;
	int m_iThumbWidth;
	int m_iThumbHeight;
	CWnd* m_pWnd; // 用于通知事件的窗口
};