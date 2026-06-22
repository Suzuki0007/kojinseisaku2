#pragma once

class CharaState
{
public:
	enum class ActionState
	{
		None,
		MoveToTarget,
		PlayAnimation,
		ReturnToHome,
	};

	virtual ~CharaState() = default;

	virtual void Enter(Actor& actor) = 0;

};

