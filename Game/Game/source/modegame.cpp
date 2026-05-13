#include "modegame.h"
#include "applicationmain.h"
#include "modeeffekseer.h"
#include "modetitle.h"
#include <algorithm>

// 初期化
bool ModeGame::Initialize()
{
	if(!base::Initialize())
	{
		return false;
	}

	// カメラ初期化
	_camera = new Camera();
	_camera->Initialize();

	ObjectInitialize();	// オブジェクト初期化

	// キャラ
	for(auto& chara : _chara)
	{
		chara->Initialize();
	}

	// オブジェクトの初期化
	for(auto& object : _object)
	{
		object->Initialize();
	}

	_map->SetCamera(_camera);
	_player->SetCamera(_camera);

	std::vector<VECTOR> cube_positions =
	{
		VGet(-1204.0f, 0.0f, 1700.0f),
		VGet(-1500.0f, 250.0f, -1600.0f),
		VGet(-950.0f, 0.0f, -1600.0f),
		VGet(-1200.0f, 0.0f, 1700.0f),
		VGet(1070.0f, 0.0f, -35.0f),
		VGet(6000.0f, 0.0f, 1500.0f)
	};

	for(size_t i = 0; i < _cube.size(); i++)
	{
		_cube[i]->SetPosition(cube_positions[i]);
		_cube[i]->UpdateAABB();
	}

	std::vector<VECTOR> enemy_positions =
	{
		VGet(-1350.0f, 400.0f, -1600.0f),
		VGet(650.0f,0.0f, 1500.0f),
		VGet(1100.0f, 0.0f, 150.0f),
		VGet(-1200.0f,100.0f, 1700.0f),
		VGet(150.0f, 0.0f, -3700.0f),
		VGet(6000.0f, 100.0f, 1500.0f)
	};

	for(size_t i = 0; i < _enemy.size() && i < enemy_positions.size(); ++i)
	{
		_enemy[i]->SetPos(enemy_positions[i]);
	}

	DebugInitialize();// デバック初期化

	_resolve_on_y = false;
	_landed_on_up = false;

	_time_limit = 60; // 制限時間1分
	_is_gameover = false;
	// 終了時の残り時間を未設定にする
	_final_remaining_time = -1;
	// _enemy_count を初期の総数として保持（倒した数は動的に計算する）
	_enemy_count = static_cast<int>(_enemy.size());

	_enemy_alive_list.clear();
	_enemy_alive_list.resize(_enemy_count, false);
	for(size_t i = 0; i < _enemy.size(); ++i)
	{
		_enemy_alive_list[i] = _enemy[i]->IsAlive();
	}

	return true;
}

// 終了
bool ModeGame::Terminate()
{
	base::Terminate();
	// キャラ
	for(auto& chara : _chara)
	{
		chara->Terminate();
	}
	_chara.clear();
	for(auto& object : _object)
	{
		object->Terminate();
	}
	_object.clear();
	delete _camera;
	return true;
}

// 円同士の当たり判定
bool ModeGame::IsHitCircle(CharaBase* c1, CharaBase* c2)
{
	// 「2つの円の中心点」の距離を求める
	// ピタゴラスの定理（三平方の定理）で求めることができる
	// x,zで処理。yは判定せず
	float w, h, length;
	w = c1->GetPos().x - c2->GetPos().x;
	h = c1->GetPos().z - c2->GetPos().z;
	length = static_cast<float>(sqrtf(w * w + h * h));

	// 中心点間の距離が、2つの円の半径の合計よりも小さい場合、当たり
	if(length < c1->GetCollisionR() + c2->GetCollisionR())
	{
		return true;
	}

	return false;
}

// プレイヤーのカメラ情報表示
bool ModeGame::PlayerCameraInfo()
{
	// カメラの位置/視点の移動を、プレイヤーの移動量に追従する
	VECTOR playermove = VSub(_player->GetPos(), _player->GetOldPos());
	// 水平移動は従来通り位置を移す
	_camera->_v_pos = VAdd(_camera->_v_pos, VGet(playermove.x, 0.0f, playermove.z));
	// 注視点はプレイヤーの現在位置を基準に高さも含めて追従させる
	VECTOR player_target = _player->GetPos();
	player_target.y += 60.0f; // プレイヤーの目線高さなどのオフセット
	_camera->SetTargetPosition(player_target);
	return true;
}

// 計算処理
bool ModeGame::Process()
{
	base::Process();

	DebugProcess();// デバック処理

	// ゲームオーバー時の入力処理（タイトルへ戻る）
	int trg = ApplicationMain::GetInstance()->GetTrg();
	if(_is_gameover)
	{
		// ゲームオーバー中は下のレイヤー処理を止める
		ModeServer::GetInstance()->SkipRenderUnderLayer();

		// 決定ボタンでタイトルを呼び出す
		if(trg & PAD_INPUT_1)
		{
			// タイトルモードを追加して、このモードを削除
			ModeServer::GetInstance()->Add(new ModeTitle(), 0, "title");
			ModeServer::GetInstance()->Del(this);
		}

		// ゲームオーバー中はゲーム処理を行わない
		return true;
	}

	// キャラ処理（生存しているもののみ）
	for(auto& chara : _chara)
	{
		if(chara->IsAlive())
		{
			chara->Process();
		}
	}

	// オブジェクト処理
	for(auto& object : _object)
	{
		object->Process();
	}

	// 敵との当たり判定処理（生存している敵のみ） & 死亡カウント検出
	for(size_t i = 0; i < _enemy.size(); i++)
	{
		auto& enemy = _enemy[i];
		if(enemy->IsAlive())
		{
			CharaToCharaCollision(_player.get(), enemy.get());
			// 敵とキューブの当たり判定
			for(auto& cube : _cube)
			{
				CharaToCubeCollision(enemy.get(), cube.get());
			}
		}

		// 死亡検出（前フレームは生存で今フレーム死んでいる => 倒した）
		bool prev_alive = false;
		if(i < _enemy_alive_list.size())
		{
			prev_alive = _enemy_alive_list[i];
		}
		bool now_alive = enemy->IsAlive();
		// 状態を保存
		if(i < _enemy_alive_list.size())
		{
			_enemy_alive_list[i] = now_alive;
		}
	}

	// 当たり判定の処理をここに書く
	EscapeCollision();

	_resolve_on_y = false;
	_landed_on_up = false;
	// プレイヤーとキューブの当たり判定
	for(auto& cube : _cube)
	{
		CharaToCubeCollision(_player.get(), cube.get());
	}

	LandCheck();

	// 攻撃判定の更新処理
	UpdateCheckAttackCollision();


	// プレイヤーのカメラ情報表示
	PlayerCameraInfo();

	// カメラ操作はプレイヤー処理の後に行い、ジャンプなどの高さ変化を追従させる
	_camera->Process();

	// 制限時間チェック（GetModeTm() はミリ秒）
	unsigned long elapsed_ms = GetModeTm();
	int elapsed_sec = static_cast<int>(elapsed_ms / 1000);
	int remaining = _time_limit - elapsed_sec;
	if(remaining <= 0)
	{
		_is_gameover = true;
		// ゲームオーバーになった瞬間、下のレイヤー処理を止める（選択的）
		ModeServer::GetInstance()->SkipProcessUnderLayer();
		// 残り時間を0で確定
		if(_final_remaining_time < 0)
		{
			_final_remaining_time = 0;
		}
	}

	// 敵が全て倒されたかチェック（残り敵数が0）
	int alive_count = 0;
	for(auto& enemy : _enemy)
	{
		if(enemy->IsAlive()) ++alive_count;
	}
	if(alive_count == 0)
	{
		_is_gameover = true;
		// 全滅でゲームオーバーになった瞬間、下のレイヤー処理を止める
		ModeServer::GetInstance()->SkipProcessUnderLayer();
		// 全滅時の残り時間を確定（負になる場合は0で）
		int final_rem = remaining;
		if(final_rem < 0) final_rem = 0;
		if(_final_remaining_time < 0)
		{
			_final_remaining_time = final_rem;
		}
	}

	return true;
}

// 描画処理
bool ModeGame::Render()
{
	base::Render();


	// カメラ設定更新
	SetCameraPositionAndTarget_UpVecY(_camera->_v_pos, _camera->_v_target);
	SetCameraNearFar(_camera->_clip_near, _camera->_clip_far);

	// キャラを描画（生存しているもののみ）
	for(auto& chara : _chara)
	{
		if(chara->IsAlive())
		{
			chara->Render();
		}
	}

	// オブジェクトを描画
	for(auto& object : _object)
	{
		object->Render();
	}

	DebugRender();// デバック描画処理

    // 敵のHP情報を画面に表示（生存している敵のみ）と生存カウント取得
    // フォントサイズを小さくして表示する
    SetFontSize(16);
    int y_offset = 10; // 画面上部からのオフセット
    int alive_count = 0; // 生存している敵のカウント用
    for(int i = 0; i < _enemy.size(); i++)
    {
        auto& enemy = _enemy[i];
        if(enemy->IsAlive())
        {
            DrawFormatString(10, y_offset + (alive_count * 20), GetColor(255, 0, 0),
                "Enemy[%d] HP: %1f",
                i,
                enemy->GetHP()
                ); // 最大HPが分からないので現在HPを表示
            alive_count++;
        }
    }

	 SetFontSize(64);

    // 残り時間表示
    unsigned long elapsed_ms = GetModeTm();
    int elapsed_sec = static_cast<int>(elapsed_ms / 1000);
    int remaining = _time_limit - elapsed_sec;
	if(remaining < 0) { remaining = 0; }
    // ゲームオーバー時に確定した残り時間があればそちらを表示
    int display_time;
    if(_final_remaining_time >= 0)
    {
        display_time = _final_remaining_time;
    }
    else
    {
        display_time = remaining;
    }

	if(!_is_gameover)
	{
		DrawFormatString(500, 10, GetColor(0, 0, 0), "Time: %d", display_time);
	}
    

	// ゲームオーバー時の表示
	if(_is_gameover)
	{
		// 中央付近に Game Over と倒した数と残り時間を表示
		int cx = 300;
		int cy = 200;
		SetFontSize(64);
		DrawFormatString(cx + 200, cy, GetColor(0, 0, 0), "GAME OVER");
		SetFontSize(32);
		// 倒した敵数は初期総数 - 現在生存数
		int defeated = _enemy_count - alive_count;
		if(defeated < 0)
		{
			defeated = 0;
		}
		DrawFormatString(cx + 200, cy + 100, GetColor(255, 0, 0), "敵を倒した数: %d", defeated);
        // 残り時間も表示（時間切れなら 0）。ゲームオーバー時に確定した残り時間を使う
        int display_final;
        if(_final_remaining_time >= 0)
        {
            display_final = _final_remaining_time;
        }
        else
        {
            display_final = remaining;
        }
        DrawFormatString(cx + 200, cy + 200, GetColor(255, 0, 0), "残り時間: %d", display_final);
		// 操作説明
		DrawFormatString(cx + 50, cy + 300, GetColor(255, 0, 0), "キーボード:Zまだはパッド:Aでタイトルへ");
		// フォントサイズを戻す（他描画に影響しないように）
		SetFontSize(16);
	}

	return true;
}