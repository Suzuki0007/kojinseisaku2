#include "pch.h"
#include "battlescene.h"

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
	DrawFormatString(100, 100, GetColor(255, 255, 255), "コマンドバトル開始！");
	DrawFormatString(100, 140, GetColor(255, 255, 255), "敵（No.%d）に遭遇した！", _targetEnemyId);
	DrawFormatString(100, 180, GetColor(255, 255, 0), "Enterキーを押してフィールドに戻る");
}
