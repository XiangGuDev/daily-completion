#pragma once
#include <YFramework/IOCContainer/IOCContainer.h>
#include "YFramework/Singleton/BaseSingleton.h"
using namespace YFramework;
class PointApp : public BaseSingleton<PointApp>
{
	friend BaseSingleton<PointApp>;
	PointApp() = default;
public:
	void Init();

	template<typename T, typename... Args>
	T* Get(const std::string& strKey, Args&&... args);
private:
	IocContainer _ioc;
};

