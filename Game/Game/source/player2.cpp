#include "pch.h"
#include "player2.h"
#include "resourcepath.h"

bool Player2::Initialize()
{
	if(!base::Initialize()) { return false; }
	_handle = Load::LoadModel(path::Player("Player"));
	_attach_index = -1;
	_status = STATUS::NONE;
	_total_time = 0.0f;
	_play_time = 0.0f;
	_pos = v::VGet(0.0f, 0.0f, 0.0f);
	_dir = v::VGet(0.0f, 0.0f, -1.0f);
	_col_sub_y = 40.0f;
	_collision_r = 30.0f;
	_collision_weight = 10.0f;
	_hp = 100;
	return true;
}

bool Player2::Terminate()
{
	base::Terminate();
	return true;
}

void Player2::CommandAttack(IBattleReceiver* target)
{
	// 攻撃コマンドの実行
}

bool Player2::IsExceutionAction() const
{
	// 攻撃コマンドの実行中かどうかを返す
	return false;
}

bool Player2::Process()
{
	base::Process();
	return true;
}

bool Player2::Render()
{
	base::Render();
	return true;
}