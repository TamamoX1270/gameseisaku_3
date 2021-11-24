#include "stdafx.h"
#include "Breakrock.h"
#include "Player.h"
#include "Enemy.h"

//CollisionObjectを使用するために、ファイルをインクルードする。
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

	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	collisionPosition.z += 5.0f;
	collisionPosition.y += 15.0f;
	//Vector3 collisionScale = Vector3(30.0f, 30.0f, 30.0f);
	//球状のコリジョンを作成する。
	m_collisionObject->CreateSphere(collisionPosition,       //座標。
		Quaternion::Identity,                             //回転。
		20.0f);                                  //大きさ。

		/*
		Quaternion collisionRotation = m_rotation;
		Vector3 meshposition = m_position;
		//メッシュのコリジョンを作成する。
		m_collisionObject->CreateMesh(meshposition,
		collisionRotation,
		m_wall.GetModel(), m_wall.GetModel().GetWorldMatrix());
		*/

		//名前を付ける。
	m_collisionObject->SetName("player_touch");
	//自動で削除を無効にする(DeleteGOで削除する必要がある)。
	m_collisionObject->SetIsEnableAutoDelete(false);

	createstate = true;
	Create();

	return true;
}

void Breakrock::GetCreate()
{

	//コリジョンオブジェクトとプレイヤーのキャラクターコントローラーが。
	//衝突していたら再生成しない。
	if (m_collisionObject->IsHit(FindGO<Player>("player")->GetCharacterController()) == true)
	{
		return;
	}

	//createstateがfalseで5.0fを越えていたら再生成する。
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
		//PhysicsStaticObjectを初期化。
		m_physicsStaticObject.CreateFromModel(
			m_rock.GetModel(),
			m_rock.GetModel().GetWorldMatrix());
	}
}

void  Breakrock::Break()
{
	//Playerが作成した、攻撃のヒット判定用のコリジョンの配列を取得。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//for文で配列を回す。
	for (auto collision : collisions)
	{
		//Playerの攻撃のコリジョンと壁のコリジョンが。
		//衝突していたら。
		if (collision->IsHit(m_collisionObject) == true)
		{
			//アニメーションの再生
			//m_rock.PlayAnimation(enAnimationClip_Break);
			//PhysicsStaticObjectをリリース
			m_physicsStaticObject.Release();
			createstate = false;
		}
	}
	//Playerが作成した、攻撃のヒット判定用のコリジョン2の配列を取得。
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_attack2");
	//for文で配列を回す。
	for (auto collision : collisions2)
	{
		//攻撃のコリジョンと壁のコリジョンが。
		//衝突していたら。
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
	//モデルの更新処理。
	m_rock.Update();
}

void  Breakrock::Render(RenderContext& rc)
{
	if (createstate == true)
	{
		m_rock.Draw(rc);    //モデルを描画する。
	}
}