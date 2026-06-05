#pragma once
#include "charabase.h"

class HealthComponent : public Component<CharaBase>
{
public:
	explicit HealthComponent(CharaBase& owner) : Component(owner) {}
	HealthComponent() = default;
	virtual ~HealthComponent() = default;
	auto GetHP() const { return _hp; }
	auto GetMaxHP() const { return _maxHp; }
	auto IsDead() const { return _isDead; }
	void ApplyDamage(int damage);

	void ForceDeath() { _hp = 0; _isDead = true; }

private:
	int _hp{ 100 }; // 現在のHP
	int _maxHp{ 100 }; // 最大HP
	bool _isDead{ false }; // 死亡フラグ
};

