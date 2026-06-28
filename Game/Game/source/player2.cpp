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

	_anim = AddComponent<AnimationComponent>();
	_anim->SetAnimation(
		{
			AnimationClip("mot_attack_charge_loop"),	// NONE
			AnimationClip("mot_attack_charge_loop"),	// WAIT
			AnimationClip("mot_move_run"),				// WALK
			AnimationClip("mot_move_jump_f_start", false),	// JUMP
			AnimationClip("mot_move_jump_f_downloop"),	// FALL
			AnimationClip("mot_attack_nomal", false, 2.0f),		// ATTACK
			AnimationClip("mot_move_land", false),	// LANDING
			AnimationClip("mot_attack_charge_step", false),	// DASHING
			AnimationClip("mot_move_jump_f_uploop", false),	// ROLLING
		});

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

	CharaBase::STATUS oldStatus = _status;

	float deltaTime = 1.0f / 60.0f; // デルタタイム
	UpdateComponent(deltaTime);

	return true;
}

bool Player2::Render()
{
	base::Render();

	AnimationRender(_handle, _pos, _dir);

	return true;
}