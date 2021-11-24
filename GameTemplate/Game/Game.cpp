#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Wall.h"
#include "Wallbreak.h"
#include "Breakrock.h"
#include "Enemy.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "MoveFloor.h"

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
		
		//���O��enemy��������B
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
			enemy->SetHP(5);
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
	DeleteGO(m_wallbreak);
	DeleteGO(m_gameCamera);
	DeleteGO(m_enemy);
	DeleteGO(m_player);
	DeleteGO(m_skyCube);
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
}