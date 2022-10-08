#pragma once

class CTimeSystem : public YFramework::BaseSystem
{
public:
	void SetTime(const int &m, const int &s);
	void GetTime(int &m, int &s);
	void GoStep(int s = 1);
protected:
	virtual void Init() override {}
private:
	int _m;
	int _s;
};

