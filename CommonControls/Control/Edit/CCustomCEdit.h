#pragma once

class COMMONCONTROLS_API CCustomCEdit : public CValidEditEx
{
	DECLARE_DYNAMIC(CCustomCEdit)
public:
	CCustomCEdit();
	virtual ~CCustomCEdit();

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnNcPaint();
	afx_msg void OnPaint();

public:
	void SetStatus(bool bStatus);
private:
	bool _bStatus;
};