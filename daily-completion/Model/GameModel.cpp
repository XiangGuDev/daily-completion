#include "pch.h"
#include "GameModel.h"
#include "../App/PointApp.h"
#include "../Storage/PlayerPrefsStorage.h"

GameModel::GameModel()
	: _cnt(10)
	, _score(0)
	, _gold(0)
{
	auto storage = PointApp::Instance()->GetUtility<PlayerPrefsStorage>();
	if (storage->DataExist(L"EnemyCnt"))
	{
		_cnt.Set(storage->LoadInt(L"EnemyCnt"));
	}
	else
	{
		_cnt.Set(10);
	}
	_cnt.OnValueChanged += [&](int oldVal, int val) {
		auto storage = PointApp::Instance()->GetUtility<PlayerPrefsStorage>();
		storage->SaveInt(L"EnemyCnt", val);
	};
}