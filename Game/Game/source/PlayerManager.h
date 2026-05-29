#pragma once
#include "appframe.h"
#include "playerbase.h"
#include "playerfactory.h"
#include "objectmanagerbase.h"
#include <string>

class PlayerManager : public Singleton<PlayerManager>, public ObjectManagerBase<PlayerBase, PlayerFactory>
{
	friend class Singleton<PlayerManager>;
public:
	using OMB = ObjectManagerBase<PlayerBase, PlayerFactory>;

	void RegisterType();
	void CreatePlayer(const std::string& type);
	std::unique_ptr<PlayerBase>& GetPlayer() { return GetOwner(); }

private:
	PlayerManager() = default;
	virtual ~PlayerManager() = default;
};

