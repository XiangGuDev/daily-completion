
//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           StaticEx.H
//    @author         fhj
//    @date           2015/9/12 14:36
//    @brief          静态文本扩展控件
//*****************************************************

#pragma once

#define GIMCHECK_BUTTONCHICK (WM_USER + 2344)
#define GIMCHECK_EDITCHANGE (WM_USER + 2345)
#define STATIC_ONSIZE (WM_USER + 2346)
#define ID_GIMCHECK_BUTTON	32832
#define ID_GIMCHECK_EDIT	32833

/**
*  @class CStaticCtrl;
*
*  @brief 扩展的Static, 可以指定其点击事件的消息响应窗口;
*/
class COMMONCONTROLS_API CStaticEx : public CStatic
{

public:
	CStaticEx(void);

	~CStaticEx(void);

	DECLARE_MESSAGE_MAP()

	DECLARE_DYNAMIC(CStaticEx)

	afx_msg void OnBtnClick();
	afx_msg void OnEnChangeEdit();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	/**
	*  @brief    设置信息父窗口
	*
	*  @param    CWnd * pParent
	*/
	void SetMsgParent(CWnd* pParent);

private:
	CWnd*	_pParent;		///< 父窗口
};