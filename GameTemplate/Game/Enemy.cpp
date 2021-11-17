#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "Game.h"

#include <time.h>
#include <stdlib.h>

//CollisionObject���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"

bool Enemy::Start()
{
	//�A�j���[�V������ǂݍ���
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/vampire/Idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/vampire/Walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/vampire/Run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack1].Load("Assets/animData/vampire/attack.tka");
	m_animationClips[enAnimationClip_Attack1].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack2].Load("Assets/animData/vampire/attack2.tka");
	m_animationClips[enAnimationClip_Attack2].SetLoopFlag(false);
	/*
	m_animationClips[enAnimationClip_Attack3].Load("Assets/animData/vampire/attack3.tka");
	m_animationClips[enAnimationClip_Attack3].SetLoopFlag(false);
	*/
	/*
	m_animationClips[enAnimationClip_MagicAttack].Load("Assets/animData/vampire/magicattack.tka");
	m_animationClips[enAnimationClip_MagicAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_HitDamage].Load("Assets/animData/vampire/receivedamage.tka");
	m_animationClips[enAnimationClip_HitDamage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/vampire/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Winner].Load("Assets/animData/vampire/winner.tka");
	m_animationClips[enAnimationClip_Winner].SetLoopFlag(false);
	*/

	//�G�̃��f����ǂݍ���
	m_enemy.Init("Assets/modelData/vampire.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	//���W��ݒ肷��B
	Vector3 position = m_position;
	position.z = 200.0f;
	m_enemy.SetPosition(position);
	m_enemy.SetScale({ 8.0f,8.0f,8.0f });
	m_enemy.SetScale(Vector3::One * 2.5f);
	//��]��ݒ肷��B
	m_enemy.SetRotation(m_rotation);
	m_enemy.Update();
	//�L�����N�^�[�R���g���[���[���������B
	m_charaCon.Init(
		25.0f,			//���a�B
		160.0f,			//�����B
		position		//���W�B
	);

	m_player = FindGO<Player>("player");

	//�������������B
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	//�uLeftHand�v�{�[����ID(�ԍ�)���擾����B
	m_LeftHandId = m_enemy.FindBoneID(L"LeftHand");
	//�uRightHand�v�{�[����ID(�ԍ�)���擾����B
	m_RightHandId = m_enemy.FindBoneID(L"RightHand");

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_enemy.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});

	return true;
};

Enemy::Enemy()
{

}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	Chase();
	Attack();
	Rotation();
	Collision();
	PlayAnimation();
	ManageState();

	//���f���̍X�V�����B
	m_enemy.Update();
}

void Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	
	//�L�[�̖��O���uattack_start�v�̎��B
	if (wcscmp(eventName, L"attack_start") == 0) {
		//�U���������true�ɂ���B
		m_isUnderAttack = true;
	}
	//�L�[�̖��O���uattack_end�v�̎��B
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//�U���������false�ɂ���B
		m_isUnderAttack = false;
	}
	
}


void Enemy::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}
	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐��B
	//m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�B
	//atan2���g�p���āA�p�x�����߂Ă���B
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_enemy.SetRotation(m_rotation);

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Enemy::Chase()
{
	//�ǐՃX�e�[�g�łȂ��Ȃ�A�ǐՏ����͂��Ȃ��B
	if (m_enemyState != enEnemyState_Chase)
	{
		return;
	}

	//�G�l�~�[���ړ�������B
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		//�d�͂�0�ɂ���B
		m_moveSpeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//���W��ݒ肷��B
	m_enemy.SetPosition(modelPosition);
}

void Enemy::Collision()
{
	//��_���[�W�A���邢�̓_�E���X�e�[�g�̎��́B
	//�����蔻�菈���͂��Ȃ��B
	if (m_enemyState == enEnemyState_ReceiveDamage ||
		m_enemyState == enEnemyState_Down)
	{
		return;
	}

	//Player���쐬�����A�U���̃q�b�g����p�̃R���W�����̔z����擾�B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//for���Ŕz����񂷁B
	for (auto collision : collisions)
	{
		//�U���̃R���W�����ƓG�̃L�����R�����B
		//�Փ˂��Ă�����B
		if (collision->IsHit(m_charaCon) == true)
		{
			DeleteGO(this);
		}
	}
	//Player���쐬�����A�U���̃q�b�g����p�̃R���W����2�̔z����擾�B
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_attack2");
	//for���Ŕz����񂷁B
	for (auto collision : collisions2)
	{
		//�U���̃R���W�����ƓG�̃R���W�������B
		//�Փ˂��Ă�����B
		if (collision->IsHit(m_charaCon) == true)
		{
			DeleteGO(this);
		}
	}

}

void Enemy::Attack()
{
	
	if (m_enemyattackmotion == 1)
	{
	    //�U���X�e�[�g�łȂ��Ȃ珈�������Ȃ��B
		if (m_enemyState != enEnemyState_Attack1)
		{
			m_enemyattackmotion = 0;
			return;
		}
	}
	else if (m_enemyattackmotion == 0)
	{
		//�U�����łȂ��Ȃ�A���������Ȃ��B
		if (m_enemyState != enEnemyState_Attack2)
		{
			return;
		}
	}

	//�U�����ł���΁B
	if (m_isUnderAttack == true && m_enemyattackmotion == 1)
	{
		//�U���p�̃R���W�������쐬����B
		MakeAttackCollision();
	}
	if (m_isUnderAttack == true && m_enemyattackmotion == 0)
	{
		//�U���p�̃R���W�������쐬����B
		MakeAttackCollision2();
	}
}

const bool Enemy::SearchPlayer() const
{
	//�G�l�~�[����v���C���[�Ɍ������x�N�g�������߂�B
	Vector3 diff = m_player->GetPosition() - m_position;

	//�v���C���[�ɂ�����x�߂�������B
	if (diff.LengthSq() <= 700.0 * 700.0f)
	{
		//�G�l�~�[����v���C���[�Ɍ������x�N�g���𐳋K��(�傫����1)����B
		diff.Normalize();
		//�G�l�~�[�̐��ʂ̃x�N�g���ƁA�G�l�~�[����v���C���[�Ɍ������x�N�g���́B
		//����(cos��)�����߂�B
		float cos = m_forward.Dot(diff);
		//����(cos��)����p�x(��)�����߂�B
		float angle = acosf(cos);
		//�p�x(��)��120��(����p)��菬������΁B
		if (angle <= (Math::PI / 180.0f) * 120.0f)
		{
			//�v���C���[���������I
			return true;
		}
	}
	//�v���C���[���������Ȃ������B
	return false;
}

void Enemy::MakeAttackCollision()
{
	
	//�U�������蔻��p�̃R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//���W���v���C���[�̏����O�ɐݒ肷��B
	collisionPosition += m_forward * 50.0f;
	//����̃R���W�������쐬����B
	collisionObject->CreateSphere(collisionPosition,        //���W�B
		Quaternion::Identity,                               //��]�B
		15.0f                                               //���a�B
	);
	collisionObject->SetName("enemy_attack");

	//���̃{�[���̃��[���h�s����擾����B
	Matrix matrix = m_enemy.GetBone(m_RightHandId)->GetWorldMatrix();
	//�uLeftHand�v�{�[���̃��[���h�s����R���W�����ɓK�p����B
	collisionObject->SetWorldMatrix(matrix);
}

void Enemy::MakeAttackCollision2()
{

	//�U�������蔻��p�̃R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject2 = NewGO<CollisionObject>(0);
	Vector3 collisionPosition2 = m_position;
	//���W���v���C���[�̏����O�ɐݒ肷��B
	collisionPosition2 += m_forward * 50.0f;
	//����̃R���W�������쐬����B
	collisionObject2->CreateSphere(collisionPosition2,        //���W�B
		Quaternion::Identity,                               //��]�B
		15.0f                                               //���a�B
	);
	collisionObject2->SetName("enemy_attack2");

	//���̃{�[���̃��[���h�s����擾����B
	Matrix matrix2 = m_enemy.GetBone(m_LeftHandId)->GetWorldMatrix();
	//�uRightHand�v�{�[���̃��[���h�s����R���W�����ɓK�p����B
	collisionObject2->SetWorldMatrix(matrix2);
}

void Enemy::ProcessCommonStateTransition()
{
	//�e�^�C�}�[���������B
		//�ҋ@���ԂƒǐՎ��Ԃ𐧌����邽�߁B
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
	Vector3 diff = m_player->GetPosition() - m_position;

	//�v���C���[����������B
	if (SearchPlayer() == true)
	{
		//�ʏ�U���ł��Ȃ������Ȃ�B
		if (IsCanAttack() == false)
		{
			//�x�N�g���𐳋K������B
			diff.Normalize();
			//�ړ����x��ݒ肷��B
			m_moveSpeed = diff * 250.0f;
			//�����ɂ���āA�ǐՂ����邩���@�U�������邩���肷��B	
			int ram = rand() % 100;
			//������40�ȏ�Ȃ�B
			if (ram > 40)
			{
				//�ǐՃX�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_Chase;
				return;
			}
			//������40�����Ȃ�B
			else {
				//�ǐՍU���X�e�[�g�ɑJ�ڂ���B
				//m_enemyState = enEnemyState_MagicAttack;
				m_enemyState = enEnemyState_Chase;
				return;
			}
		}
		//�U���ł��鋗���Ȃ�B
		else
		{
			//�����ɂ���āA�U�����邩�ҋ@�����邩�����肷��B	
			int ram = rand() % 100;
			if (ram > 60)
			{
					m_enemyState = enEnemyState_Attack1;
					m_enemyattackmotion = 1;


				m_isUnderAttack = false;
				return;
			}
			else
			{
				//�ҋ@�X�e�[�g�ɑJ�ڂ���B
				m_enemyState = enEnemyState_Idle;
				return;
			}
		}
	}
	//�v���C���[���������Ȃ���΁B
	else
	{
		//�ҋ@�X�e�[�g�ɑJ�ڂ���B
		m_enemyState = enEnemyState_Idle;
		return;

	}
}

void Enemy::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//�ҋ@���Ԃ�������x�o�߂�����B
	if (m_idleTimer >= 0.9f)
	{
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();
	}

}

void Enemy::ProcessWalkStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���B
	ProcessCommonStateTransition();
}

void Enemy::ProcessRunStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���B
	ProcessCommonStateTransition();
}

void Enemy::ProcessChaseStateTransition()
{
	//�U���ł��鋗���Ȃ�B
	if (IsCanAttack() == true)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
		return;
	}
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//�ǐՎ��Ԃ�������x�o�߂�����B
	if (m_chaseTimer >= 0.8f)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Enemy::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_enemy.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}
void Enemy::ManageState()
{
	switch (m_enemyState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enEnemyState_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessIdleStateTransition();
		break;
		//�ǐՃX�e�[�g�̎��B
	case enEnemyState_Chase:
		//�ǐՃX�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessChaseStateTransition();
		break;
		//�U���X�e�[�g�̎��B
	case enEnemyState_Attack1:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessAttackStateTransition();
		break;
		//�U���X�e�[�g2�̎��B
	case enEnemyState_Attack2:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessAttackStateTransition();
		break;
	}
}
void Enemy::PlayAnimation()
{
	m_enemy.SetAnimationSpeed(1.0f);
	switch (m_enemyState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enEnemyState_Idle:
		//�ҋ@�A�j���[�V�������Đ��B
		m_enemy.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//�ǐՃX�e�[�g�̎��B
	case enEnemyState_Chase:
		//����A�j���[�V�������Đ��B
		m_enemy.PlayAnimation(enAnimationClip_Run, 0.1f);
		break;
		//�G�l�~�[�X�e�[�g��enPlayerState_Attack1��������B
	case enEnemyState_Attack1:
		//attack���[�V�������Đ�
		m_enemy.PlayAnimation(enAnimationClip_Attack1, 0.1f);
		break;
		//�G�l�~�[�X�e�[�g��enPlayerState_Attack2��������B
	case enEnemyState_Attack2:
		//attack���[�V�������Đ�
		m_enemy.PlayAnimation(enAnimationClip_Attack2, 0.1f);
		break;
	default:
		break;
	}
}

const bool Enemy::IsCanAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//�G�l�~�[�ƃv���C���[�̋������߂�������B
	if (diff.LengthSq() <= 100.0f * 100.0f)
	{
		//�U���ł���I
		return true;
	}
	//�U���ł��Ȃ��B
	return false;
}

void Enemy::Render(RenderContext& rc)
{
	m_enemy.Draw(rc);
}