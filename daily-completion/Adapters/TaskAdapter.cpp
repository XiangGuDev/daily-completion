#include "pch.h"
#include "../System/TaskSystem.h"
#include "../Model/Task.h"
#include "TaskAdapter.h"
#include "../Model/GlobalModel.h"

using namespace ControlUI;

CTaskAdapter::CTaskAdapter(std::shared_ptr<Task> task, bool bRoot)
{
	_bRoot = bRoot;
	_task = task;
}

size_t CTaskAdapter::GetCount(CTreeListConfig * pConfig)
{
	if (_bRoot)
	{
		return GetSystem<CTaskSystem>()->GetTaskCnt();
	}
	return 0;
}

size_t CTaskAdapter::GetKey()
{
	return (size_t)_task.get();
}

IListDataSource * CTaskAdapter::GetItem(size_t nIndex, bool & bShowItem, HITEMDATA hItemData, CTreeListConfig * pConfig)
{
	if (!_bRoot)
	{
		return NULL;
	}
	
	auto taskSys = GetSystem<CTaskSystem>();
	auto task = taskSys->GetTask(nIndex);
	CTaskAdapter *pChildSource = new CTaskAdapter(task, false);
	CString strKey = GetModel<CGlobalModel>()->SearchKey.Get();
	bShowItem = task->strName.Find(strKey) > -1;
	return pChildSource;
}

void CTaskAdapter::GetCellData(size_t nCol, size_t nRow, const HCOLUMNDATA hColumnData, HCELLINFO hCellData, HEDITCONFIG hEditConfig, CTreeListConfig * pConfig)
{
	// 直接复制表头配置
	hEditConfig->CopyFrom(hColumnData->editConfig);

	CString strField = hEditConfig->field;
	if (strField == L"序号")
	{
		hCellData->textColor = RGB(115, 115, 115);
	}
	if (_task && _task->bComplete)
	{
		hCellData->backColor = RGB(111, 225, 111);
	}
	else if (nRow % 2 == 0)
	{
		hCellData->backColor = RGB(225, 225, 225);
	}
	else
	{
		hCellData->backColor = RGB(200, 200, 200);
	}
}

CString CTaskAdapter::GetCellText(size_t nCol, size_t nRow, const HEDITCONFIG hEditConfig, CTreeListConfig * pConfig)
{
	if (_bRoot)return L"";
	CString strField = hEditConfig->field;
	if (strField == L"序号")
	{
		return CConvert::Int2Text(nRow + 1);
	}
	else if (strField == L"任务名称")
	{
		return _task->strName; 
	}
	return L"";
}

bool CTaskAdapter::SetCellText(size_t nCol, size_t nRow, const HEDITCONFIG hEditConfig, const CString & strNewValue, CTreeListConfig * pConfig)
{
	CString strField = hEditConfig->field;
	if (strField == L"任务名称")
	{
		_task->strName = strNewValue;
	}
	return true;
}

