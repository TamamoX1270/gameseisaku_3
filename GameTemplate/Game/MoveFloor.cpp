#include "stdafx.h"
#include "MoveFloor.h"
#include "MoveFloorWall.h"
#include "stdafx.h"
#include "Player.h"
#include "Game.h"

namespace
{
	float SPEED = 20.0f;
	Vector3 COLLISION_HEIGHT = Vector3(0.0f, 1.5f, 0.0f);
	Vector3	COLLISION_SIZE = Vector3(108.0f, 5.0f, 90.0f);

}

MoveFloor::MoveFloor()
{

}

MoveFloor::~MoveFloor()
{
	DeleteGO(m_collisionObject);
}

bool MoveFloor::Start()
{
	m_modelRender.Init("Assets/modelData/stage/movefloor.tkm");
	m_modelRender.Update();
	//PhysicsStaticObject���������B
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	m_collisionObject = NewGO<CollisionObject>(0, "collisionObject");
	//�R���W�����I�u�W�F�N�g���B
	//�������̏�ɐݒu����(�L�����N�^�[����ɏ�����甽������悤�ɂ���������)�B
	m_collisionObject->CreateBox(
		m_position + COLLISION_HEIGHT,
		Quaternion::Identity,
		COLLISION_SIZE
	);
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_firstPosition = m_position;
	return true;
}

void MoveFloor::Update()
{

	if (m_MoveFloorState == true)
	{
		m_timer += g_gameTime->GetFrameDeltaTime();
	}

	Move();

	m_modelRender.Update();
	m_physicsStaticObject.SetPosition(m_position);
	m_collisionObject->SetPosition(m_position + COLLISION_HEIGHT);

	int x = m_position.x;
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"x:%d", x);

	//�\������e�L�X�g��ݒ�B
	m_fontRender.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_fontRender.SetPosition(Vector3(300.0f, 200.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_fontRender.SetScale(1.0f);

	int y = m_position.y;
	wchar_t wcsbuf2[256];
	swprintf_s(wcsbuf2, 256, L"y:%d", y);

	//�\������e�L�X�g��ݒ�B
	m_fontRender2.SetText(wcsbuf2);
	//�t�H���g�̈ʒu��ݒ�B
	m_fontRender2.SetPosition(Vector3(300.0f, 150.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_fontRender2.SetScale(1.0f);

	int z = m_position.z;
	wchar_t wcsbuf3[256];
	swprintf_s(wcsbuf3, 256, L"z:%d", z);

	//�\������e�L�X�g��ݒ�B
	m_fontRender3.SetText(wcsbuf3);
	//�t�H���g�̈ʒu��ݒ�B
	m_fontRender3.SetPosition(Vector3(300.0f, 100.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_fontRender3.SetScale(1.0f);

}

void MoveFloor::Move()
{
	Vector3 moveSpeed = Vector3::Zero;

	if (m_timer >= 1.0f)
	{
		FindGO<Game>("game")->SetMoveFloorState(true);
		if (m_MoveFloorState == true)
		{

			if (m_MovingFloorState == enMovingFloor_Y)
			{
				moveSpeed.y = SPEED;
			}
			else if (m_MovingFloorState == enMovingFloor_Z)
			{
				moveSpeed.z = SPEED;
			}

			m_position += moveSpeed * g_gameTime->GetFrameDeltaTime();

			if (m_MovingFloorState == enMovingFloor_Y && m_position.y >= 300.0f)
			{
				m_MovingFloorState = enMovingFloor_Z;
			}
			else if (m_MovingFloorState == enMovingFloor_Z && m_position.z >= 1050.0f)
			{
				m_MovingFloorState = enMovingFloor_None;
				m_MoveFloorState = false;
			}
		}
	}
	m_modelRender.SetPosition(m_position);
	//�R���W�����I�u�W�F�N�g�ƃv���C���[�̃L�����N�^�[�R���g���[���[���B
    //�Փ˂�����B(�L�����N�^�[���������̏�ɏ������)�B
	if (m_collisionObject->IsHit(FindGO<Player>("player")->GetCharacterController()) == true)
	{
		//�������̈ړ����x���L�����N�^�[�̈ړ����x�ɉ��Z�B
		FindGO<Player>("player")->AddMoveSpeed(moveSpeed);
		if (m_position.z <= 1050.0f)
		{
			m_MoveFloorState = true;
		}
	}
}

void MoveFloor::Render(RenderContext& rc)
{
	//�`�悷��B
	m_fontRender.Draw(rc);
	m_fontRender2.Draw(rc);
	m_fontRender3.Draw(rc);

	m_modelRender.Draw(rc);
}