/*
** toluaでluaに追加するコマンド群
*/

#include "pch.h"
#include	"lua.hpp"
#include	"lua_cmd.h"

#include "ApplicationMain.h"
#include "ModeGame.h"

//void _LOG(const char* msg) {
//	// luaから来る文字列はUTF-8, Win32APIはS-JISなので変換
//	std::string sSJIS = ConvUTF8toSJIS(msg);
//	OutputDebugString(sSJIS.c_str());		// Win32API, デバッグコンソールにメッセージを出す
//}

// ModeGameにアクセスするため
static ModeGame* MODEGAME() 
{
	return (ModeGame*)(ModeServer::GetInstance()->Get("game"));
}

// パッド情報を得る
int	GetPad() {
	int key = ApplicationMain::GetInstance()->GetKey();
	return key;
}