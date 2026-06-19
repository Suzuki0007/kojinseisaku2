#include "pch.h"
#include "player.h"
#include "resourcpath.h"

// プレイヤーの移動
bool Player::PlayerMove(Vec4 v)
{
	_pos.x += v.x;
	_pos.z += v.z;
	return true;
}

// 初期化
bool Player::Initialize()
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
	_pos = v::VGet(0.0f, 0.0f, 0.0f); // 初期位置が同じだが、押し出され処理のおかげで位置がずれる
	_dir = v::VGet(0.0f, 0.0f, -1.0f);// キャラモデルはデフォルトで-Z方向を向いている
	// 腰位置の設定
	_col_sub_y = 40.0f;
	// コリジョン半径の設定
	_collision_r = 30.0f;
	_collision_weight = 20.0f;
	_cam = nullptr;
	_mv_speed = 6.0f;
	// ジャンプ、重力関連初期化
	_land = true;
	_jumpHeight = 20;
	_gravity = 0.0f;
	// ダッシュ関連初期化
	_is_dashing = false;
	_dash_speed = 20.0f;
	_dash_time = 12.0f;
	_dash_timer = 0.0f;
	_dash_direction = v::VGet(0.0f, 0.0f, 0.0f);

	// ドッジロール関連初期化
	_is_rolling = false;						// ドッジロール中かどうか
	_roll_speed = 50.0f;						// ドッジロール速度
	_roll_time = 14.0f;						// ドッジロール継続時間
	_roll_timer = 0.0f;						// ドッジロール残り時間
	_roll_direction = v::VGet(0.0f, 0.0f, 0.0f);	// ドッジロール方向

	_jumpCount = true;
	_air_control = 1.0f;

	_air_attack_used = false;	// 空中攻撃フラグ初期化

	return true;
}

// 終了
bool Player::Terminate()
{
	base::Terminate();
	return true;
}

// 攻撃アクション
bool Player::Attack()
{
	AttackCapsule
	(
		v::VGet(0.0f, 0.0f, 0.0f),		// カプセルの下位置(剣の根元)
		v::VGet(0.0f, 0.0f, 0.0f),		// カプセルの上位置(剣の先端)
		80.0f,						// 半径
		10,							// 発生までの時間
		10,							// 有効時間
		0,							// カプセルの伸縮速度
		true,						// カプセルがキャラに追従するか
		10.0f,						// ダメージ量
		10,							// ノックバックフレーム数
		_dir						// 攻撃方向
	);

	return true;
}

// 攻撃用カプセル当たり判定登録
bool Player::AttackCapsule(Vec4 underpos, Vec4 overpos, float r, int waittime, int activetime, int timespeed, bool follow, float damage, int framenum, Vec4 dir)
{
	// 攻撃用カプセル当たり判定情報を追加
	mymath::ATTACKCOLLISION attackcollision;
	attackcollision.capsule.underpos = underpos;
	attackcollision.capsule.overpos = overpos;
	attackcollision.capsule.r = r;
	if(follow)
	{
		attackcollision.capsule.modelhandle = _handle;
		attackcollision.capsule.framenum = MV1SearchFrame(_handle, "Character1_RightHandMiddle1"); // 右手中指先端に追従
	}
	attackcollision.waittime = waittime;		// 発生までの時間
	attackcollision.activetime = activetime;	// 有効時間
	attackcollision.damage = damage;			// ダメージ量
	attackcollision.follow = follow;			// 追従するかどうか
	attackcollision.attackChara = "Player";		// 攻撃をしたキャラクターの名前
	attackcollision.isHit = false;				// 当たっていない状態で追加

	_attack_collision.push_back(attackcollision);
	return true;
}

// 計算処理
bool Player::Process()
{
	base::Process();
	int key = ApplicationBase::GetInstance()->GetKey();
	int trg = ApplicationBase::GetInstance()->GetTrg();

	InputDevice& input = InputLocator::Get();
	input.Update();

	// 処理前の位置を保存
	_oldPos = _pos;

	if(_land)
	{
		_gravity = 0.0f;
	}

	// 処理前のステータスを保存しておく
	CharaBase::STATUS old_status = _status;
	Vec4 v = v::VGet(0.0f, 0.0f, 0.0f);
	float length = 0.0f;

	float sx = _cam->_v_pos.x - _cam->_v_target.x;
	float sz = _cam->_v_pos.z - _cam->_v_target.z;
	float camrad = atan2(sz, sx);
	float rad = 0.0f;

	if((key & (PAD_INPUT_7 | PAD_INPUT_8)) == 0)
	{
		InputDevice& input = InputLocator::Get();

		if(input.IsPress(InputButton::StickDown))
		{
			v.x = 1;
		}
		if(input.IsPress(InputButton::StickUp))
		{
			v.x = -1;
		}
		if(input.IsPress(InputButton::StickLeft))
		{
			v.z = -1;
		}
		if(input.IsPress(InputButton::StickRight))
		{
			v.z = 1;
		}

		_input_v = v;

		if(v::VSize(v) > 0.0f)
		{
			length = _mv_speed;
		}
		rad = atan2(v.z, v.x);
		v.x = cos(rad + camrad) * length;
		v.z = sin(rad + camrad) * length;
	}

	// 移動処理
	if(trg & PAD_INPUT_1)
	{
		if(_land == true)
		{
			// ジャンプ
			_jumpCount = false;
			_gravity = _jumpHeight;
			_pos.y += _gravity;
			_status = STATUS::JUMP;
			_land = false;
		}
		else
		{
			// 空中かつダッシュしていない場合、ダッシュ開始
			if(!_is_dashing)
			{
				_is_dashing = true;
				_dash_timer = _dash_time;
				_status = STATUS::DASHING;
				// ダッシュ方向は現在の入力方向
				if(v::VSize(v) > 0.0f)
				{
					_dash_direction = v;
					_dash_direction.y = 0.0f;

					if(v::VSize(_dash_direction) > 0.0f)
					{
						_dash_direction = v::VNorm(_dash_direction);
					}
				}
				else
				{
					_dash_direction = _dir;
					_dash_direction.y = 0.0f;

					if(v::VSize(_dash_direction) > 0.0f)
					{
						_dash_direction = v::VNorm(_dash_direction);
					}
					else
					{
						_dash_direction = v::VGet(0.0f, 0.0f, -1.0f);
					}
				}
			}

		}
	}

	// ドッジロール
	if(_land == true && (trg & PAD_INPUT_3) && !_is_rolling && !_is_dashing)
	{
		_is_rolling = true;
		_roll_timer = _roll_time;
		_status = STATUS::ROLLING;
		// ドッジロール方向は現在の入力方向
		Vec4 dir = v::VGet(0.0f, 0.0f, 0.0f);
		if(v::VSize(v) > 0.0f)
		{
			dir = v;
		}
		else
		{
			dir = _dir;
			dir.y = 0.0f;
			if(v::VSize(dir) <= 0.0f)
			{
				dir = v::VGet(0.0f, 0.0f, -1.0f);
			}
		}

		// 正規化
		if(v::VSize(dir) > 0.0f)
		{
			dir = v::VNorm(dir);
		}

		_roll_direction = dir;
	}

	// 攻撃入力（PAD_INPUT_2）
	if(trg & PAD_INPUT_2)
	{
		// 地上にいる場合、または空中で未使用の場合のみ攻撃可能
		if(_status != STATUS::ATTACK)
		{
			// 地上にいる場合は常に攻撃可能
			if(_land)
			{
				_status = STATUS::ATTACK;
				Attack();
			}
			// 空中にいて、まだ空中攻撃を使っていない場合のみ攻撃可能
			else if(!_air_attack_used)
			{
				_status = STATUS::ATTACK;
				Attack();
				_air_attack_used = true; // 空中攻撃を使用した
			}
		}
	}

	// 攻撃中は移動・ジャンプ処理をしない
	if(_status == STATUS::ATTACK)
	{
		// 攻撃時の前進（省略せず既存処理）
		if(_play_time < _total_time * 0.5f)
		{
			float attack_move_speed;
			if(_land)
			{
				attack_move_speed = 5.0f;
			}
			else
			{
				attack_move_speed = 8.0f;
			}
			Vec4 forward_dir = v::VNorm(_dir);
			_pos.x += forward_dir.x * attack_move_speed;
			_pos.z += forward_dir.z * attack_move_speed;
		}

		// 空中で攻撃している場合は重力を適用
		if(!_land)
		{
			_gravity -= 0.5f;
			_pos.y += _gravity;

			if(_pos.y <= 0.0f)
			{
				_pos.y = 0.0f;
				_land = true;
				_gravity = 0.0f;
				_jumpCount = true;
				_air_attack_used = false;
			}
		}
	}
	else if(_land == false)
	{
		// 重力処理
		_gravity -= 0.98f;
		_pos.y += _gravity;
		if(_gravity < 0.0f)
		{
			_status = STATUS::FALL;
		}
		if(_pos.y <= 0.0f)
		{
			_pos.y = 0.0f;
			_land = true;
			_gravity = 0.0f;
			_jumpCount = true;
			_is_dashing = false;
			_dash_timer = 0.0f;
			_air_attack_used = false;
		}

		// ダッシュ処理
		if(_is_dashing)
		{
			if(_dash_timer > 0.0f)
			{
				_pos.x += _dash_direction.x * _dash_speed;
				_pos.z += _dash_direction.z * _dash_speed;
				_dash_timer -= 1.0f;
			}
			else
			{
				_is_dashing = false;
				_status = STATUS::FALL;
			}
		}
		else
		{
			// 空中通常移動（v はワールド移動量）
			if(v::VSize(v) > 0.0f)
			{
				_pos.x += v.x * _air_control;
				_pos.z += v.z * _air_control;
				// 軸ロック中は向きを固定
                _dir = v;
			}
		}
	}
	else
	{
		if(_is_rolling)
		{
			if(_roll_timer > 0.0f)
			{
				float ratio = _roll_timer / _roll_time;
				float speed = _roll_speed * (ratio * ratio);
				_pos.x += _roll_direction.x * speed;
				_pos.z += _roll_direction.z * speed;
				_dir = _roll_direction;
				_roll_timer -= 1.0f;
				_status = STATUS::ROLLING;
			}
			else
			{
				_is_rolling = false;
				_status = STATUS::WAIT;
			}
		}
		else
		{
			// 地上移動（v はワールド移動量）
			if(v::VSize(v) > 0.0f && _status != STATUS::ATTACK)
			{
                _dir = v;

				_status = STATUS::WALK;
				PlayerMove(v);
			}
			else
			{
				_status = STATUS::WAIT;
				if(old_status == STATUS::FALL || old_status == STATUS::LANDING)
				{
					_status = STATUS::LANDING;
				}
			}
		}

	}

	// アニメーション時間・アタッチ管理（既存処理そのまま）
	if(old_status == _status)
	{
		float anim_speed = 0.5f;
		if(_status == STATUS::ATTACK)
		{
			anim_speed = 2.0f;
		}
		_play_time += anim_speed;
		switch(_status)
		{
		case STATUS::WAIT:
		{
			_play_time += (float)(rand() % 10) / 100.0f;
			break;
		}
		}
	}
	else
	{
		if(_attach_index != -1)
		{
			MV1DetachAnim(_handle, _attach_index);
			_attach_index = -1;
		}
		switch(_status)
		{
		case STATUS::WAIT:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_attack_charge_loop"), -1, FALSE);
			break;
		case STATUS::WALK:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_move_run"), -1, FALSE);
			break;
		case STATUS::JUMP:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_move_jump_f_start"), -1, FALSE);
			break;
		case STATUS::FALL:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_move_jump_f_downloop"), -1, FALSE);
			break;
		case STATUS::LANDING:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_move_land"), -1, FALSE);
			break;
		case STATUS::DASHING:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_attack_charge_step"), -1, FALSE);
			break;
		case STATUS::ATTACK:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_attack_nomal"), -1, FALSE);
			break;
		case STATUS::ROLLING:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_move_jump_f_uploop"), -1, FALSE);
			break;
		}

        if(_attach_index != -1)
        {
            _total_time = MV1GetAttachAnimTotalTime(_handle, _attach_index);
        }
        else
        {
            _total_time = 1.0f;
        }

		_play_time = 0.0f;

		switch(_status)
		{
		case STATUS::WAIT:
			_play_time += rand() % 30;
			break;
		}
	}
	if(_play_time >= _total_time)
	{
		if(_status == STATUS::LANDING)
		{
			_status = STATUS::WAIT;
			if(_attach_index != -1)
			{
				MV1DetachAnim(_handle, _attach_index);
				_attach_index = -1;
			}
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_attack_charge_loop"), -1, FALSE);
			_total_time = MV1GetAttachAnimTotalTime(_handle, _attach_index);
		}
		else if(_status == STATUS::ATTACK)
		{
			if(_land)
			{
				_status = STATUS::WAIT;
				if(_attach_index != -1)
				{
					MV1DetachAnim(_handle, _attach_index);
					_attach_index = -1;
				}
				_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_attack_charge_loop"), -1, FALSE);
				_total_time = MV1GetAttachAnimTotalTime(_handle, _attach_index);
			}
			else
			{
				_status = STATUS::FALL;
				if(_attach_index != -1)
				{
					MV1DetachAnim(_handle, _attach_index);
					_attach_index = -1;
				}
				_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "mot_move_jump_f_downloop"), -1, FALSE);
				_total_time = MV1GetAttachAnimTotalTime(_handle, _attach_index);
			}
		}
		_play_time = 0.0f;
	}
	return true;
}

// 描画処理
bool Player::Render()
{
    base::Render();
    // 再生時間をセットする
    if(_attach_index != -1)
    {
        MV1SetAttachAnimTime(_handle, _attach_index, _play_time);
    }

    // 位置
    VC::MV1SetPosition(_handle, _pos);
    // 向きからY軸回転を算出
    Vec4 vrot = { 0.0f, 0.0f, 0.0f };
    vrot.y = atan2(_dir.x * -1, _dir.z * -1);// モデルが標準でどちらを向いているかで式が変わる(これは-zを向いている場合)
    VC::MV1SetRotationXYZ(_handle, vrot);
    // 描画
    MV1DrawModel(_handle);

    return true;

}

