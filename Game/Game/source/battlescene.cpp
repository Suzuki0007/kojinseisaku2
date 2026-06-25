#include "pch.h"
#include "battlescene.h"
#include "playerbase.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "attackaction.h"

IBattleReceiver* BattleScene::GetPlayer() 
{
	if(!_battlePlayers.empty())
	{
		return _battlePlayers.front();
	}

	auto& player = PlayerManager::GetInstance()->GetPlayer();
	if(!player.empty())
	{
		return player.front().get();
	}
	return nullptr;
}

IBattleReceiver* BattleScene::GetEnemy() const
{
	if(!_battleEnemies.empty())
	{
		return _battleEnemies.front().get();
	}

	auto& enemy = EnemyManager::GetInstance()->GetEnemies();
	if(!enemy.empty())
	{
		return enemy.front().get();
	}
	return nullptr;
}

void BattleScene::Initialize()
{
	_battleCamera.Initialize();
	_battleCamera._v_pos = v::VGet(0.0f, 210.0f, 620.0f);
	_battleCamera._v_target = v::VGet(0.0f, 70.0f, 0.0f);
	_battleCamera._clip_near = 2.0f;
	_battleCamera._clip_far = 5000.0f;

	_backgroundHandle = LoadGraph("res/Texture/Background.png");

	_mapRender = std::make_unique<MapRenderComponent>();
	_mapRender->Initialize();

	_actions.clear();
	_actions.emplace_back(std::make_unique<AttackAction>());

	CreateBattleMembers();
}

void BattleScene::Update()
{
	InputDevice& input = InputLocator::Get();
	input.Update();

	UpdateBattleMembers();
	if(_mapRender)
	{
		_mapRender->Process();
	}

	if(input.IsTrigger(InputButton::SceneDebug))
	{
		if(_observer)
		{
			_observer->OnChangeState(GameState::World, _targetEnemyId);
		}
		return;
	}

	if(IsActionRunning())
	{
		return;
	}

	// コマンドメニューのカーソル移動
	if(input.IsTrigger(InputButton::CommandUp))
	{
		_commandMenu.MoveUp();
	}
	if(input.IsTrigger(InputButton::CommandDown))
	{
		_commandMenu.MoveDown();
	}

	// 決定
	if(input.IsTrigger(InputButton::Decide))
	{
		OnCommandSelected(_commandMenu.GetSelectedCommand());
	}
}

void BattleScene::OnCommandSelected(BattleCommandMenu::BattleCommand command)
{
	switch(command)
	{
	case BattleCommandMenu::BattleCommand::Fight:
		// たたかうコマンドが選択されたら、既存の攻撃アクション経由で再生する
		if(!_actions.empty())
		{
			_actions.front()->Execute(GetPlayer(), GetEnemy());
		}
		break;
	case BattleCommandMenu::BattleCommand::End:
		if(_observer)
		{
			_observer->OnChangeState(GameState::World, _targetEnemyId);
		}
		break;
	default:
		break;
	}
}

void BattleScene::Render()
{
	ClearDrawScreen();

	SetCameraPositionAndTarget_UpVecY(
		VC::VecToDxLib(_battleCamera._v_pos),
		VC::VecToDxLib(_battleCamera._v_target));
	SetCameraNearFar(_battleCamera._clip_near, _battleCamera._clip_far);

	RenderBattleMembers();

	if(_mapRender)
	{
		_mapRender->Render();
	}

	SetFontSize(32);
	DrawFormatString(80, 60, GetColor(255, 255, 255), "コマンドバトル");
	DrawFormatString(80, 105, GetColor(255, 255, 255), "敵（No.%d）に遭遇した！", _targetEnemyId);

	// コマンド選択メニューを描画
	_commandMenu.Render(80, 540, 42);
	SetFontSize(16);
}

void BattleScene::CreateBattleMembers()
{
	_battlePlayers.clear();
	_battleEnemies.clear();

	const Vec4 playerPositions[] =
	{
		v::VGet(-170.0f, 0.0f, 220.0f),
		v::VGet(0.0f, 0.0f, 260.0f),
		v::VGet(170.0f, 0.0f, 220.0f),
	};
	const Vec4 enemyPositions[] =
	{
		v::VGet(-170.0f, 0.0f, -160.0f),
		v::VGet(0.0f, 0.0f, -210.0f),
		v::VGet(170.0f, 0.0f, -160.0f),
	};

	auto& players = PlayerManager::GetInstance()->GetPlayer();

	for(int i = 0; i < 3; ++i)
	{
		// プレイヤーの配置（万が一マネージャーに3人いなくてもエラーにならないようにif文をつける）
		if(i < players.size())
		{
			auto player = players[i].get();
			player->Initialize();
			player->SetCamera(&_battleCamera);
			player->SetPos(playerPositions[i]);
			player->SetDir(v::VGet(0.0f, 0.0f, -1.0f));
			_battlePlayers.emplace_back(player);
		}

		// 敵の生成と配置
		auto enemy = std::make_unique<Enemy>();
		enemy->Initialize();
		enemy->SetPos(enemyPositions[i]);
		enemy->SetDir(v::VGet(0.0f, 0.0f, 1.0f));
		_battleEnemies.emplace_back(std::move(enemy));
	}
}

void BattleScene::UpdateBattleMembers()
{
	for(auto& player : _battlePlayers)
	{
		player->Process();
	}
	for(auto& enemy : _battleEnemies)
	{
		enemy->Process();
	}
}

void BattleScene::RenderBattleMembers()
{
	for(auto& enemy : _battleEnemies)
	{
		enemy->Render();
	}
	for(auto& player : _battlePlayers)
	{
		player->Render();
	}
}

bool BattleScene::IsActionRunning() const
{
	for(const auto& player : _battlePlayers)
	{
		if(player->IsExceutionAction())
		{
			return true;
		}
	}
	for(const auto& enemy : _battleEnemies)
	{
		if(enemy->IsExceutionAction())
		{
			return true;
		}
	}
	return false;
}
