#include "pch.h"
#include "PointApp.h"
#include <WinUser.h>
#include "../System/TaskSystem.h"
#include "../Model/Task.h"

void PointApp::OnInit()
{
	RegisterModel(std::make_shared<CTaskModel>());
	RegisterSystem(std::make_shared<CTaskSystem>());
}

