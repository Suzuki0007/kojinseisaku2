#pragma once
#include "objectbase.h"
#include "camera.h"
#include "battlereceiver.h"
#include "animationcomponent.h"
#include "pch.h"

class CharaBase : public ObjectBase, public IComponentBindable<CharaBase>, public IBattleReceiver
{
	typedef ObjectBase base;
public:
	enum class STATUS
	{
		NONE,
		WAIT,
		WALK,
		JUMP,
		FALL,
		ATTACK,
		LANDING,
		DASHING,
		ROLLING,
		COUNT
	};
	STATUS _status;

	struct AnimConfig
	{
		std::string_view idle;
		std::string_view walk;
		std::string_view attack;
	};

	virtual ~CharaBase();

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	const Vec4& GetPos() const { return _pos; }
	const Vec4& GetOldPos() const { return _oldPos; }
	const Vec4& SetPos(const Vec4& pos) { return _pos = pos; }
	const Vec4& SetDir(const Vec4& dir) { return _dir = dir; }
	auto GetColSubY() const { return _col_sub_y; }
	auto GetCollisionR() const { return _collision_r; }
	auto GetCollisionWeight() const { return _collision_weight; }
	auto GetStatus() const { return _status; }

	bool Damage(float damage);

	float GetHP() const { return _hp; }	// HP取得用ゲッター
	bool IsAlive() const { return _is_alive; } // 生存確認用ゲッター

	auto GetLand() const { return _land; }
	void SetLand(bool land) { _land = land; }

	float GetGravity() const { return _gravity; }
	void SetGravity(float g) { _gravity = g; }

	float GetJumpHeight() const { return _jumpHeight; }
	void SetJumpHeight(float h) { _jumpHeight = h; }

	void SetJumpCount(bool v) { _jumpCount = v; }
	bool GetJumpCount() const { return _jumpCount; }

	// ジャンプするかどうか
	void RequestJump(bool v) { _jumpRequest = true; }
	// ジャンプ要求があればtrueを返し、要求を消費する
	bool ConsumeJumpRequest()
	{
		if(!_jumpRequest) {return false;}
		_jumpRequest = false;
		return true;
	}

	void AnimationRender(int handle, const Vec4& pos, const Vec4& dir);

	void SetOnAnimEndCallback(std::move_only_function<void()>&& callback)
	{
		_onAnimEnd = std::move(callback);
	}
	void ExecuteOnAnimEnd()
	{
		if(_onAnimEnd)
		{
			_onAnimEnd();
			_onAnimEnd = nullptr;
		}
	}
	AnimConfig& GetAnimConfig() { return _animConfig; }

protected:
	int _attach_index;
	float _total_time;
	float _play_time;
	Vec4 _pos; // キャラの位置
	Vec4 _oldPos; // 1フレーム前のキャラの位置
	Vec4 _dir; // キャラの向き
	float _col_sub_y; // コリジョン判定用のYオフセット
	float _collision_r; // 円の半径
	float _collision_weight; // キャラの重さ

	float _hp;// キャラの体力
	bool _is_alive;// 生存フラグ

	bool _land { false };// 着地しているかどうか
	bool _jumpRequest { false }; // ジャンプ要求フラグ
	float _jumpHeight { 0.0f }; // ジャンプの高さ
	float _gravity { 0.0f }; // 重力の強さ
	bool _jumpCount { true };			// ジャンプ回数制限用フラグ
	AnimConfig _animConfig; // アニメーション名の設定

	std::move_only_function<void()> _onAnimEnd{ nullptr }; // アニメーション終了時のコールバック関数

	AnimationComponent* _anim;
	ComponentOwner<CharaBase> _comOwner; // コンポーネント
};

