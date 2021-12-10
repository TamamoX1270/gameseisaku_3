#include "stdafx.h"
#include "Mediumboss.h"
#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "Game.h"

#include <time.h>
#include <stdlib.h>

//CollisionObject���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"
//EffectEmitter���g�p���鎞�̓t�@�C�����C���N���[�h����K�v������B
#include "graphics/effect/EffectEmitter.h"

bool Mediumboss::Start()
{
	//�A�j���[�V������ǂݍ���
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/maw_j_laygo/Idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/maw_j_laygo/Run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack1].Load("Assets/animData/maw_j_laygo/attack1.tka");
	m_animationClips[enAnimationClip_Attack1].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack2].Load("Assets/animData/maw_j_laygo/attack2.tka");
	m_animationClips[enAnimationClip_Attack2].SetLoopFlag(false);
	m_animationClips[enAnimationClip_HitDamage1].Load("Assets/animData/maw_j_laygo/hitdamage1.tka");
	m_animationClips[enAnimationClip_HitDamage1].SetLoopFlag(false);
	m_animationClips[enAnimationClip_HitDamage2].Load("Assets/animData/maw_j_laygo/hitdamage2.tka");
	m_animationClips[enAnimationClip_HitDamage2].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down1].Load("Assets/animData/maw_j_laygo/down1.tka");
	m_animationClips[enAnimationClip_Down1].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down2].Load("Assets/animData/maw_j_laygo/down2.tka");
	m_animationClips[enAnimationClip_Down2].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Roar].Load("Assets/animData/maw_j_laygo/roar.tka");
	m_animationClips[enAnimationClip_Roar].SetLoopFlag(false);
	/*
	m_animationClips[enAnimationClip_MagicAttack].Load("Assets/animData/vampire/magicattack.tka");
	m_animationClips[enAnimationClip_MagicAttack].SetLoopFlag(false);
	*/

	//�G�t�F�N�g��ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/blood.efk");

	//�G�̃��f����ǂݍ���
	m_enemy.Init("Assets/modelData/maw_j_laygo.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	m_enemy2.Init("Assets/modelData/maw_j_laygo.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	//���W��ݒ肷��B
	Vector3 position = m_position;
	Vector3 efpos = m_position;
	m_enemy.SetPosition(position);
	m_enemy.SetScale(m_scale);
	//��]��ݒ肷��B
	m_enemy.SetRotation(m_rotation);
	m_enemy.Update();
	m_enemy2.SetPosition(position);
	m_enemy2.SetScale(m_scale);
	//��]��ݒ肷��B
	m_enemy2.SetRotation(m_rotation);
	m_enemy2.Update();
	//�L�����N�^�[�R���g���[���[���������B
	m_charaCon.Init(
		2.5f,			//���a�B
		8.0f,			//�����B
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

	m_enemy.PlayAnimation(enAnimationClip_Roar, 0.1f);

	//NewGO���ɍĐ�����B
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(0);
	effectEmitter->SetScale({ 5.0f,5.0f,5.0f });
	effectEmitter->SetPosition(efpos);
	effectEmitter->Play();

	return true;
};

Mediumboss::Mediumboss()
{
}

Mediumboss::~Mediumboss()
{
}

void Mediumboss::Update()
{

	int x = m_hp;
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"HP:%d", x);

	//�\������e�L�X�g��ݒ�B
	m_fontRender.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_fontRender.SetPosition(Vector3(-30.0f, 20.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_fontRender.SetScale(1.0f);

	Chase();
	Attack();
	Rotation();
	Collision();
	PlayAnimation();
	ManageState();

	//���f���̍X�V�����B
	m_enemy.Update();
	m_enemy2.Update();
}

void Mediumboss::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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


void Mediumboss::Rotation()
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
	m_enemy2.SetRotation(m_rotation);

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Mediumboss::Chase()
{
	if (m_entrystate == true)
	{
		m_entryTimer += g_gameTime->GetFrameDeltaTime();
	}
	if (m_entryTimer >= 5.0f)
	{
		m_entrystate = false;
	}

	//�ǐՃX�e�[�g�łȂ����A�ǐՏ����͂��Ȃ��B
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
		m_enemy2.SetPosition(modelPosition);
}

void Mediumboss::Collision()
{
	//��_���[�W�A���邢�̓_�E���X�e�[�g�̎��́B
	//�����蔻�菈���͂��Ȃ��B
	if (m_enemyState == enEnemyState_HitDamage1 ||
		m_enemyState == enEnemyState_HitDamage2 ||
		m_enemyState == enEnemyState_Down ||
		m_hp == 0)
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
			//hitdamagecooltime��false�̎�
			if (hitdamagecooltime == false)
			{
				//hp���P���炷
				m_hp--;
			}
			hitdamagecooltime = true;
			//��_���[�W�X�e�[�g�ɑJ�ڂ���B
			m_enemyState = enEnemyState_HitDamage1;
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
			//hitdamagecooltime��false�̎�
			if (hitdamagecooltime == false)
			{
				//hp���P���炷
				m_hp--;
			}
			hitdamagecooltime = true;
			//��_���[�W�X�e�[�g�ɑJ�ڂ���B
			m_enemyState = enEnemyState_HitDamage2;
		}
	}
}

void Mediumboss::Attack()
{

	if (m_enemyattackmotion == 1)
	{
		//�U���X�e�[�g�łȂ��Ȃ珈�������Ȃ��B
		if (m_enemyState != enEnemyState_Attack1)
		{
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

const bool Mediumboss::SearchPlayer() const
{
	//�v���C���[��HP��0�̎���
	//���������Ȃ��B
	if (m_player->GetPlayerhp() == 0)
	{
		return false;
	}
	//�G���g���[�X�e�[�g��false�̎����������Ȃ��悤�ɂ���B
	if (m_entrystate == false)
	{
		//�G�l�~�[����v���C���[�Ɍ������x�N�g�������߂�B
		Vector3 diff = m_player->GetPosition() - m_position;

		//�v���C���[�ɂ�����x�߂�������B
		if (diff.LengthSq() <= 100.0 * 100.0f)
		{
			//�v���C���[�Ɍ������Ă���
			return true;
			/*
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
			*/
		}
		else
		{
			//�v���C���[���������Ȃ������B
			return false;
		}
	}
}

void Mediumboss::MakeAttackCollision()
{

	//�U�������蔻��p�̃R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//���W���G�l�~�[�̏����O�ɐݒ肷��B
	collisionPosition += m_forward * 50.0f;
	//����̃R���W�������쐬����B
	collisionObject->CreateSphere(collisionPosition,        //���W�B
		Quaternion::Identity,                               //��]�B
		2.0f                                               //���a�B
	);
	collisionObject->SetName("enemy_attack");

	//���̃{�[���̃��[���h�s����擾����B
	Matrix matrix = m_enemy.GetBone(m_RightHandId)->GetWorldMatrix();
	//�uLeftHand�v�{�[���̃��[���h�s����R���W�����ɓK�p����B
	collisionObject->SetWorldMatrix(matrix);
}

void Mediumboss::MakeAttackCollision2()
{

	//�U�������蔻��p�̃R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject2 = NewGO<CollisionObject>(0);
	Vector3 collisionPosition2 = m_position;
	//���W���G�l�~�[�̏����O�ɐݒ肷��B
	collisionPosition2 += m_forward * 50.0f;
	//����̃R���W�������쐬����B
	collisionObject2->CreateSphere(collisionPosition2,        //���W�B
		Quaternion::Identity,                               //��]�B
		3.0f                                               //���a�B
	);
	collisionObject2->SetName("enemy_attack2");

	//���̃{�[���̃��[���h�s����擾����B
	Matrix matrix2 = m_enemy.GetBone(m_LeftHandId)->GetWorldMatrix();
	//�uRightHand�v�{�[���̃��[���h�s����R���W�����ɓK�p����B
	collisionObject2->SetWorldMatrix(matrix2);
}

void Mediumboss::ProcessCommonStateTransition()
{
	//�e�^�C�}�[���������B
	//�ҋ@���ԂƒǐՎ��Ԃ𐧌����邽�߁B
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
	Vector3 diff = m_player->GetPosition() - m_position;

	if (m_hp == 0)
	{
		//�L�����R���̍��̂�j������B
		m_charaCon.RemoveRigidBoby();
		//�����ɂ���āA�_�E�����[�V���������肷��B
		int ram2 = rand() % 100;
		if (ram2 > 50)
		{
			m_enemyState = enEnemyState_Down1;
		}
		else
		{
			m_enemyState = enEnemyState_Down2;
		}
	}
	else
	{
		//�v���C���[����������B
		if (SearchPlayer() == true)
		{
			//�ʏ�U���ł��Ȃ������Ȃ�B
			if (IsCanAttack() == false)
			{
				//�x�N�g���𐳋K������B
				diff.Normalize();
				//�ړ����x��ݒ肷��B
				m_moveSpeed = diff * 50.0f;
				//y���W�ɂ͈ړ������Ȃ�
				m_moveSpeed.y = 0.0f;
				//�ǐՃX�e�[�g�ɑJ�ڂ���B
				//m_enemyState = enEnemyState_MagicAttack;
				m_enemyState = enEnemyState_Chase;
				return;
			}
			//�U���ł��鋗���Ȃ�B
			else
			{
				//�����ɂ���āA�U�������肷��B	
				int ram = rand() % 100;
					if (ram <= 50)
					{
						m_enemyState = enEnemyState_Attack1;
						m_enemyattackmotion = 1;
						m_isUnderAttack = false;
						return;
					}
					else
					{
						/*
						//�ҋ@�X�e�[�g�ɑJ�ڂ���B
						m_enemyState = enEnemyState_Idle;
						*/
						m_enemyState = enEnemyState_Attack2;
						m_enemyattackmotion = 0;
						m_isUnderAttack = false;
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
}

void Mediumboss::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//�ҋ@���Ԃ�������x�o�߂�����B
	if (m_idleTimer >= 0.9f)
	{
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();
	}

}

void Mediumboss::ProcessRunStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���B
	ProcessCommonStateTransition();
}

void Mediumboss::ProcessChaseStateTransition()
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

void Mediumboss::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_enemy.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}
void Mediumboss::ProcessHitDamageStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_enemy.IsPlayingAnimation() == false)
	{
		//hit�_���[�W�N�[���^�C����false�ɂ���B
		hitdamagecooltime = false;
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}
void Mediumboss::ProcessDownStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_enemy.IsPlayingAnimation() == false)
	{
		FindGO<Game>("game")->SetBosskill(true);
		DeleteGO(this);
	}
}
void Mediumboss::ManageState()
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
		//�G�l�~�[�X�e�[�g��enPlayerState_HitDamage1��������B
	case  enEnemyState_HitDamage1:
		//��_���[�W�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessHitDamageStateTransition();
		break;
		//�G�l�~�[�X�e�[�g��enPlayerState_HitDamage2��������B
	case  enEnemyState_HitDamage2:
		//��_���[�W�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessHitDamageStateTransition();
		break;
		//�G�l�~�[�X�e�[�g��enPlayerState_Down1��������B
	case  enEnemyState_Down1:
		//�_�E���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessDownStateTransition();
		break;
		//�G�l�~�[�X�e�[�g��enPlayerState_Down2��������B
	case  enEnemyState_Down2:
		//�_�E���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessDownStateTransition();
		break;
	}
}
void Mediumboss::PlayAnimation()
{
	m_enemy.SetAnimationSpeed(1.0f);
	m_enemy2.SetAnimationSpeed(1.0f);

	if (m_entrystate==true)
	{
		m_enemy.PlayAnimation(enAnimationClip_Roar, 0.1f);
	}
	else
	{
		switch (m_enemyState)
		{
			//�ҋ@�X�e�[�g�̎��B
		case enEnemyState_Idle:
			//�ҋ@�A�j���[�V�������Đ��B
			m_enemy.PlayAnimation(enAnimationClip_Idle, 0.5f);
			m_enemy2.PlayAnimation(enAnimationClip_Idle, 0.5f);
			break;
			//�ǐՃX�e�[�g�̎��B
		case enEnemyState_Chase:
			//����A�j���[�V�������Đ��B
			m_enemy.PlayAnimation(enAnimationClip_Run, 0.1f);
			m_enemy2.PlayAnimation(enAnimationClip_Run, 0.1f);
			break;
			//�G�l�~�[�X�e�[�g��enPlayerState_Attack1��������B
		case enEnemyState_Attack1:
			//attack���[�V�������Đ�
			m_enemy.PlayAnimation(enAnimationClip_Attack1, 0.1f);
			m_enemy2.PlayAnimation(enAnimationClip_Attack1, 0.1f);
			break;
			//�G�l�~�[�X�e�[�g��enPlayerState_Attack2��������B
		case enEnemyState_Attack2:
			//attack���[�V�������Đ�
			m_enemy.PlayAnimation(enAnimationClip_Attack2, 0.1f);
			m_enemy2.PlayAnimation(enAnimationClip_Attack2, 0.1f);
			break;
			//�G�l�~�[�X�e�[�g��enPlayerState_HitDamage1��������B
		case  enEnemyState_HitDamage1:
			m_enemy.SetAnimationSpeed(1.3f);
			//HitDamage���[�V�������Đ�
			m_enemy.PlayAnimation(enAnimationClip_HitDamage1, 0.1f);
			m_enemy2.PlayAnimation(enAnimationClip_HitDamage1, 0.1f);
			break;
			//�G�l�~�[�X�e�[�g��enPlayerState_HitDamage2��������B
		case  enEnemyState_HitDamage2:
			m_enemy.SetAnimationSpeed(1.3f);
			//HitDamage���[�V�������Đ�
			m_enemy.PlayAnimation(enAnimationClip_HitDamage2, 0.1f);
			m_enemy2.PlayAnimation(enAnimationClip_HitDamage2, 0.1f);
			break;
			//�G�l�~�[�X�e�[�g��enPlayerState_Down1��������B
		case  enEnemyState_Down1:
			//HitDamage���[�V�������Đ�
			m_enemy.PlayAnimation(enAnimationClip_Down1, 0.1f);
			m_enemy2.PlayAnimation(enAnimationClip_Down1, 0.1f);
			break;
			//�G�l�~�[�X�e�[�g��enPlayerState_Down2��������B
		case  enEnemyState_Down2:
			//HitDamage���[�V�������Đ�
			m_enemy.PlayAnimation(enAnimationClip_Down2, 0.1f);
			m_enemy2.PlayAnimation(enAnimationClip_Down2, 0.1f);
			break;
		default:
			break;
		}
	}
}

const bool Mediumboss::IsCanAttack() const
{
	//�v���C���[��HP��0�Ȃ�
	if (m_player->GetPlayerhp() == 0)
	{
		//�U���ł��Ȃ�
		return false;
	}
	//�G�l�~�[����v���C���[�Ɍ������x�N�g�������߂�
	Vector3 diff = m_player->GetPosition() - m_position;
	//�G�l�~�[����v���C���[�Ɍ������x�N�g�������߂�B
	Vector3 diff2 = m_player->GetPosition() - m_position;
	//�G�l�~�[�ƃv���C���[�̋������߂�������B
	if (diff.LengthSq() <= 8.0f * 8.0f)
	{

		//�G�l�~�[����v���C���[�Ɍ������x�N�g���𐳋K��(�傫����1)����B
		diff2.Normalize();
		//�G�l�~�[�̐��ʂ̃x�N�g���ƁA�G�l�~�[����v���C���[�Ɍ������x�N�g���́B
		//����(cos��)�����߂�B
		float cos = m_forward.Dot(diff2);
		//����(cos��)����p�x(��)�����߂�B
		float angle = acosf(cos);
		//�p�x(��)��60����菬������΁B
		if (angle <= (Math::PI / 180.0f) * 60.0f)
		{
			//�U���ł���I
			return true;
		}
	}
	//�U���ł��Ȃ��B
	return false;
}

void Mediumboss::Render(RenderContext& rc)
{
	if (FindGO<Game>("game")->GetWorldState() == 0)
	{
		m_enemy.Draw(rc);
	}
	if (FindGO<Game>("game")->GetWorldState() == 1)
	{
		m_enemy2.Draw(rc);
	}
	//m_fontRender.Draw(rc);
}