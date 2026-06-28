#pragma once
#include "pch.h"

// アニメーションの情報を保持するクラス
class AnimationClip
{
public:
	AnimationClip() = default;
	AnimationClip(
		const std::string_view& name,
		bool loop = true, 
		float speed = 1.0f
	)
		: _name(name), _loop(loop), _speed(speed)
	{
	}

	const std::string_view& GetName() const { return _name; }
	bool IsLoop() const { return _loop; }
	float GetSpeed() const { return _speed; }

private:
	std::string_view _name;
	bool _loop{ true };
	float _speed{ 1.0f };
};

