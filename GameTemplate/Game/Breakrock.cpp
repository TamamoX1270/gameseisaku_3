#include "stdafx.h"
#include "Breakrock.h"
#include "Player.h"
#include "Enemy.h"

//CollisionObject���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"

Breakrock::Breakrock()
{

}

Breakrock::~Breakrock()
{
	DeleteGO(m_collisionObject);
}
bool   Breakrock::Start()
{
	m_rock.Init("Assets/modelData/breakrock.tkm");
	m_rock.SetPosition(m_position);
	m_rock.SetRotation(m_rotation);
	m_rock.SetScale(m_scale);
	m_rock.Update();

	m_animationClips[enAnimationClip_Break].Load("Assets/animData/breakrockeffect.tka");
	m_animationClips[enAnimationClip_Break].SetLoopFlag(true);

	//�R���W�����I�u�W�F�N�g���쐬����B
	m_collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	collisionPosition.z += 5.0f;
	collisionPosition.y += 15.0f;
	//Vector3 collisionScale = Vector3(30.0f, 30.0f, 30.0f);
	//����̃R���W�������쐬����B
	m_collisionObject->CreateSphere(collisionPosition,       //���W�B
		Quaternion::Identity,                             //��]�B
		20.0f);                                  //�傫���B

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

void Breakrock::GetCreate()
{

	//�R���W�����I�u�W�F�N�g�ƃv���C���[�̃L�����N�^�[�R���g���[���[���B
	//�Փ˂��Ă�����Đ������Ȃ��B
	if (m_collisionObject->IsHit(FindGO<Player>("player")->GetCharacterController()) == true)
	{
		return;
	}

	//createstate��false��5.0f���z���Ă�����Đ�������B
	if (createstate == false && m_timer >= 5.0f)
	{
		m_timer = 0.0f;
		createstate = true;
		Create();
	}
}

void Breakrock::Create()
{
	if (createstate == true)
	{
		//PhysicsStaticObject���������B
		m_physicsStaticObject.CreateFromModel(
			m_rock.GetModel(),
			m_rock.GetModel().GetWorldMatrix());
	}
}

void  Breakrock::Break()
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
			//�A�j���[�V�����̍Đ�
			//m_rock.PlayAnimation(enAnimationClip_Break);
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

	if (createstate == false)
	{
		m_timer += g_gameTime->GetFrameDeltaTime();
	}
}

void  Breakrock::Update()
{
	Break();
	GetCreate();
	//���f���̍X�V�����B
	m_rock.Update();
}

void  Breakrock::Render(RenderContext& rc)
{
	if (createstate == true)
	{
		m_rock.Draw(rc);    //���f����`�悷��B
	}
}