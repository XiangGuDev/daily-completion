#include "pch.h"
#include "../Model/GameModel.h"
#include "KillCmd.h"
#include "../App/PointApp.h"

void CKillCmd::Excute()
{
	auto gameModel = PointApp::Instance()->GetModel<GameModel>();
	auto cnt = gameModel->_cnt.Get();
	gameModel->_cnt.Set(max(0, cnt - 1));
}
