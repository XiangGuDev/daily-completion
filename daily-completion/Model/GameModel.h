#pragma once
#include "YFramework/Singleton/BaseSingleton.h"
#include "YFramework/BindableProperty/BindableProperty.h"
#include "YFramework/Model/BaseModel.h"

using namespace YFramework;

class GameModel : public BaseModel
{
public:
	GameModel();
	BindableProperty<int> _cnt;
	BindableProperty<int> _score;
	BindableProperty<int> _gold;
protected:
	virtual void Init() {};
};

