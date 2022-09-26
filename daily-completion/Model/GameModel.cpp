#include "pch.h"
#include "GameModel.h"
#include "../App/PointApp.h"
#include "../Storage/IBaseStorage.h"

GameModel::GameModel()
	: _cnt(10)
	, _score(0)
	, _gold(0)
{
	auto storage = PointApp::Instance()->Get<IBaseStorage>();
	auto cnt = storage->LoadInt("EnemyCnt");
	_cnt.Set(cnt);
	_cnt.OnCountChanged += [&](int val) {
		storage->SaveInt("EnemyCnt", val);
	};
}