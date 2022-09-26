#include "pch.h"
#include "PointApp.h"
#include "../Model/GameModel.h"
#include "../Storage/PlayerPrefsStorage.h"

PointApp::PointApp()
{
}

void PointApp::Init()
{
	_ioc.Register<PlayerPrefsStorage>();
	_ioc.RegisterInterface<IBaseStorage, PlayerPrefsStorage>();
	_ioc.Register<GameModel>();
}


