#pragma once
#include "YFramework/System/BaseSystem.h"

using namespace YFramework;
class IAchievementSystem
{

};
class AchievementSystem : public BaseSystem, public IAchievementSystem
{
protected:
	virtual void Init() override;
};

