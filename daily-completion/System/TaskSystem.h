#pragma once
#include "YFramework/System/BaseSystem.h"

using namespace YFramework;

struct Task;
class ITaskSystem
{
public:
	virtual ~ITaskSystem(){}
	virtual void AddTask(std::shared_ptr<Task> task) = 0;
	virtual void RemoveTask(std::shared_ptr<Task> task) = 0;
	virtual std::shared_ptr<Task> GetTask(int idx) = 0;
	virtual int GetTaskCnt() = 0;
	virtual bool IsEmpty() = 0;
};

class CTaskSystem : public BaseSystem, public ITaskSystem
{
public:
	virtual void AddTask(std::shared_ptr<Task> task) override;
	virtual void RemoveTask(std::shared_ptr<Task> task) override;
	virtual std::shared_ptr<Task> GetTask(int idx) override;
	virtual int GetTaskCnt() override;
	virtual bool IsEmpty() override;
protected:
	virtual void Init() override;
private:
	std::vector<std::shared_ptr<Task>> _taskList;
};

