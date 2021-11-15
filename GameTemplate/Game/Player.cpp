#include "stdafx.h"
#include "Player.h"
#include "game.h"
#include "Wall.h"

//CollisionObject���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"

bool Player::Start()
{
	//�A�j���[�V������ǂݍ���
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/kachujin/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/kachujin/Dwarf_Walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/kachujin/Run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack1].Load("Assets/animData/kachujin/attack.tka");
	m_animationClips[enAnimationClip_Attack1].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack2].Load("Assets/animData/kachujin/attack2.tka");
	m_animationClips[enAnimationClip_Attack2].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack3].Load("Assets/animData/kachujin/attack3.tka");
	m_animationClips[enAnimationClip_Attack3].SetLoopFlag(false);
	/*
	m_animationClips[enAnimationClip_MagicAttack].Load("Assets/animData/kachujin/magicattack.tka");
	m_animationClips[enAnimationClip_MagicAttack].SetLoopFlag(false);
	*/
	m_animationClips[enAnimationClip_HitDamage].Load("Assets/animData/kachujin/receivedamage.tka");
	m_animationClips[enAnimationClip_HitDamage].SetLoopFlag(false);
	/*
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/kachujin/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Winner].Load("Assets/animData/kachujin/winner.tka");
	m_animationClips[enAnimationClip_Winner].SetLoopFlag(false);
	*/

	//���f����ǂݍ���
	m_modelRender.Init("Assets/modelData/Kachujin.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	m_modelRender2.Init("Assets/modelData/blackKachujin.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);

	m_modelRender.SetScale({ 1.5f,1.5f,1.5f });
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetPosition({ 1.0f,0.0f,1.0f });
	m_modelRender.Update();
	m_modelRender2.SetScale({ 1.5f,1.5f,1.5f });
	m_modelRender2.SetPosition(m_position2);
	m_modelRender2.SetPosition({ 1.0f,0.0f,1.0f });
	m_modelRender2.Update();

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});

	//�uLeftHand�v�{�[����ID(�ԍ�)���擾����B
	m_LeftHandId = m_modelRender.FindBoneID(L"LeftHand");
	//�uRightHand�v�{�[����ID(�ԍ�)���擾����B
	m_RightHandId = m_modelRender.FindBoneID(L"RightHand");

	//�R���C�_�[���������B�i�X�t�B�A�ɂȂ��Ă�)
	m_sphereCollider.Create(48.0f);

	//�L�����R��������������B
	characterController.Init(25.0f, 25.0f, m_position);

	return true;
}

Player::Player()
{

}

Player::~Player()
{
}

void Player::Update() //���1�b�Ԃ�60��Ăяo�����
{
	Moji();
	Move();
	ManageState();
	PlayAnimation();
	Attack();
	Timer();
	Rotation();
	//�G�`������̍X�V�����B
	m_modelRender.Update();
	m_modelRender2.Update();
}

void Player::Move()
{
	//�ړ��ł��Ȃ���Ԃł���΁A�ړ������͂��Ȃ��B
	if (IsEnableMove() == false)
	{
		return;
	}

	m_position2 = m_position;
	//xz�̈ړ����x��0.0f�ɂ���B
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ��擾�B
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	//y�����ɂ͈ړ������Ȃ��B
	forward.y = 0.0f;
	right.y = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ�120.0f����Z�B
	right *= stickL.x * 700.0f;
	forward *= stickL.y * 700.0f;

	//�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	m_moveSpeed += right + forward;

	//�L�����N�^�[�R���g���[���[���g���č��W���ړ�������B
	m_position = characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	//���̐��E�̎������̃`�F���W�X�e�[�g��true����LB2�������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonLB2)&& worldstate==0 && whiteChangestate == true)
	{
		//���̐��E�ɂ���
		worldstate = 1;
		//���^�C�}�[��0.0f�ɂ���B
		blacktimer = 0.0f;
		//���̃`�F���W�X�e�[�g��false�ɂ���B
		blackChangestate = false;

		m_wall = NewGO<Wall>(0, "wall");
	}
	//���̐��E�̎������̃`�F���W�X�e�[�g��true����LB2�������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonLB1) && worldstate == 1&& blackChangestate ==true )
	{
		//���̐��E�ɂ���
		worldstate = 0;
		//���^�C�}�[��0.0f�ɂ���B
		whitetimer = 0.0f;
		//���̃`�F���W�X�e�[�g��false�ɂ���B
		whiteChangestate = false;
	}

	//�G�`������ɍ��W��������B
	m_modelRender.SetPosition(m_position);
	m_modelRender2.SetPosition(m_position);
}
void Player::ProcessIdleStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

void Player::ProcessWalkStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

void Player::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I������Ȃ�B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Player::ProcessReceiveDamageStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}
void Player::ManageState()
{
	switch (m_playerState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enPlayerState_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessIdleStateTransition();
		break;
		//�����X�e�[�g�̎��B
	case enPlayerState_Walk:
		//�����X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessWalkStateTransition();
		break;
		//����X�e�[�g�̎��B
	case enPlayerState_Run:
		//����X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessRunStateTransition();
		break;
		//�U���X�e�[�g1�̎��B
	case enPlayerState_Attack1:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessAttackStateTransition();
		break;
		//�U���X�e�[�g2�̎��B
	case enPlayerState_Attack2:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessAttackStateTransition();
		break;
		//�U���X�e�[�g3�̎��B
	case enPlayerState_Attack3:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessAttackStateTransition();
		break;
		//��_���[�W���X�e�[�g�̎��B
	case enPlayerState_ReceiveDamage:
		//��_���[�W���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessReceiveDamageStateTransition();
		break;
	}
}

void Player::ProcessCommonStateTransition()
{
	//RB1�{�^���������ꂽ��B
	if (g_pad[0]->IsTrigger(enButtonRB1)&& player_attackmotion == 0)
	{
		//m_attacktimer��0.21f�ɂ���B(0.3f�̗P�\���������邽��)
		m_attacktimer = 0.21f;
		//�U�����ɂ���B
		m_attackstate = true;
		//�U���X�e�[�g�Ɉڍs����B
		m_playerState = enPlayerState_Attack1;
		//player_atacckmotion��+1����B
		player_attackmotion++;
		//�t���O��false�ɂ���B
		m_isUnderAttack = false;
		return;
	}
	else if (g_pad[0]->IsTrigger(enButtonRB1) && player_attackmotion == 1)
	{
		//m_attacktimer��0.2f�ɂ���B(0.3f�̗P�\���������邽��)
		m_attacktimer = 0.2f;
		//�U�����ɂ���B
		m_attackstate = true;
		//�U���X�e�[�g�Ɉڍs����B
		m_playerState = enPlayerState_Attack2;
		//player_atacckmotion��+1����B
		player_attackmotion++;
		//�t���O��false�ɂ���B
		m_isUnderAttack = false;
		return;
	}
	else if (g_pad[0]->IsTrigger(enButtonRB1) && player_attackmotion == 2)
	{
		//m_attacktimer��0.36f�ɂ���B(0.3f�̗P�\���������邽��)
		m_attacktimer = 0.36f;
		//�U�����ɂ���B
		m_attackstate = true;
		//�U���X�e�[�g�Ɉڍs����B
		m_playerState = enPlayerState_Attack3;
		//player_atacckmotion�����Z�b�g����B
		player_attackmotion=0;
		//�t���O��false�ɂ���B
		m_isUnderAttack = false;
		return;
	}
	if (m_attackstate == false)
	{
		//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			//�X�e�[�g��2(����)�ɂ���B
			m_playerState = enPlayerState_Walk;
		}
		//x��z�̈ړ����x������������(�X�e�B�b�N�̓��͂�����������)�B
		else
		{
			//�X�e�[�g��0(�ҋ@)�ɂ���B
			m_playerState = enPlayerState_Idle;
		}
	}
	else if (m_attackstate == true)
	{
		if (m_attacktimer >= 3.0f)
		{
			if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
			{
				//�X�e�[�g��2(����)�ɂ���B
				m_playerState = enPlayerState_Walk;
			}
			//x��z�̈ړ����x������������(�X�e�B�b�N�̓��͂�����������)�B
			else
			{
				//�X�e�[�g��0(�ҋ@)�ɂ���B
				m_playerState = enPlayerState_Idle;
			}
		}
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�L�[�̖��O���uattack_start�v�̎��B
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		//�U�����ɂ���B
		m_isUnderAttack = true;
	}
	//�L�[�̖��O���uattack_end�v�̎��B
	else if (wcscmp(eventName, L"attack_end") == 0)
	{
		//�U�����I���B
		m_isUnderAttack = false;
	}

}

void Player::PlayAnimation()
{

	//switch���B
	switch (m_playerState) {
		//�v���C���[�X�e�[�g��idle��������B
	case enPlayerState_Idle:
		//�ҋ@�A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		m_modelRender2.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//�v���C���[�X�e�[�g��walk��������B
	case enPlayerState_Walk:
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		m_modelRender2.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
		//�v���C���[�X�e�[�g��HitDamage��������B
	case enAnimationClip_HitDamage:
		//��_���[�W���[�V�������Đ�
		m_modelRender.PlayAnimation(enAnimationClip_HitDamage, 0.1f);
		m_modelRender2.PlayAnimation(enAnimationClip_HitDamage, 0.1f);
		break;
		//�v���C���[�X�e�[�g��enPlayerState_Attack1��������B
	case enPlayerState_Attack1:
		//attack���[�V�������Đ�
		m_modelRender.PlayAnimation(enAnimationClip_Attack1, 0.1f);
		m_modelRender2.PlayAnimation(enAnimationClip_Attack1, 0.1f);
		break;
		//�v���C���[�X�e�[�g��enPlayerState_Attack2��������B
	case enPlayerState_Attack2:
		//attack���[�V�������Đ�
		m_modelRender.PlayAnimation(enAnimationClip_Attack2);
		m_modelRender2.PlayAnimation(enAnimationClip_Attack2);
		break;
		//�v���C���[�X�e�[�g��enPlayerState_Attack3��������B
	case enPlayerState_Attack3:
		//attack���[�V�������Đ�
		m_modelRender.PlayAnimation(enAnimationClip_Attack3);
		m_modelRender2.PlayAnimation(enAnimationClip_Attack3);
		break;
	}
	
}

void Player::Attack()
{
	if (player_attackmotion == 1)
	{
		//�U�����łȂ��Ȃ�A���������Ȃ��B
		if (m_playerState != enPlayerState_Attack1)
		{
			return;
		}
	}
	else if (player_attackmotion == 2)
	{
		//�U�����łȂ��Ȃ�A���������Ȃ��B
		if (m_playerState != enPlayerState_Attack2)
		{
			return;
		}
	}
	else if (player_attackmotion == 0)
	{
		//�U�����łȂ��Ȃ�A���������Ȃ��B
		if (m_playerState != enPlayerState_Attack3)
		{
			return;
		}
	}

	//�U�����蒆�ł���player_attackmotion��1�ł���΁B
	if (m_isUnderAttack == true && player_attackmotion == 1)
	{
		//�U���p�̃R���W�������쐬����B
		MakeAttackCollision();
	}
	//�U�����蒆�ł���player_attackmotion��2�ł���΁B
	if (m_isUnderAttack == true && player_attackmotion == 2)
	{
		//�U���p�̃R���W�������쐬����B
		MakeAttackCollision2();
	}
	//�U�����蒆�ł���player_attackmotion��0�ł���΁B
	if (m_isUnderAttack == true && player_attackmotion == 0)
	{
		//�U���p�̃R���W�������쐬����B
		MakeAttackCollision();
	}

}

void Player::MakeAttackCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//���W���v���C���[�̏����O�ɐݒ肷��B
	collisionPosition += m_forward * 50.0f;
	//����̃R���W�������쐬����B
	collisionObject->CreateSphere(collisionPosition,        //���W�B
		Quaternion::Identity,                               //��]�B
		15.0f                                               //���a�B
	);
	collisionObject->SetName("player_attack");

	//�uLeftHand�v�{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_LeftHandId)->GetWorldMatrix();
	//�uLeftHand�v�{�[���̃��[���h�s����R���W�����ɓK�p����B
	collisionObject->SetWorldMatrix(matrix);
}

void Player::MakeAttackCollision2()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject2 = NewGO<CollisionObject>(1);

	Vector3 collisionPosition2 = m_position;
	//���W���v���C���[�̏����O�ɐݒ肷��B
	collisionPosition2 += m_forward * 50.0f;
	//����̃R���W�������쐬����B
	collisionObject2->CreateSphere(collisionPosition2,        //���W�B
		Quaternion::Identity,                               //��]�B
		15.0f                                               //���a�B
	);
	collisionObject2->SetName("player_attack2");

	//�uRightHand�v�{�[���̃��[���h�s����擾����B
	Matrix matrix2 = m_modelRender.GetBone(m_RightHandId)->GetWorldMatrix();
	//�uRightHand�v�{�[���̃��[���h�s����R���W�����ɓK�p����B
	collisionObject2->SetWorldMatrix(matrix2);
}

void Player::Timer()
{
	//�U�����^�C�}�[���擾����
	if (m_attackstate == true)
	{
		m_attacktimer += g_gameTime->GetFrameDeltaTime();
	}
	//�U��������m_attacktimer��1.3f���z���Ă�����(�U���I����0.5�b�o�߂�)
	if (m_attackstate == true && m_attacktimer >= 1.3f)
	{
		//m_attackstate��false�ɂ���m_attacktimer��0�ɖ߂�
		m_attackstate = false;
		m_attacktimer = 0.0f;
	}
	//���̐��E�̎������̃`�F���W�X�e�[�g��false�Ȃ甒�^�C�}�[���擾����B
	if (worldstate == 0&&whiteChangestate==false)
	{
		whitetimer += g_gameTime->GetFrameDeltaTime();
	}
	//���̐��E�̃^�C�}�[��2.0f���z���Ă�����
	if (whitetimer >= 2.0f&&whiteChangestate == false)
	{
		//���̃`�F���W�X�e�[�g��true�ɂ���B
		whiteChangestate = true;
	}
	//���̐��E�̎������̃`�F���W�X�e�[�g��false�Ȃ獕�^�C�}�[���擾����B
	if (worldstate == 1&& blackChangestate == false)
	{
		blacktimer += g_gameTime->GetFrameDeltaTime();
	}
	//���̐��E�̃^�C�}�[��2.0f���z���Ă�����
	if (blacktimer >= 2.0f&&blackChangestate == false)
	{
		blackChangestate = true;
	}

	wchar_t wcsbuf4[256];
	swprintf_s(wcsbuf4, 256, L"���`�F���W%d�b", int(blacktimer));

	//�\������e�L�X�g��ݒ�B
	m_fontRender4.SetText(wcsbuf4);
	//�t�H���g�̈ʒu��ݒ�B
	m_fontRender4.SetPosition(Vector3(-500.0f, 520.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_fontRender4.SetScale(2.1f);
	//���F�ɐݒ�
	m_fontRender4.SetColor(g_vec4White);

	wchar_t wcsbuf5[256];
	swprintf_s(wcsbuf5, 256, L"���`�F���W%d�b", int(whitetimer));

	//�\������e�L�X�g��ݒ�B
	m_fontRender5.SetText(wcsbuf5);
	//�t�H���g�̈ʒu��ݒ�B
	m_fontRender5.SetPosition(Vector3(500.0f, 520.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_fontRender5.SetScale(2.1f);
	//���F�ɐݒ�
	m_fontRender5.SetColor(g_vec4White);
}

void Player::Rotation()
{
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//�L�����N�^�[�̕�����ς���B
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		//�G�`������ɉ�]��������B
		m_modelRender.SetRotation(m_rotation);
		m_modelRender2.SetRotation(m_rotation);
		//�v���C���[�̑O�����̃x�N�g����ݒ肷��B
		m_forward = Vector3(0.0f, 0.0f, 1.0f);
		//�x�N�g���ɃN�E�H�[�^�j�I����K�����ăv���C���[�̌����ɉ�]������
		m_rotation.Apply(m_forward);
	}
}


void Player::Moji()
{

int x = m_position.x;
wchar_t wcsbuf[256];
swprintf_s(wcsbuf, 256, L"x:%d", x);

//�\������e�L�X�g��ݒ�B
m_fontRender.SetText(wcsbuf);
//�t�H���g�̈ʒu��ݒ�B
m_fontRender.SetPosition(Vector3(-500.0f, 200.0f, 0.0f));
//�t�H���g�̑傫����ݒ�B
m_fontRender.SetScale(1.0f);

int y = m_position.y;
wchar_t wcsbuf2[256];
swprintf_s(wcsbuf2, 256, L"y:%d", y);

//�\������e�L�X�g��ݒ�B
m_fontRender2.SetText(wcsbuf2);
//�t�H���g�̈ʒu��ݒ�B
m_fontRender2.SetPosition(Vector3(-500.0f, 150.0f, 0.0f));
//�t�H���g�̑傫����ݒ�B
m_fontRender2.SetScale(1.0f);

int z = m_position.z;
wchar_t wcsbuf3[256];
swprintf_s(wcsbuf3, 256, L"z:%d", z);

//�\������e�L�X�g��ݒ�B
m_fontRender3.SetText(wcsbuf3);
//�t�H���g�̈ʒu��ݒ�B
m_fontRender3.SetPosition(Vector3(-500.0f, 100.0f, 0.0f));
//�t�H���g�̑傫����ݒ�B
m_fontRender3.SetScale(1.0f);

}

void Player::Render(RenderContext& rc)
{
	//�`�悷��B
	m_fontRender.Draw(rc);
	m_fontRender2.Draw(rc);
	m_fontRender3.Draw(rc);
	//m_fontRender4.Draw(rc);
	//m_fontRender5.Draw(rc);
	if (worldstate == 0)
	{
		m_modelRender.Draw(rc);
	}
	if (worldstate == 1)
	{
		m_modelRender2.Draw(rc);
	}
}