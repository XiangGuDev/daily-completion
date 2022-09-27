#pragma once
#include "YFramework/Controller/BaseController.h"
#include "../App/PointApp.h"

using namespace YFramework;

class DCController : public IController
{
public:
	virtual ~DCController(){}
private:
	virtual BaseApp * GetApp() override
	{
		return PointApp::Instance();
	}
};