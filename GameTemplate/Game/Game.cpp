#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Wall.h"
#include "Enemy.h"
#include "BackGround.h"
#include "GameCamera.h"



bool Game::Start()
{
	m_wall = NewGO<Wall>(0, "wall");
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
}

void Game::Render(RenderContext& rc)
{
}