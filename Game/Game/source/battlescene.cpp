#include "pch.h"
#include "battlescene.h"
#include "playerbase.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

IBattleReceiver* BattleScene::GetPlayer() 
{
	auto& player = PlayerManager::GetInstance()->GetPlayer();
	if(player)
	{
		return player.get();
	}
	else
	{
		return nullptr;
	}
}

IBattleReceiver* BattleScene::GetEnemy() const
{
	auto& enemy = EnemyManager::GetInstance()->GetEnemy(_targetEnemyId);
	if(enemy)
	{
		return enemy.get();
	}
	return nullptr;
}

void BattleScene::Initialize()
{

}

void BattleScene::Update()
{
	InputDevice& input = InputLocator::Get();
	input.Update();
	if(input.IsTrigger(InputButton::SceneDebug))
	{
		if (_observer)
		{
			_observer->OnChangeState(GameState::World, _targetEnemyId);
		}
	}
}

void BattleScene::Render()
{
	// 敵のIDを表示する
	DrawFormatString(100, 100, GetColor(0, 0, 0), "コマンドバトル開始！");
	DrawFormatString(100, 200, GetColor(0, 0, 0), "敵（No.%d）に遭遇した！", _targetEnemyId);
	DrawFormatString(100, 300, GetColor(0, 0, 0), "キーを押してフィールドに戻る");
}
