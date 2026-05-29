#include "pch.h"
#include "jumpcomponet.h"

namespace gv = gravity;

void JumpComponent::Update(float deltaTime)
{
	Jump();
}

void JumpComponent::Jump()
{
	// ジャンプ要求があって、着地していて、攻撃状態でないならジャンプする
	if(_owner.ConsumeJumpRequest() && _owner.GetLand() && _owner._status != STA::ATTACK)
	{
		_owner.SetJumpCount(false); // ジャンプ回数制限用フラグをfalseにする
		_owner.SetGravity(_owner.GetJumpHeight());

		Vec4 pos = _owner.GetPos();
		pos.y += _owner.GetGravity();
		_owner.SetPos(pos);

		_owner._status = STA::JUMP; // ジャンプにする
		_owner.SetLand(false); // 空中にいるので着地フラグをfalseにする
	}
}

void JumpComponent::Fall()
{
	if(!_owner.GetLand())
	{
		_owner.SetGravity(_owner.GetGravity() - gv::GRAVITY); // 重力を加算する

		Vec4 pos = _owner.GetPos();
		pos.y += _owner.GetGravity(); // 現在の重力分だけ位置を下げる
		_owner.SetPos(pos);

		if(_owner.GetGravity() < 0.0f)
		{
			_owner._status = STA::FALL; // 落下にする
		}
	}
}
