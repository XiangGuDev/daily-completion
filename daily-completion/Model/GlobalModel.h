#pragma once
struct CGlobalModel : public BaseModel
{
public:
	YFramework::BindableProperty<CString> SearchKey; // ËÑË÷¹Ø¼ü×Ö
protected:
	virtual void Init() {}
};
