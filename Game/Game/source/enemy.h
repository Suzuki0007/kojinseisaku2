#pragma once
#include "enemybase.h"
class Enemy : public EnemyBase
{
	typedef EnemyBase base;

public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void CommandAttack(IBattleReceiver* target) override;
	bool IsExceutionAction() const override;

protected:

};

