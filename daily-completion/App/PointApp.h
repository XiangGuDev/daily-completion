#pragma once
#include <YFramework/IOCContainer/IOCContainer.h>
#include "YFramework/Singleton/BaseSingleton.h"
using namespace YFramework;
class PointApp : public BaseSingleton<PointApp>
{
	friend BaseSingleton<PointApp>;
	PointApp();
public:
	void Init();

	template <typename T>
	shared_ptr<T> Get();
public:
	IocContainer _ioc;
};

template <typename T>
shared_ptr<T> PointApp::Get()
{
	return _ioc.Get<T>();
}


