#include "pch.h"
#include "PointApp.h"
#include "../Model/GameModel.h"

void PointApp::Init()
{
	_ioc.Register<GameModel>("GameModel");
}

template<typename T, typename... Args>
T* PointApp::Get(const std::string& strKey, Args&&... args)
{
	return _ioc.Get<T>(key, args);
}