#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Wall.h"
#include "Wallbreak.h"
#include "Breakrock.h"
#include "Enemy.h"
#include "Mediumboss.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "MoveFloor.h"
#include "MoveFloorWall.h"
#include "GameClear.h"

//�X�J�C�L���[�u�B
#include "nature/SkyCube.h"

bool Game::Start()
{
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetType(enSkyCubeType_DayToon_2);
	m_skyCube->SetType2(enSkyCubeType_NightToon_2);
	m_skyCube->SetScale({ 700.0f });
	m_skyCube->SetScale2({ 700.0f });
	m_skyCube->SetLuminance({ 0.1f });
	m_skyCube->SetLuminance2({ 0.1f });

	//m_wall = NewGO<Wall>(0, "wall");
	//m_wallbreak = NewGO<Wallbreak>(0, "wallbreak");
	//m_player = NewGO<Player>(0, "player");
	//m_backGround = NewGO<BackGround>(0, "backGround");
	//m_enemy = NewGO<Enemy>(0, "enemy");

	//���x�����\�z����B
	m_levelRender.Init("Assets/level3D/stagelevel.tkl",
		[&](LevelObjectData& objData)
	{
		//���O��human��������B
		if (objData.EqualObjectName(L"kachujin_g_rosales") == true)
		{
			//�v���C���[�̃I�u�W�F�N�g���쐬����B
			m_player = NewGO<Player>(1, "player");
			//���W��ݒ肷��B
			m_player->SetPosition(objData.position);
			//��]��ݒ肷��B
			m_player->SetRotation(objData.rotation);
			//�傫����ݒ肷��B
			m_player->SetScale(objData.scale);
			//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�܂��B
			return true;
		}
		//���O��background��������B
		else if (objData.EqualObjectName(L"background") == true)
		{
			//�w�i�I�u�W�F�N�g���쐬����B
			m_backGround = NewGO<BackGround>(0, "background");
			//���W��ݒ肷��B
			m_backGround->SetPosition(objData.position);
			//�傫����ݒ肷��B
			m_backGround->SetScale(objData.scale);
			//��]��ݒ肷��B
			m_backGround->SetRotation(objData.rotation);
			return true;
		}
		//���O��vampire_a_lusth��������B
		else if (objData.EqualObjectName(L"vampire_a_lusth") == true)
		{
			//�G�l�~�[�̃I�u�W�F�N�g���쐬����B
			Enemy* enemy = NewGO<Enemy>(0, "enemy");
			//���W��ݒ肷��B
			enemy->SetPosition(objData.position);
			//��]��ݒ肷��B
			enemy->SetRotation(objData.rotation);
			//�傫����ݒ肷��B
			enemy->SetScale(objData.scale);
			//HP��5�ɐݒ肷��B
			enemy->SetHP(1);
			//�쐬�����G�l�~�[�̘a�𐔂������̂ŁA+1����B
			m_numEnemy++;
			return true;
		}
		//���O��movefloor��������B
		else if (objData.EqualObjectName(L"movefloor") == true)
		{
			//�������I�u�W�F�N�g���쐬����B
			m_movefloor = NewGO<MoveFloor>(0, "movefloor");
			//���W��ݒ肷��B
			m_movefloor->SetPosition(objData.position);
			//�傫����ݒ肷��B
			m_movefloor->SetScale(objData.scale);
			//��]��ݒ肷��B
			m_movefloor->SetRotation(objData.rotation);
			return true;
		}
		//���O��breakrock��������B
		else if (objData.EqualObjectName(L"breakrock") == true)
		{
			//�󂹂��I�u�W�F�N�g���쐬����B
			m_breakrock = NewGO<Breakrock>(0, "background");
			//���W��ݒ肷��B
			m_breakrock->SetPosition(objData.position);
			//�傫����ݒ肷��B
			m_breakrock->SetScale(objData.scale);
			//��]��ݒ肷��B
			m_breakrock->SetRotation(objData.rotation);
			return true;
		}
		return true;
	});

	return true;
}

Game::~Game()
{
	DeleteGO(m_backGround);
	DeleteGO(m_gameCamera);
	DeleteGO(m_enemy);
	DeleteGO(m_player);
	DeleteGO(m_skyCube);
	DeleteGO(m_movefloor);
	DeleteGO(m_breakrock);
}

void Game::KnockCount()
{
	if (m_enemykillcount >= 4&& m_MediumbossNewGO == false)
	{
		m_enemykillcount = 0;
		m_MediumbossNewGO = true;
	}
	else if (m_MediumbossNewGO == true)
	{
		BossNewGO();
		m_MediumbossNewGO = false;
	}
	if (m_BossKill == true)
	{
		NewGO<GameClear>(0,"gameclear");
		DeleteGO(this);
	}
}

void Game::BossNewGO()
{
	//���{�X�̃I�u�W�F�N�g���쐬����B
	Mediumboss* mediumboss = NewGO<Mediumboss>(0, "mediumboss");
	mediumboss->SetEntry(true);
	m_gameCamera->SetCameraState(1);
	//���x�����\�z����B
	m_levelRender.Init("Assets/level3D/stagelevel.tkl",
		[&](LevelObjectData& objData)
	{
		//���O��maw_j_laygo��������B
		if (objData.EqualObjectName(L"maw_j_laygo") == true)
		{
		//���W��ݒ肷��B
		mediumboss->SetPosition(objData.position);
		//��]��ݒ肷��B
		mediumboss->SetRotation(objData.rotation);
		//�傫����ݒ肷��B
		mediumboss->SetScale(objData.scale);
		//HP��5�ɐݒ肷��B
		mediumboss->SetHP(1);
		//�쐬�����G�l�~�[�̘a�𐔂������̂ŁA+1����B
		m_numEnemy++;
		return true;
		}
		return true;
	});
}

void Game::MoveFloorWallNewGO()
{
	//�������̕ǃI�u�W�F�N�g���쐬����B
	m_movefloorwall = NewGO<MoveFloorWall>(0, "movefloorwall");
	//���x�����\�z����B
	m_levelRender.Init("Assets/level3D/stagelevel.tkl",
		[&](LevelObjectData& objData)
	{
		//���O��movefloorwall��������B
		if (objData.EqualObjectName(L"movefloorwall") == true)
		{
			//���W��ݒ肷��B
			m_movefloorwall->SetPosition(objData.position);
			//�傫����ݒ肷��B
			m_movefloorwall->SetScale(objData.scale);
			//��]��ݒ肷��B
			m_movefloorwall->SetRotation(objData.rotation);
			return true;
		}
		return true;
	});
}

void Game::Update()
{
	int b = m_enemykillcount;
	wchar_t wcsbuf1[256];
	swprintf_s(wcsbuf1, 256, L"�Ă�������������%d", b);

	//�\������e�L�X�g��ݒ�B
	m_fontRender.SetText(wcsbuf1);
	//�t�H���g�̈ʒu��ݒ�B
	m_fontRender.SetPosition(Vector3(-710.0f, -185.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_fontRender.SetScale(3.0f);
	//���F�ɐݒ�
	m_fontRender.SetColor(g_vec4Black);
	Timer();
	WorldChanege();
	KnockCount();
	if (m_movefloorwallstate == true && m_floorwallcount == false)
	{
		MoveFloorWallNewGO();
		m_floorwallcount = true;
	}

	if (FindGO<Player>("player")->GetPosition().y <= -900.0f)
	{
		DeleteGO(this);
	}
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

	//�J�����X�e�[�g��1(���{�X�o����)�Ȃ�^�C�}�[���擾����B
	if (m_gameCamera->GetCameraState() == 1)
	{
		m_gamecameratimer += g_gameTime->GetFrameDeltaTime();
	}
	//3�b�o�߂�����v���C���[���_�ɖ߂��B
	if (m_gamecameratimer >= 4.0f)
	{
		m_gameCamera->SetCameraState(0);
		m_gamecameratimer = 0.0f;
	}
}
void Game::WorldChanege()
{
	//���̐��E�̎������̃`�F���W�X�e�[�g��true����LB2�������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonLB2) && worldstate == 0 && whiteChangestate == true)
	{
		//���̐��E�ɂ���
		worldstate = 1;
		m_skyCube->SetWorldState({ 1 });//�O����,�P����;
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
		m_skyCube->SetWorldState({ 0 });//�O����,�P����;
		//���^�C�}�[��0.0f�ɂ���B
		whitetimer = 0.0f;
		//���̃`�F���W�X�e�[�g��false�ɂ���B
		whiteChangestate = false;
	}
}

void Game::Render(RenderContext& rc)
{
	m_fontRender.Draw(rc);
}