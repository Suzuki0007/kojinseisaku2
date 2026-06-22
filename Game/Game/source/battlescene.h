#pragma once
#include "scenebase.h"
#include "battleaction.h"
#include "battlereceiver.h"

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
	std::vector<std::unique_ptr<BattleAction>> _actions;
	int _targetEnemyId { -1 };
	int _commandCursor{ 0 };

	IBattleReceiver* GetPlayer();
	IBattleReceiver* GetEnemy() const;
};

