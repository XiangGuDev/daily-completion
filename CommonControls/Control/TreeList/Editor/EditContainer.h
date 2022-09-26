//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           EditContainer.H
//    @author         jzl
//    @data           2015/7/7 17:08
//    @brief          编辑控件容器
//*****************************************************

#pragma once

/**
*  @class    CEditContainer
*
*  @brief    编辑控件容器
*/
class COMMONCONTROLS_API CEditContainer : public CStatic
{
public:
	CEditContainer();
	virtual ~CEditContainer();

protected:
	DECLARE_MESSAGE_MAP()

public:
	/**
	*  @brief    创建控件;
	*
	*  @param    CRect & rect 绘制区域;
	*  @param    CWnd * pParent 父窗口句柄;
	*  @return   BOOL 创建成功返回true，否则返回false;
	*/
	BOOL Create(CRect& rect, CWnd* pParent);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
	COLORREF _bkColor;	///< 背景色
};