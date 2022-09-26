/*---------------------------------------------------------------------
*文件名称：TimeLineSplitCtrl.h
*功能描述：时间轴控件需要的派生类分隔条控件
*创建标识：陶子龙2021.9.28.
*
*修改标识：
*修改描述：
----------------------------------------------------------------------*/
#pragma once

class CTimeLineSplitCtrl :public CSplitCtrl
{
public:
	CTimeLineSplitCtrl(void);

protected:
	DECLARE_MESSAGE_MAP()

public:
	/*
	* 函数介绍：更新滚动条
	* 输入参数：void
	* 输出参数：void
	* 返回值  ：void
	*/
	void UpdateScrollBar(void);

	/*
	* 函数介绍：设置对应的时间轴控件中的右侧时间帧控件
	* 输入参数：CTimeFrameListCtrl *pTimeFrameCtrl 对应的时间轴控件中的右侧时间帧控件
	* 输出参数：void
	* 返回值  ：void
	*/
	void SetCorrespondingTimeFrameCtrl(CTimeFrameListCtrl *pTimeFrameCtrl);

protected:
	//纵向滚动条响应事件
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

protected:
	CTimeFrameListCtrl*				_pTimeLineCtrl;					// 对应的时间轴控件中的右侧时间帧控件
};