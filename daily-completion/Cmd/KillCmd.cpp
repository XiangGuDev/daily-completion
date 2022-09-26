#include "pch.h"
#include "KillCmd.h"
#include "../Model/GameModel.h"
#include "../App/PointApp.h"

void CKillCmd::Excute()
{
	auto gameModel = PointApp::Instance()->Get<GameModel>("GameModel");
	auto cnt = gameModel->_cnt.Get();
	gameModel->_cnt.Set(max(0, cnt - 1));
}
