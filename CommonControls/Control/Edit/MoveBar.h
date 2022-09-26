#pragma once

/**
*  @class    ¸¡¶¯¹¤¾ßÀ¸
*
*  @brief
*/
class COMMONCONTROLS_API CMoveBar : public CToolBar
{
protected:
	DECLARE_DYNAMIC(CMoveBar)

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnNcHitTest(CPoint pt);
};