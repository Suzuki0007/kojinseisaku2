#pragma once
#include "playerbase.h"

class Player3 : public PlayerBase
{
	typedef PlayerBase base;
public:
	Player3() = default;
	virtual ~Player3() = default;

	bool Initialize() override;
	bool Terminate() override;
	bool Process() override;
	bool Render() override;

	void CommandAttack(IBattleReceiver* target) override;
	bool IsExceutionAction() const override;
};

