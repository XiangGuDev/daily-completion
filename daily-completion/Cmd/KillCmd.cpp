#include "pch.h"
#include "../Model/GameModel.h"
#include "KillCmd.h"
#include "../App/PointApp.h"

void CKillCmd::Init()
{
}

void CKillCmd::Execute()
{
	auto gameModel = GetApp()->GetModel<GameModel>();
	auto cnt = gameModel->_cnt.Get();
	gameModel->_cnt.Set(max(0, cnt - 1));
}
