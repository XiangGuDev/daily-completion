#pragma once
#include <YFramework/IOCContainer/IOCContainer.h>
#include "YFramework/App/BaseApp.h"

using namespace YFramework;

class PointApp : public BaseApp, public BaseSingleton<PointApp>
{
	friend BaseSingleton<PointApp>;
	PointApp() {}
public:
	void Init();
};




