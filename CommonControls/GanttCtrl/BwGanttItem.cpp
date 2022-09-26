#include "stdafx.h"
#include "BwGanttItem.h"

CBwGanttItem::CBwGanttItem()
	: m_pNode(NULL)
{
	m_dtRealStart = COleDateTime::GetCurrentTime();
	m_dtRealFinish = m_dtRealStart + COleDateTimeSpan(1, 0, 0, 0);
}

void CBwGanttItem::SetRealInterval(COleDateTime dtRealStart, COleDateTime dtRealFinish)
{
	if (dtRealFinish < dtRealStart)
	{
		dtRealFinish = dtRealStart;
	}

	if (m_dtRealStart != dtRealStart || m_dtRealFinish != dtRealFinish)
	{
		m_dtRealStart = dtRealStart;
		m_dtRealFinish = dtRealFinish;
		Update(BCGP_GANTT_ITEM_PROP_START | BCGP_GANTT_ITEM_PROP_FINISH);
	}
}

void CBwGanttItem::SetRealPrimaryColor(COLORREF clr)
{
	if (m_clrRealPrimaryColor != clr)
	{
		m_clrRealPrimaryColor = clr;
		Update(BCGP_GANTT_ITEM_PROP_COLORS);
	}
}

void CBwGanttItem::SetRealCompleteColor(COLORREF clr)
{
	if (m_clrRealCompleteColor != clr)
	{
		m_clrRealCompleteColor = clr;
		Update(BCGP_GANTT_ITEM_PROP_COLORS);
	}
}

void CBwGanttItem::SetRealProgress(double dPercents)
{
	if (dPercents > 1.0) dPercents = 1.0;
	if (dPercents < 0.0) dPercents = 0.0;

	if (m_dRealProgress != dPercents)
	{
		m_dRealProgress = dPercents;
		Update(BCGP_GANTT_ITEM_PROP_PROGRESS);
	}
}

void CBwGanttItem::SetGrowthPattern(const CString &strGrowthPattern)
{
	m_strGrowthPattern = strGrowthPattern;
}

void CBwGanttItem::SetNode(KNode * pNode)
{
	m_pNode = pNode;
}
