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
	//���̐��E�̎������̃`�F���W�X�e�[�g��false�Ȃ甒�^�C�}�[���擾����B
	if (worldstate == 0 && whiteChangestate == false)
	{
		whitetimer += g_gameTime->GetFrameDeltaTime();
	}
	//���̐��E�̃^�C�}�[��2.0f���z���Ă�����
	if (whitetimer >= 2.0f && whiteChangestate == false)
	{
		//���̃`�F���W�X�e�[�g��true�ɂ���B
		whiteChangestate = true;
	}
	//���̐��E�̎������̃`�F���W�X�e�[�g��false�Ȃ獕�^�C�}�[���擾����B
	if (worldstate == 1 && blackChangestate == false)
	{
		blacktimer += g_gameTime->GetFrameDeltaTime();
	}
	//���̐��E�̃^�C�}�[��2.0f���z���Ă�����
	if (blacktimer >= 2.0f && blackChangestate == false)
	{
		blackChangestate = true;
	}
}
void Game::WorldChanege()
{
	//���̐��E�̎������̃`�F���W�X�e�[�g��true����LB2�������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonLB2) && worldstate == 0 && whiteChangestate == true)
	{
		//���̐��E�ɂ���
		worldstate = 1;
		//���^�C�}�[��0.0f�ɂ���B
		blacktimer = 0.0f;
		//���̃`�F���W�X�e�[�g��false�ɂ���B
		blackChangestate = false;
	}
	//���̐��E�̎������̃`�F���W�X�e�[�g��true����LB2�������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonLB1) && worldstate == 1 && blackChangestate == true)
	{
		//���̐��E�ɂ���
		worldstate = 0;
		//���^�C�}�[��0.0f�ɂ���B
		whitetimer = 0.0f;
		//���̃`�F���W�X�e�[�g��false�ɂ���B
		whiteChangestate = false;
	}
}

void Game::Render(RenderContext& rc)
{
}