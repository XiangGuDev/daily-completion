#pragma once

struct Task
{
	CString strName;
	bool bComplete;
};

struct CTaskModel : public BaseModel
{
public:
	YFramework::BindableProperty<CString> SearchKey; // ËÑË÷¹Ø¼ü×Ö
	YFramework::BindableProperty<int> k; // ËÑË÷¹Ø¼ü×Ö
protected:
	virtual void Init(){}
};

class UpdateGridEvent
{

};