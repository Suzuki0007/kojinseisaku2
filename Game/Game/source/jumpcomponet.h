#pragma once
#include "pch.h"
#include "charabase.h"

namespace gravity
{
	static constexpr float GRAVITY = 0.98f; // 重力の強さ
}

class JumpComponent final : public Component<CharaBase>
{
public:
	using Component::Component;
	using STA = CharaBase::STATUS;

	void Jump();
	void Fall();

	void Update(float deltaTime) override;
};

