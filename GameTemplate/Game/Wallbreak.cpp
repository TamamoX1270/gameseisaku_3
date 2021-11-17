#include "stdafx.h"
#include "Wallbreak.h"
#include "Player.h"

//CollisionObject���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"

Wallbreak::Wallbreak()
{

}

Wallbreak::~Wallbreak()
{
	DeleteGO(m_collisionObject);
}
bool  Wallbreak::Start()
{
	m_position = Vector3(0.0f, 0.0f, 500.0f);
	m_scale = Vector3(Vector3::One * 20.0f);
	m_wall.Init("Assets/modelData/stage/woodwall2.tkm");
	m_wall.SetPosition(m_position);
	m_wall.SetRotation(m_rotation);
	m_wall.SetScale(m_scale);
	m_wall.Update();

	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	collisionPosition.z = 525.0f;
	Vector3 collisionScale = Vector3(1400.0f, 600.0f, 50.0f);
	//�{�b�N�X��̃R���W�������쐬����B
	m_collisionObject->CreateBox(collisionPosition,       //���W�B
		Quaternion::Identity,                             //��]�B
		collisionScale);                                  //�傫���B

		/*
		Quaternion collisionRotation = m_rotation;
		Vector3 meshposition = m_position;
		//���b�V���̃R���W�������쐬����B
		m_collisionObject->CreateMesh(meshposition,
		collisionRotation,
		m_wall.GetModel(), m_wall.GetModel().GetWorldMatrix());
		*/

	//���O��t����B
	m_collisionObject->SetName("player_touch");
	//�����ō폜�𖳌��ɂ���(DeleteGO�ō폜����K�v������)�B
	m_collisionObject->SetIsEnableAutoDelete(false);

	createstate = true;
	Create();

	return true;
}

void Wallbreak::GetCreate()
{
	//createstate��false��5.0f���z���Ă�����Đ�������B
	if (createstate == false&&m_timer>=5.0f)
	{
		m_timer = 0.0f;
		createstate = true;
		Create();
	}
	if (createstate == false)
	{
		m_timer += g_gameTime->GetFrameDeltaTime();
	}
}

void Wallbreak::Create()
{
	if (createstate == true)
	{
		//PhysicsStaticObject���������B
		m_physicsStaticObject.CreateFromModel(
			m_wall.GetModel(),
			m_wall.GetModel().GetWorldMatrix());
	}
}

void  Wallbreak::Break()
{
	
	//Player���쐬�����A�U���̃q�b�g����p�̃R���W�����̔z����擾�B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//for���Ŕz����񂷁B
	for (auto collision : collisions)
	{
		//Player�̍U���̃R���W�����ƕǂ̃R���W�������B
		//�Փ˂��Ă�����B
		if (collision->IsHit(m_collisionObject) == true)
		{
			//PhysicsStaticObject�������[�X
			m_physicsStaticObject.Release();
			createstate = false;
		}
	}
	//Player���쐬�����A�U���̃q�b�g����p�̃R���W����2�̔z����擾�B
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_attack2");
	//for���Ŕz����񂷁B
	for (auto collision : collisions2)
	{
		//�U���̃R���W�����ƕǂ̃R���W�������B
		//�Փ˂��Ă�����B
		if (collision->IsHit(m_collisionObject) == true)
		{
			m_physicsStaticObject.Release();
			createstate = false;
		}
	}
}

void  Wallbreak::Update()
{
	Break();
	GetCreate();
	//���f���̍X�V�����B
	m_wall.Update();
}

void  Wallbreak::Render(RenderContext& rc)
{
	if (createstate == true)
	{
		m_wall.Draw(rc);    //���f����`�悷��B
	}
}