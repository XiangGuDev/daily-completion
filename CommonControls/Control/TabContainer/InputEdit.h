#pragma once
#define WM_END_EDIT (WM_USER+1)

class IValidatorChecker
{
public:
	virtual bool CheckCharactorValidator(LPCTSTR info, CString &errInfo) = 0;
};
class COMMONCONTROLS_API CInnerEdit :public CEdit
{
public:
	CInnerEdit(IValidatorChecker *checker);

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnTimer(UINT_PTR timer);
	void ShowBalloonTip(const CString& strTitle, const CString& strContent, int tipIcon, DWORD dwTimeOut);
protected:
	IValidatorChecker	*m_pValidator;
	bool				m_bInPaste;
};

class COMMONCONTROLS_API CInputEdit : public CWnd, public IValidatorChecker
{
public:
	CInputEdit();
	~CInputEdit();

	void ShowDefText(LPCTSTR szDefTxt);
	virtual CString PreShowText(LPCTSTR szText);
	virtual CString AppEditText(LPCTSTR szEditedText);
	virtual bool CheckCharactorValidator(LPCTSTR info, CString &errInfo);
	void EndEdit();
	void CreateWnd(CWnd *pParent);
	void SetEditorFont(CFont *pFnt);
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

protected:
	

protected:
	CInnerEdit m_Edit;
};

