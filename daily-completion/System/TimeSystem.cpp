#include "pch.h"
#include "TimeSystem.h"

void CTimeSystem::SetTime(const int &m, const int &s)
{
	_m = m;
	_s = s;
}

void CTimeSystem::GetTime(int &m, int &s)
{
	m = _m;
	s = _s;
}

void CTimeSystem::GoStep(int s)
{
	if (_m == 0 && _s == 0 && s < 0)
		return;

	_s += s;
	int carry = 0;
	if (_s >= 0)
	{
		carry = _s / 60;
		_s = _s % 60;
	}
	else
	{
		carry -= (60 - _s) / 60;
		_s = 60 - (60 - _s) % 60;
	}
	_m += carry;
	if (_m < 0)
	{
		_m = _s = 0;
	}
}

