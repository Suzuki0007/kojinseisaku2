#pragma once
#include "scenebase.h"

class BattleScene : public SceneBase
{
public:
	BattleScene() = default;
	virtual ~BattleScene() = default;
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
	void SetUp(int enemyId) { _targetEnemyId = enemyId; }
	auto GetTargetEnemyId() const { return _targetEnemyId; }
private:
	int _targetEnemyId { -1 };
};

