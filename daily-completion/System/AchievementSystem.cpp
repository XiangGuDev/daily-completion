#include "pch.h"
#include "AchievementSystem.h"
#include "../Model/GameModel.h"

void AchievementSystem::Init()
{
	auto gameModel = GetApp()->GetModel<GameModel>();
	int preCnt = gameModel->_cnt.Get();
	gameModel->_cnt.OnValueChanged += [&](int oldVal, int val)
	{
		if (preCnt > 5 && val == 5)
		{
			MessageBox(::GetActiveWindow(), L"解锁成就[五杀]", L"恭喜！", MB_OK);
		}
		else if (preCnt > 0 && val == 0)
		{
			MessageBox(::GetActiveWindow(), L"解锁成就[杀疯了]", L"恭喜！", MB_OK);
		}
	};
}
