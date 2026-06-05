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
	DrawFormatString(100, 100, GetColor(0, 0, 0), "コマンドバトル開始！");
	DrawFormatString(100, 200, GetColor(0, 0, 0), "敵（No.%d）に遭遇した！", _targetEnemyId);
	DrawFormatString(100, 300, GetColor(0, 0, 0), "キーを押してフィールドに戻る");
}
