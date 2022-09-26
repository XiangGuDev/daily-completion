#include "pch.h"
#include "PointApp.h"
#include "../Model/GameModel.h"

PointApp::PointApp()
{
	Init();
}


void PointApp::Init()
{
	_ioc.Register<GameModel>();
}


