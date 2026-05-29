#include "pch.h"
#include "PlayerManager.h"
#include "player.h"

void PlayerManager::RegisterType()
{
	OMB::RegisterType("Player", []() { return std::make_unique<Player>(); });
}

void PlayerManager::CreatePlayer(const std::string& type)
{
	Create(type);
}