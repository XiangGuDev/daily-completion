//*****************************************************
//    
//    @copyright      
//    @version        v1.0
//    @file           CGanttNode.h
//    @author         严峰
//    @data           2020/12/28  
//    @brief          4D进度节点
//
//*****************************************************
#pragma once

#define MERGE_COL_JHJD _T("计划进度")
#define MERGE_COL_SJJD _T("实际进度")

#define COL_NAME _T("名称")
#define COL_ANIMNAME _T("动画名称")
#define COL_PLAN_DURINGTIME _T("计划持续时间")
#define COL_PLAN_STARTTIME _T("计划开始时间")
#define COL_PLAN_ENDTIME _T("计划结束时间")
#define COL_REAL_DURINGTIME _T("实际持续时间")
#define COL_REAL_STARTTIME _T("实际开始时间")
#define COL_REAL_ENDTIME _T("实际结束时间")

#define COL_PLAN_PRIMARY_COLOR _T("计划未完成颜色")
#define COL_PLAN_FINISH_COLOR _T("计划完成颜色")
#define COL_REAL_PRIMARY_COLOR _T("实际未完成颜色")
#define COL_REAL_FINISH_COLOR _T("实际完成颜色")

#define COL_GROWTH_PATTERN _T("生长方式")
#define COL_MODEL_NUM _T("模型数量")
#define COL_OPERATION _T("操作")

#define COL_LAYER _T("层级")

#define COL_PRETASK _T("前置任务")
#define COL_RESOURCELIST _T("资源列表")
#define COL_RESOURCEEFFECT _T("资源外观")


#define GROWTH_BOTTOM2TOP	1
#define GROWTH_TOP2BOTTOM	2
#define GROWTH_LEFT2RIGHT	3
#define GROWTH_RIGHT2LEFT	4
#define GROWTH_MIDDLE2SIDES 5

#define OPERATION_RELEASEBIND _T("解绑")

#define STR_BOTTOM2TOP _T("从下往上")
#define STR_TOP2BOTTOM _T("从上往下")
#define STR_LEFT2RIGHT _T("从左往右")
#define STR_RIGHT2LEFT _T("从右往左")
#define STR_MIDDLE2SIDES _T("从中心往两边")

#define SAVE_FILE_NAME _T("施工进度方案记录.xml")

#define e4DProgressNode 40000

class COMMONCONTROLS_API CGanttNode : public PlatformKernel::BnsWrapper
{
public:
	CGanttNode() {};
	CGanttNode(KNode* pNode) : BnsWrapper(pNode)
	{
		VERIFY(pNode == nullptr || pNode->GetTag() == e4DProgressNode);
		_pNode = pNode;
	}
	virtual ~CGanttNode() {};

public:
	DefinePropertyT(Name, COL_NAME, CString);
	DefinePropertyT(PlanPrimaryColor, COL_PLAN_PRIMARY_COLOR, int);
	DefinePropertyT(PlanFinishColor, COL_PLAN_FINISH_COLOR, int);
	DefinePropertyT(RealPrimaryColor, COL_REAL_PRIMARY_COLOR, int);
	DefinePropertyT(RealFinishColor, COL_REAL_FINISH_COLOR, int);
	DefinePropertyT(GrowthPattern, COL_GROWTH_PATTERN, int);
	DefinePropertyT(ModelNum, COL_MODEL_NUM, int);
	DefinePropertyT(Operation, COL_OPERATION, CString);

	DefinePropertyT(PreTask, COL_PRETASK, CString);
	DefinePropertyT(ResourceList, COL_RESOURCELIST, CString);
	DefinePropertyT(ResourceEffect, COL_RESOURCEEFFECT, CString);

	virtual CString GetGrowthPatternString();

	// 设置生长方式
	virtual void SetGrowthPattern(const CString &strPattern);

	// 获取实际持续时间
	virtual int GetRealDuringTime() const
	{
		return (int)_pNode->GetData(COL_REAL_DURINGTIME);
	}

	// 设置实际持续时间
	virtual void SetRealDuringTime(int nDuring)
	{
		if (_pNode->HasData(COL_REAL_DURINGTIME) && nDuring == GetRealDuringTime())
			return;
		if (nDuring < 1)
		{
			nDuring = 1;
		}
		else if (nDuring > 9999)
		{
			nDuring = 9999;
		}
		_pNode->SetData(COL_REAL_DURINGTIME, nDuring);
		if(_pNode->HasData(COL_REAL_STARTTIME))
			SetRealEndTime(GetRealStartTime() + nDuring - 1);
	}

	// 获取计划持续时间
	virtual int GetPlanDuringTime() const
	{
		return (int)_pNode->GetData(COL_PLAN_DURINGTIME);
	}

	// 设置计划持续时间
	virtual void SetPlanDuringTime(int nDuring)
	{
		if (_pNode->HasData(COL_PLAN_DURINGTIME) && nDuring == GetPlanDuringTime())
			return;
		if (nDuring < 1)
		{
			nDuring = 1;
		}
		else if (nDuring > 9999)
		{
			nDuring = 9999;
		}
		_pNode->SetData(COL_PLAN_DURINGTIME, nDuring);
		if (_pNode->HasData(COL_PLAN_STARTTIME))
			SetPlanEndTime(GetPlanStartTime() + nDuring - 1);
	}

	// 获取计划结束时间
	virtual double GetPlanEndTime() const
	{
		return (double)_pNode->GetData(COL_PLAN_ENDTIME);
	}

	// 获取实际结束时间
	virtual double GetRealEndTime() const
	{
		return (double)_pNode->GetData(COL_REAL_ENDTIME);
	}

	// 获取计划开始时间
	virtual double GetPlanStartTime() const
	{
		return (double)_pNode->GetData(COL_PLAN_STARTTIME);
	}

	// 获取实际开始时间
	double GetRealStartTime() const 
	{
		return (double)_pNode->GetData(COL_REAL_STARTTIME);
	}

	// 设置计划开始时间
	virtual void SetPlanStartTime(double datetime)
	{
		if (_pNode->HasData(COL_PLAN_STARTTIME) && (int)datetime == (int)GetPlanStartTime())
			return;
		if (_pNode->HasData(COL_PLAN_ENDTIME) && datetime < GetPlanEndTime() - 9999 + 1)
		{
			datetime = GetPlanEndTime() - 9999 + 1;
		}
		_pNode->SetData(COL_PLAN_STARTTIME, CDataConvert::FloatQuZheng(datetime));
		
		if (_pNode->HasData(COL_PLAN_DURINGTIME) && _pNode->HasData(COL_PLAN_ENDTIME) && datetime > GetPlanEndTime())
		{
			SetPlanEndTime(datetime + GetPlanDuringTime() - 1);
		}
		if(_pNode->HasData(COL_PLAN_STARTTIME) && _pNode->HasData(COL_PLAN_ENDTIME))
			SetPlanDuringTime(GetPlanEndTime() - GetPlanStartTime() + 1);
	}

	// 设置实际开始时间
	virtual void SetRealStartTime(double datetime)
	{
		if (_pNode->HasData(COL_REAL_STARTTIME) && (int)datetime == (int)GetRealStartTime())
			return;
		if (_pNode->HasData(COL_REAL_ENDTIME) && datetime < GetRealEndTime() - 9999 + 1)
		{
			datetime = GetRealEndTime() - 9999 + 1;
		}
		_pNode->SetData(COL_REAL_STARTTIME, CDataConvert::FloatQuZheng(datetime));
		
		if (_pNode->HasData(COL_REAL_DURINGTIME) && _pNode->HasData(COL_REAL_ENDTIME) && datetime > GetRealEndTime())
		{
			SetRealEndTime(datetime + GetRealDuringTime() - 1);
		}

		if (_pNode->HasData(COL_REAL_STARTTIME) && _pNode->HasData(COL_REAL_ENDTIME))
			SetRealDuringTime(GetRealEndTime() - GetRealStartTime() + 1);
	}

	// 设置计划结束时间
	virtual void SetPlanEndTime(double datetime)
	{
		if (_pNode->HasData(COL_PLAN_ENDTIME) && (int)datetime == (int)GetPlanEndTime())
			return;
		if (_pNode->HasData(COL_PLAN_STARTTIME) && _pNode->HasData(COL_PLAN_ENDTIME) && GetPlanEndTime() >= GetPlanStartTime() + 9999)
		{
			datetime = GetPlanStartTime() + 9999 - 1;
		}
		COleDateTime tempTime = datetime;
		tempTime.SetDateTime(tempTime.GetYear(), tempTime.GetMonth(), tempTime.GetDay(), 23, 59, 59);
		_pNode->SetData(COL_PLAN_ENDTIME, (double)tempTime);

		if (_pNode->HasData(COL_PLAN_STARTTIME) && _pNode->HasData(COL_PLAN_ENDTIME) && GetPlanEndTime() < GetPlanStartTime())
		{
			SetPlanStartTime(datetime - GetPlanDuringTime() + 1);
		}
		if(_pNode->HasData(COL_PLAN_STARTTIME) && _pNode->HasData(COL_PLAN_ENDTIME))
			SetPlanDuringTime(GetPlanEndTime() - GetPlanStartTime() + 1);
	}

	// 设置实际结束时间
	virtual void SetRealEndTime(double datetime)
	{
		if (_pNode->HasData(COL_REAL_ENDTIME) && (int)datetime == (int)GetRealEndTime())
			return;
		if (_pNode->HasData(COL_REAL_ENDTIME) && _pNode->HasData(COL_REAL_STARTTIME) && GetRealEndTime() >= GetRealStartTime() + 9999)
		{
			datetime = GetRealStartTime() + 9999 - 1;
		}
		COleDateTime tempTime = datetime;
		tempTime.SetDateTime(tempTime.GetYear(), tempTime.GetMonth(), tempTime.GetDay(), 23, 59, 59);
		_pNode->SetData(COL_REAL_ENDTIME, (double)tempTime);
	
		if (_pNode->HasData(COL_REAL_ENDTIME) && _pNode->HasData(COL_REAL_STARTTIME) && GetRealEndTime() < GetRealStartTime())
		{
			SetRealStartTime(datetime - GetRealDuringTime() + 1);
		}
	
		if(_pNode->HasData(COL_REAL_ENDTIME) && _pNode->HasData(COL_REAL_STARTTIME))
			SetRealDuringTime(GetRealEndTime() - GetRealStartTime() + 1);
	}




	// 获取完成计划百分比
	virtual float GetPlanPercent(double curTime);

	// 获取完成实际百分比
	virtual float GetRealPercent(double curTime);

	// 计划实际时间统一修改接口
	virtual void SetStartTime(double dStartTime);
	virtual void SetEndTime(double dEndTime);
	virtual void SetDuringTime(int nDuring);
	virtual double GetStartTime() const;
	virtual double GetEndTime() const;
	virtual int GetDuringTime() const;
};
