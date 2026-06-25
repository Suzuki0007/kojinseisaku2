#pragma once
#include "playerbase.h"
class Player2 : public PlayerBase
{
	typedef PlayerBase base;
public:
	Player2() = default;
	virtual ~Player2() = default;

	bool Initialize() override;
	bool Terminate() override;
	bool Process() override;
	bool Render() override;

	void CommandAttack(IBattleReceiver* target) override;
	bool IsExceutionAction() const override;
};

