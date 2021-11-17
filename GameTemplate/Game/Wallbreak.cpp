#include "stdafx.h"
#include "Wallbreak.h"
#include "Player.h"

//CollisionObjectを使用するために、ファイルをインクルードする。
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

	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	collisionPosition.z = 525.0f;
	Vector3 collisionScale = Vector3(1400.0f, 600.0f, 50.0f);
	//ボックス状のコリジョンを作成する。
	m_collisionObject->CreateBox(collisionPosition,       //座標。
		Quaternion::Identity,                             //回転。
		collisionScale);                                  //大きさ。

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

void Wallbreak::GetCreate()
{
	//createstateがfalseで5.0fを越えていたら再生成する。
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
		//PhysicsStaticObjectを初期化。
		m_physicsStaticObject.CreateFromModel(
			m_wall.GetModel(),
			m_wall.GetModel().GetWorldMatrix());
	}
}

void  Wallbreak::Break()
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
}

void  Wallbreak::Update()
{
	Break();
	GetCreate();
	//モデルの更新処理。
	m_wall.Update();
}

void  Wallbreak::Render(RenderContext& rc)
{
	if (createstate == true)
	{
		m_wall.Draw(rc);    //モデルを描画する。
	}
}