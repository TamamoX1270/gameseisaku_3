#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "Game.h"

//CollisionObjectを使用するために、ファイルをインクルードする。
#include "collision/CollisionObject.h"

bool Enemy::Start()
{
	m_position.x = -500.0f;
	m_position.y = -50.0f;
	m_enemy.SetPosition(m_position);
	m_enemy.Update();

	//敵のモデルを読み込む
	m_enemy.Init("Assets/modelData/Creaturecat.tkm");
	m_enemy.SetScale({ 10.0f,10.0f,10.0f });
	m_enemy.SetScale(Vector3::One * 3.0f);
	m_enemy.SetPosition(m_position);
	m_enemy.Update();

	//コリジョンオブジェクトを作成する。
	m_collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//座標を敵の位置に設定する。
	collisionPosition = m_position;
	//球状のコリジョンを作成する。
	m_collisionObject->CreateSphere(collisionPosition,       //座標。
		Quaternion::Identity,                             //回転。
		100.0f);                                          //球の大きさ(半径)。
	//名前を付ける。
	m_collisionObject->SetName("enemy_hit");
	//自動で削除を無効にする(DeleteGOで削除する必要がある)。
	m_collisionObject->SetIsEnableAutoDelete(false);

	//キャラコンを初期化する。
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

	//モデルの更新処理。
	m_enemy.Update();
}

void Enemy::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}
	//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数。
	//m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる。
	//atan2を使用して、角度を求めている。
	//これが回転角度になる。
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atanが返してくる角度はラジアン単位なので
	//SetRotationDegではなくSetRotationを使用する。
	m_rotation.SetRotationY(-angle);

	//回転を設定する。
	m_enemy.SetRotation(m_rotation);

	//プレイヤーの前ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Enemy::Move()
{
	m_playerposition = FindGO<Player>("player")->m_position;
	//エネミーを移動させる。
	m_position = m_characterController.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_characterController.IsOnGround()) {
		//地面についた。
		//重力を0にする。
		m_moveSpeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//ちょっとだけモデルの座標を挙げる。
	modelPosition.y += 2.5f;
	//座標を設定する。
	m_enemy.SetPosition(modelPosition);

	//エネミーからプレイヤーに向かうベクトルを計算する。
	Vector3 diff = m_playerposition - m_position;

	//ベクトルを正規化する。
	diff.Normalize();
	//移動速度を設定する。
	m_moveSpeed = diff * 100.0f;

	//Playerが作成した、攻撃のヒット判定用のコリジョンの配列を取得。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//for文で配列を回す。
	for (auto collision : collisions)
	{
		//弾のコリジョンと敵のコリジョンが。
		//衝突していたら。
		if (collision->IsHit(m_collisionObject) == true)
		{
			DeleteGO(this);
		}
	}
	//Playerが作成した、攻撃のヒット判定用のコリジョン2の配列を取得。
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_attack2");
	//for文で配列を回す。
	for (auto collision : collisions2)
	{
		//弾のコリジョンと敵のコリジョンが。
		//衝突していたら。
		if (collision->IsHit(m_collisionObject) == true)
		{
			DeleteGO(this);
		}
	}

	//setpositionで動いた敵の当たり判定の反映
	m_collisionObject->SetPosition(m_position);
	//動いた敵の描画
	m_enemy.SetPosition(m_position);

}

void Enemy::Render(RenderContext& rc)
{
	m_enemy.Draw(rc);
}