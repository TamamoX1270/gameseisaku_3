#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"



bool Game::Start()
{
	m_player = NewGO<Player>(0, "player");
	m_backGround = NewGO<BackGround>(0, "backGround");
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");

	return true;
}
Game::~Game()
{
	DeleteGO(m_backGround);
}

void Game::Update()
{
}

void Game::Render(RenderContext& rc)
{
}