#include "pch.h"
#include "Player3.h"
#include "resourcepath.h"

bool Player3::Initialize()
{
	if(!base::Initialize()) { return false; }
	_handle = Load::LoadModel(path::Player("Player"));
	_attach_index = -1;
	// ステータスを「無し」に設定
	_status = STATUS::NONE;
	// 再生時間の初期化
	_total_time = 0.0f;
	_play_time = 0.0f;
	// 位置、向きの初期化
	_pos = v::VGet(0.0f, 0.0f, 0.0f);
	_dir = v::VGet(0.0f, 0.0f, -1.0f);// キャラモデルはデフォルトで-Z方向を向いている
	// 腰位置の設定
	_col_sub_y = 40.0f;
	// コリジョン半径の設定
	_collision_r = 30.0f;
	_collision_weight = 10.0f;
	_hp = 100;
	return true;
}

bool Player3::Terminate()
{
	base::Terminate();
	return true;
}

void Player3::CommandAttack(IBattleReceiver* target)
{
	// 攻撃コマンドの実行
}

bool Player3::IsExceutionAction() const
{
	// 攻撃コマンドの実行中かどうかを返す
	return false;
}

bool Player3::Process()
{
	base::Process();
	return true;
}

bool Player3::Render()
{
	base::Render();
	return true;
}