#include "stdafx.h"
#include "StaticResourceClear.h"

std::auto_ptr<StaticResource2Clear> StaticResource2Clear::_instance;

StaticResource2Clear::~StaticResource2Clear()
{
	ClearAll();
}

StaticResource2Clear* StaticResource2Clear::GetInstance()
{
	if (!_instance.get())
	{
		_instance.reset(new StaticResource2Clear());
	}

	return _instance.get();
}

void StaticResource2Clear::Register(ClearProxy* sw)
{
	_wrapperArray.Add(sw);
}

void StaticResource2Clear::ClearAll()
{
	for (int i = 0; i < (int)_wrapperArray.GetCount(); i++)
	{
		_wrapperArray[i]->Clear();
		delete _wrapperArray[i];
	}

	_wrapperArray.RemoveAll();
}