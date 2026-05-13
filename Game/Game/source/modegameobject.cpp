#include "modegame.h"
#include "player.h"
#include "cube.h"
#include "map.h"

// オブジェクトの初期化
// オブジェクトの初期化
bool ModeGame::ObjectInitialize()
{
	// キューブ初期化
	for(int i = 0; i < CUBE_COUNT; i++)
	{
		auto cube = std::make_shared<Cube>();
		_object.emplace_back(cube);
		_cube.emplace_back(cube);
	}

	// マップ初期化
	_map = std::make_shared<Map>();
	_object.emplace_back(_map);

	// プレイヤー初期化
	_player = std::make_shared<Player>();
	_chara.emplace_back(_player);

	// 敵初期化
	for(int i = 0; i < ENEMY_COUNT; ++i)
	{
		auto enemy = std::make_shared<Enemy>();
		_enemy.emplace_back(enemy);
		_chara.emplace_back(enemy);
	}

	return true;
}
