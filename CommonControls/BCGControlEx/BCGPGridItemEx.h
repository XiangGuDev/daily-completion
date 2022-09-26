#pragma once
#include "BCGPGridCtrl.h"
////////////////////////////////////////////////////////////////////////////\
// CBCGPGridSpinItem object

class CBCGPSpinButtonCtrlEx : public CBCGPSpinButtonCtrl
{
	DECLARE_DYNAMIC(CBCGPSpinButtonCtrlEx)

public:
	CBCGPSpinButtonCtrlEx();
	virtual ~CBCGPSpinButtonCtrlEx();

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

class CBCGPGridSpinEdit : public CBCGPEdit
{
	DECLARE_DYNAMIC(CBCGPGridSpinEdit)
public:
	CBCGPGridSpinEdit();
	virtual ~CBCGPGridSpinEdit();

	void RefBySpinButtonDown(bool bIsUp);

protected:
	afx_msg BOOL OnChange();
	DECLARE_MESSAGE_MAP()
};

class CONTROLSAPI_EXPORT CBCGPGridSpinItem : public CBCGPGridItem
{
	//DECLARE_DYNAMIC(CBCGPGridItemEx)
	DECLARE_SERIAL(CBCGPGridSpinItem)
	// Construction
protected:
	CBCGPGridSpinItem();

public:
	CBCGPGridSpinItem(const _variant_t& varValue, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL,
		LPCTSTR lpszValidChars = NULL);

	virtual ~CBCGPGridSpinItem();
	virtual CString FormatItem();
	virtual CEdit* NewInPlaceEdit();
	virtual CSpinButtonCtrl* CreateSpinControl(CRect rectSpin);
	//virtual void DoClickButton(CPoint point);
	// Overrides
public:
	void EnableSpinControl(BOOL bEnable = TRUE, int nMin = 0, int nMax = 0);

	// Attributes
public:

protected:
};


