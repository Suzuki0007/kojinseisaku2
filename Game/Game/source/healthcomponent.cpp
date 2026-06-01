#include "pch.h"
#include "healthcomponent.h"

void HealthComponent::ApplyDamage(int damage)
{
	if(_isDead) { return; }

	_hp -= damage;
	if(_hp <= 0)
	{
		_hp = 0;
		_isDead = true;
	}
}
