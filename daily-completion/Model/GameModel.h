#pragma once
#include "YFramework/Singleton/BaseSingleton.h"
#include "YFramework/BindableProperty/BindableProperty.h"

using namespace YFramework;

class GameModel
{
public:
	GameModel();
	BindableProperty<int> _cnt;
	BindableProperty<int> _score;
	BindableProperty<int> _gold;
};

