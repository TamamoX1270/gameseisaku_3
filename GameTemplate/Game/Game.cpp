#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Wall.h"
#include "Wallbreak.h"
#include "Enemy.h"
#include "BackGround.h"
#include "GameCamera.h"



bool Game::Start()
{
	m_wall = NewGO<Wall>(0, "wall");
	//m_wallbreak = NewGO<Wallbreak>(0, "wallbreak");
	m_player = NewGO<Player>(0, "player");
	m_backGround = NewGO<BackGround>(0, "backGround");
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");

	Enemy* enemy = NewGO<Enemy>(0, "enemy");

	return true;
}
Game::~Game()
{
	DeleteGO(m_backGround);
	DeleteGO(m_wall);
}


void Game::Update()
{
	Timer();
	WorldChanege();
}

void Game::Timer()
{
	//白の世界の時かつ白のチェンジステートがfalseなら白タイマーを取得する。
	if (worldstate == 0 && whiteChangestate == false)
	{
		whitetimer += g_gameTime->GetFrameDeltaTime();
	}
	//白の世界のタイマーが2.0fを越えていたら
	if (whitetimer >= 2.0f && whiteChangestate == false)
	{
		//白のチェンジステートをtrueにする。
		whiteChangestate = true;
	}
	//黒の世界の時かつ黒のチェンジステートがfalseなら黒タイマーを取得する。
	if (worldstate == 1 && blackChangestate == false)
	{
		blacktimer += g_gameTime->GetFrameDeltaTime();
	}
	//黒の世界のタイマーが2.0fを越えていたら
	if (blacktimer >= 2.0f && blackChangestate == false)
	{
		blackChangestate = true;
	}
}
void Game::WorldChanege()
{
	//白の世界の時かつ白のチェンジステートがtrueかつLB2を押されたら
	if (g_pad[0]->IsTrigger(enButtonLB2) && worldstate == 0 && whiteChangestate == true)
	{
		//黒の世界にして
		worldstate = 1;
		//黒タイマーを0.0fにする。
		blacktimer = 0.0f;
		//黒のチェンジステートをfalseにする。
		blackChangestate = false;
	}
	//黒の世界の時かつ黒のチェンジステートがtrueかつLB2を押されたら
	if (g_pad[0]->IsTrigger(enButtonLB1) && worldstate == 1 && blackChangestate == true)
	{
		//白の世界にして
		worldstate = 0;
		//白タイマーを0.0fにする。
		whitetimer = 0.0f;
		//白のチェンジステートをfalseにする。
		whiteChangestate = false;
	}
}

void Game::Render(RenderContext& rc)
{
}