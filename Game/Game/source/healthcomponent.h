#pragma once
#include "charabase.h"

class HealthComponent : public Component<CharaBase>
{
public:

	HealthComponent() = default;
	virtual ~HealthComponent() = default;
	auto GetHP() const { return _hp; }
	auto GetMaxHP() const { return _maxHp; }
	auto IsDead() const { return _isDead; }
	void ApplyDamage(int damage);

private:
	int _hp{ 100 }; // 現在のHP
	int _maxHp{ 100 }; // 最大HP
	bool _isDead{ false }; // 死亡フラグ
};

