#include "pch.h"
#include "TaskSystem.h"
#include "../Model/Task.h"
#include "../Model/UpdateGridEvent.h"
#include "../Utility/TaskSaveUtility.h"

void CTaskSystem::Init()
{
	GetUtility<CTaskSaveUtility>()->LoadData(_taskList);
}

void CTaskSystem::AddTask()
{
	auto task = std::make_shared<Task>();
	task->strName = L"新任务(双击重命名)";
	task->bComplete = false;
	task->bFixed = false;
	_taskList.push_back(task);
	SendEvent<UpdateGridEvent>();
	Save();
}

void CTaskSystem::RemoveTask(std::shared_ptr<Task> task)
{
	for (auto iter = _taskList.begin(); iter != _taskList.end(); ++iter)
	{
		if ((*iter)->strName == task->strName)
		{
			_taskList.erase(iter);
			SendEvent<UpdateGridEvent>();
			Save();
			return;
		}
	}
}

std::shared_ptr<Task> CTaskSystem::GetTask(int idx)
{
	return _taskList[idx];
}

int CTaskSystem::GetTaskCnt()
{
	return _taskList.size();
}

bool CTaskSystem::IsEmpty()
{
	return _taskList.empty();
}

void CTaskSystem::Save()
{
	GetUtility<CTaskSaveUtility>()->SaveData(_taskList);
}
