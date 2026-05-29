#pragma once
#include "appframe.h"
#include "camera.h"
#include <memory>
class ModeBattle : public ModeBase
{
	typedef ModeBase base;

public:
	virtual bool Initialize() override;
	virtual bool Terminate() override;
	virtual bool Process() override;
	virtual bool Render() override;

private:
	int _enemyIndex;// 敵のインデックス
	std::unique_ptr<Camera> _camera;// カメラ
};

