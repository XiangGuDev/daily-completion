#include "pch.h"
#include "PointApp.h"
#include <WinUser.h>
#include "../System/TaskSystem.h"

void PointApp::OnInit()
{
	RegisterSystem(std::make_shared<CTaskSystem>());
}

