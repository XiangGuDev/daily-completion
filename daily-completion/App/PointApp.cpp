#include "pch.h"
#include "PointApp.h"
#include "../Model/GameModel.h"
#include "../Storage/PlayerPrefsStorage.h"

void PointApp::OnInit()
{
	RegisterUtility(std::make_shared<PlayerPrefsStorage>());
	RegisterModel(std::make_shared<GameModel>());
}

