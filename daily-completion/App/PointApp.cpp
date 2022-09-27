#include "pch.h"
#include "PointApp.h"
#include "../Model/GameModel.h"
#include "../Storage/PlayerPrefsStorage.h"
#include "../System/AchievementSystem.h"
#include <WinUser.h>

class EA {
public:
	int i;
};
class EB {
public:
	int i;
};
class EC {
public:
	int i;
};

void PointApp::OnInit()
{
	RegisterUtility(std::make_shared<PlayerPrefsStorage>());
	RegisterModel(std::make_shared<GameModel>());
	RegisterSystem(std::make_shared<AchievementSystem>());
}

