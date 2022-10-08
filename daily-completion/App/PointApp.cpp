#include "pch.h"
#include "PointApp.h"
#include <WinUser.h>
#include "../System/TaskSystem.h"
#include "../Model/GlobalModel.h"
#include "../Utility/TaskSaveUtility.h"
#include "../System/TimeSystem.h"
#include "../Utility/LayoutUtility.h"

void PointApp::OnInit()
{
	RegisterUtility(std::make_shared<CTaskSaveUtility>());
	RegisterModel(std::make_shared<CGlobalModel>());
	RegisterSystem(std::make_shared<CTaskSystem>());
	RegisterSystem(std::make_shared<CTimeSystem>());
}

