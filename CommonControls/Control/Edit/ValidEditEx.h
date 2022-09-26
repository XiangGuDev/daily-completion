//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           ValidEdit.H
//    @author         KG
//    @date           2015/3/7 10:41
//    @brief          校验编辑控件
//*****************************************************

#pragma once

/**
*  @class    校验编辑控件
*
*  @brief
*/
class COMMONCONTROLS_API CValidEditEx : public CValidEdit
{
	DECLARE_DYNAMIC(CValidEditEx)
public:
	CValidEditEx();
	virtual ~CValidEditEx();

protected:
	DECLARE_MESSAGE_MAP()

	/**
	*  @brief    失去焦点
	*
	*  @param    CWnd * pNewWnd
	*  @return   void
	*/
	virtual void OnKillFocus(CWnd* pNewWnd);
};

