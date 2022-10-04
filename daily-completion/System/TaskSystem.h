#pragma once
#include "YFramework/System/BaseSystem.h"

using namespace YFramework;

struct Task;
class CTaskSystem : public BaseSystem
{
public:
	void AddTask() ;
	void RemoveTask(std::shared_ptr<Task> task) ;
	std::shared_ptr<Task> GetTask(int idx) ;
	int GetTaskCnt() ;
	bool IsEmpty() ;
	void Save() ;
	void SetDate(int y, int m, int d);
	const COleDateTime &GetDate() { return _time; }
protected:
	virtual void Init() ;
private:
	std::vector<std::shared_ptr<Task>> _taskList;
	COleDateTime _time;
};

