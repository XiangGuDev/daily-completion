#pragma once
#include "YFramework/Controller/BaseController.h"
#include "../App/PointApp.h"

using namespace YFramework;

class DCController : IController
{
public:
	virtual ~DCController(){}
protected:
	virtual BaseApp * GetApp() override
	{
		return PointApp::Instance();
	}
};