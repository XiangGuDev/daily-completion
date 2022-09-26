#include "stdafx.h"
#include "GanttNode.h"

CString CGanttNode::GetGrowthPatternString()
{
	int nGrowthPattern = GetGrowthPattern();
	switch (nGrowthPattern)
	{
	case GROWTH_BOTTOM2TOP:
		return STR_BOTTOM2TOP;
	case GROWTH_TOP2BOTTOM:
		return STR_TOP2BOTTOM;
	case GROWTH_LEFT2RIGHT:
		return STR_LEFT2RIGHT;
	case GROWTH_RIGHT2LEFT:
		return STR_RIGHT2LEFT;
	case GROWTH_MIDDLE2SIDES:
		return STR_MIDDLE2SIDES;
	default:
		return _T("");
	}
}

void CGanttNode::SetGrowthPattern(const CString &strPattern)
{
	if (strPattern == STR_BOTTOM2TOP)
		SetGrowthPattern( GROWTH_BOTTOM2TOP);
	else if (strPattern == STR_TOP2BOTTOM)
		SetGrowthPattern( GROWTH_TOP2BOTTOM);
	else if (strPattern == STR_LEFT2RIGHT)
		SetGrowthPattern( GROWTH_LEFT2RIGHT);
	else if (strPattern == STR_RIGHT2LEFT)
		SetGrowthPattern( GROWTH_RIGHT2LEFT);
	else if (strPattern == STR_MIDDLE2SIDES)
		SetGrowthPattern( GROWTH_MIDDLE2SIDES);
	else
		SetGrowthPattern( GROWTH_BOTTOM2TOP);
}



float CGanttNode::GetPlanPercent(double curTime)
{
	double planStartTime = GetPlanStartTime();
	double planEndTime = GetPlanEndTime();

	double fDivident = curTime - planStartTime;
	double nDivisor = planEndTime - planStartTime;
	if (nDivisor <= 0)
		return 1;
	if (fDivident <= 0)
		return 0;
	double fPercent = fDivident / (float)nDivisor;
	if (fPercent > 1)
		fPercent = 1;
	return fPercent;
}

float CGanttNode::GetRealPercent(double curTime)
{
	double realStartTime = GetRealStartTime();
	double realEndTime = GetRealEndTime();

	double fDivident = curTime - realStartTime;
	double nDivisor = realEndTime - realStartTime;
	if (nDivisor <= 0)
		return 1;
	if (fDivident <= 0)
		return 0;
	double fPercent = fDivident / (float)nDivisor;
	if (fPercent > 1)
		fPercent = 1;
	return fPercent;
}



void CGanttNode::SetStartTime(double dStartTime) 
{
	SetPlanStartTime(dStartTime);
	SetRealStartTime(dStartTime);
}
void CGanttNode::SetEndTime(double dEndTime)
{
	SetPlanEndTime(dEndTime);
	SetRealEndTime(dEndTime);
}
void CGanttNode::SetDuringTime(int nDuring)
{
	SetPlanDuringTime(nDuring);
	SetRealDuringTime(nDuring);
}
double CGanttNode::GetStartTime() const
{
	return GetPlanStartTime();
}
double CGanttNode::GetEndTime() const
{
	return GetPlanEndTime();
}

int CGanttNode::GetDuringTime() const
{
	return GetPlanDuringTime();
}
