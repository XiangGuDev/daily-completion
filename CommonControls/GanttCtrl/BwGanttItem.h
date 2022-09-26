//*****************************************************
//    
//    @copyright      
//    @version        v1.0
//    @file           BwGanttItem.h
//    @author         yf
//    @data           2020/12/16  
//    @brief          甘特图控件Item
//
//*****************************************************

#pragma once

#include "BCGPGanttItem.h"

class COMMONCONTROLS_API CBwGanttItem : public CBCGPGanttItem
{
	friend class CBwGanttChart;

public:
	CBwGanttItem();

	COleDateTime   GetFinishTime() const
	{
		COleDateTime tempTime = m_dtFinish;
		tempTime.SetDateTime(tempTime.GetYear(), tempTime.GetMonth(), tempTime.GetDay(), 23, 59, 59);

		return tempTime;
	}

	// 获得实际开始时间
	COleDateTime   GetRealStartTime() const
	{
		return m_dtRealStart;
	}

	// 获得实际完成时间
	COleDateTime   GetRealFinishTime() const
	{
		COleDateTime tempTime = m_dtRealFinish;
		tempTime.SetDateTime(tempTime.GetYear(), tempTime.GetMonth(), tempTime.GetDay(), 23, 59, 59);

		return tempTime;
	}

	// 设置实际开始结束时间
	void	SetRealInterval(COleDateTime dtRealStart, COleDateTime dtRealFinish);

	// 设置实际未完成颜色
	void	SetRealPrimaryColor(COLORREF clr);
	COLORREF GetRealPrimaryColor() const
	{
		return m_clrRealPrimaryColor;
	}

	// 设置实际完成颜色
	void	SetRealCompleteColor(COLORREF clr);
	COLORREF GetRealCompleteColor() const
	{
		return m_clrRealCompleteColor;
	}

	// 设置实际进度
	void	SetRealProgress(double dPercents);
	double	GetRealProgress() const
	{
		return m_dProgress;
	}

	// 设置生长方式
	void SetGrowthPattern(const CString &strGrowthPattern);
	CString GetGrowthPattern() const
	{
		return m_strGrowthPattern;
	}

	// 设置绑定节点
	void SetNode(KNode *pNode);
	
	// 获取绑定节点
	KNode *GetNode()
	{
		return m_pNode;
	}
protected:
	COleDateTime	m_dtRealStart;
	COleDateTime	m_dtRealFinish;
	COLORREF		m_clrRealPrimaryColor;
	COLORREF		m_clrRealCompleteColor;
	double			m_dRealProgress; // 0.0 .. 1.0
	CString			m_strGrowthPattern;
	KNode *			m_pNode;
};
