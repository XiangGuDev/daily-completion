#pragma once
class COMMONCONTROLS_API CColorStatic : public CStatic
{
public:
	CColorStatic();
	~CColorStatic();

	/**
	*	@brief	
	*/
	void SetBgColor(COLORREF cr);

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_colorBg;
};

