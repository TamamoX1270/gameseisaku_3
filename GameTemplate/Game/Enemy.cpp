#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "Game.h"

//CollisionObject���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"

bool Enemy::Start()
{
	m_position.x = -500.0f;
	m_position.y = -50.0f;
	m_enemy.SetPosition(m_position);
	m_enemy.Update();

	//�G�̃��f����ǂݍ���
	m_enemy.Init("Assets/modelData/Creaturecat.tkm");
	m_enemy.SetScale({ 10.0f,10.0f,10.0f });
	m_enemy.SetScale(Vector3::One * 3.0f);
	m_enemy.SetPosition(m_position);
	m_enemy.Update();

	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//���W��G�̈ʒu�ɐݒ肷��B
	collisionPosition = m_position;
	//����̃R���W�������쐬����B
	m_collisionObject->CreateSphere(collisionPosition,       //���W�B
		Quaternion::Identity,                             //��]�B
		100.0f);                                          //���̑傫��(���a)�B
	//���O��t����B
	m_collisionObject->SetName("enemy_hit");
	//�����ō폜�𖳌��ɂ���(DeleteGO�ō폜����K�v������)�B
	m_collisionObject->SetIsEnableAutoDelete(false);

	//�L�����R��������������B
	m_characterController.Init(25.0f, 25.0f, m_position);

	return true;
};

Enemy::Enemy()
{

}

Enemy::~Enemy()
{
	DeleteGO(m_collisionObject);
}

void Enemy::Update()
{
	Move();
	Rotation();

	//���f���̍X�V�����B
	m_enemy.Update();
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

void Enemy::Move()
{
	m_playerposition = FindGO<Player>("player")->m_position;
	//�G�l�~�[���ړ�������B
	m_position = m_characterController.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_characterController.IsOnGround()) {
		//�n�ʂɂ����B
		//�d�͂�0�ɂ���B
		m_moveSpeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//������Ƃ������f���̍��W��������B
	modelPosition.y += 2.5f;
	//���W��ݒ肷��B
	m_enemy.SetPosition(modelPosition);

	//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
	Vector3 diff = m_playerposition - m_position;

	//�x�N�g���𐳋K������B
	diff.Normalize();
	//�ړ����x��ݒ肷��B
	m_moveSpeed = diff * 100.0f;

	//Player���쐬�����A�U���̃q�b�g����p�̃R���W�����̔z����擾�B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//for���Ŕz����񂷁B
	for (auto collision : collisions)
	{
		//�e�̃R���W�����ƓG�̃R���W�������B
		//�Փ˂��Ă�����B
		if (collision->IsHit(m_collisionObject) == true)
		{
			DeleteGO(this);
		}
	}
	//Player���쐬�����A�U���̃q�b�g����p�̃R���W����2�̔z����擾�B
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_attack2");
	//for���Ŕz����񂷁B
	for (auto collision : collisions2)
	{
		//�e�̃R���W�����ƓG�̃R���W�������B
		//�Փ˂��Ă�����B
		if (collision->IsHit(m_collisionObject) == true)
		{
			DeleteGO(this);
		}
	}

	//setposition�œ������G�̓����蔻��̔��f
	m_collisionObject->SetPosition(m_position);
	//�������G�̕`��
	m_enemy.SetPosition(m_position);

}

void Enemy::Render(RenderContext& rc)
{
	m_enemy.Draw(rc);
}