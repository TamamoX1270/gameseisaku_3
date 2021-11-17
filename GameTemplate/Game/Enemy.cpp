#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "Game.h"

//CollisionObjectを使用するために、ファイルをインクルードする。
#include "collision/CollisionObject.h"

bool Enemy::Start()
{
	//アニメーションを読み込む
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/mutant/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/mutant/MutantWalking.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	/*
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/mutant/Run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack1].Load("Assets/animData/mutant/attack.tka");
	m_animationClips[enAnimationClip_Attack1].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack2].Load("Assets/animData/mutant/attack2.tka");
	m_animationClips[enAnimationClip_Attack2].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack3].Load("Assets/animData/mutant/attack3.tka");
	m_animationClips[enAnimationClip_Attack3].SetLoopFlag(false);
	*/
	/*
	m_animationClips[enAnimationClip_MagicAttack].Load("Assets/animData/mutant/magicattack.tka");
	m_animationClips[enAnimationClip_MagicAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_HitDamage].Load("Assets/animData/mutant/receivedamage.tka");
	m_animationClips[enAnimationClip_HitDamage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/mutant/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Winner].Load("Assets/animData/mutant/winner.tka");
	m_animationClips[enAnimationClip_Winner].SetLoopFlag(false);
	*/

	//敵のモデルを読み込む
	m_enemy.Init("Assets/modelData/mutant.tkm");
	//座標を設定する。
	Vector3 position = m_position;
	position.z = 200.0f;
	m_enemy.SetPosition(position);
	m_enemy.SetScale({ 10.0f,10.0f,10.0f });
	m_enemy.SetScale(Vector3::One * 2.5f);
	//回転を設定する。
	m_enemy.SetRotation(m_rotation);
	m_enemy.Update();

	//キャラクターコントローラーを初期化。
	m_charaCon.Init(
		20.0f,			//半径。
		100.0f,			//高さ。
		position		//座標。
	);

	m_player = FindGO<Player>("player");

	//アニメーションイベント用の関数を設定する。
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

	//モデルの更新処理。
	m_enemy.Update();
}

void Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	
	//キーの名前が「attack_start」の時。
	if (wcscmp(eventName, L"attack_start") == 0) {
		//攻撃中判定をtrueにする。
		m_isUnderAttack = true;
		//効果音を再生する。
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(3);
		se->Play(false);
		se->SetVolume(0.8f);

		//攻撃エフェクトを発生させる。
		EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
		effectEmitter->Init(1);
		//大きさを設定する。
		effectEmitter->SetScale(Vector3::One * 12.0f);
		Vector3 effectPosition = m_position;
		//座標を少し上にする。
		effectPosition.y += 70.0f;
		//座標を設定する。
		effectEmitter->SetPosition(effectPosition);
		Quaternion rotation;
		rotation = m_rotation;
		rotation.AddRotationDegY(360.0f);
		//回転を設定する。
		effectEmitter->SetRotation(rotation);
		//エフェクトを再生する。
		effectEmitter->Play();
	}
	//キーの名前が「attack_end」の時。
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//攻撃中判定をfalseにする。
		m_isUnderAttack = false;
	}
	
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

void Enemy::Chase()
{
	//追跡ステートでないなら、追跡処理はしない。
	if (m_enemyState != enEnemyState_Chase)
	{
		return;
	}

	//エネミーを移動させる。
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//地面についた。
		//重力を0にする。
		m_moveSpeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//座標を設定する。
	m_enemy.SetPosition(modelPosition);
}

void Enemy::Collision()
{
	//被ダメージ、あるいはダウンステートの時は。
	//当たり判定処理はしない。
	if (m_enemyState == enEnemyState_ReceiveDamage ||
		m_enemyState == enEnemyState_Down)
	{
		return;
	}

	//Playerが作成した、攻撃のヒット判定用のコリジョンの配列を取得。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//for文で配列を回す。
	for (auto collision : collisions)
	{
		//攻撃のコリジョンと敵のキャラコンが。
		//衝突していたら。
		if (collision->IsHit(m_charaCon) == true)
		{
			DeleteGO(this);
		}
	}
	//Playerが作成した、攻撃のヒット判定用のコリジョン2の配列を取得。
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_attack2");
	//for文で配列を回す。
	for (auto collision : collisions2)
	{
		//攻撃のコリジョンと敵のコリジョンが。
		//衝突していたら。
		if (collision->IsHit(m_charaCon) == true)
		{
			DeleteGO(this);
		}
	}

}

void Enemy::Attack()
{
	//攻撃ステートでないなら処理をしない。
	if (m_enemyState != enEnemyState_Attack)
	{
		return;
	}

	//攻撃中であれば。
	if (m_isUnderAttack == true)
	{
		//攻撃用のコリジョンを作成する。
		MakeAttackCollision();
	}
}

const bool Enemy::SearchPlayer() const
{
	//エネミーからプレイヤーに向かうベクトルを求める。
	Vector3 diff = m_player->GetPosition() - m_position;

	//エネミーからプレイヤーに向かうベクトルを正規化(大きさを1)する。
	diff.Normalize();
	//エネミーの正面のベクトルと、エネミーからプレイヤーに向かうベクトルの。
	//内積(cosθ)を求める。
	float cos = m_forward.Dot(diff);
	//内積(cosθ)から角度(θ)を求める。
	float angle = acosf(cos);
	//角度(θ)が120°(視野角)より小さければ。
	if (angle <= (Math::PI / 180.0f) * 120.0f)
	{
		//プレイヤーを見つけた！
		return true;
	}

	//プレイヤーにある程度近かったら。
	if (diff.LengthSq() <= 700.0 * 700.0f)
	{
		//エネミーからプレイヤーに向かうベクトルを正規化(大きさを1)する。
		diff.Normalize();
		//エネミーの正面のベクトルと、エネミーからプレイヤーに向かうベクトルの。
		//内積(cosθ)を求める。
		float cos = m_forward.Dot(diff);
		//内積(cosθ)から角度(θ)を求める。
		float angle = acosf(cos);
		//角度(θ)が120°(視野角)より小さければ。
		if (angle <= (Math::PI / 180.0f) * 120.0f)
		{
			//プレイヤーを見つけた！
			return true;
		}
	}
	//見つけられなかったら実行しない
	return false;
}

void Enemy::MakeAttackCollision()
{
	/*
	//攻撃当たり判定用のコリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);
	//剣のボーンのワールド行列を取得する。
	Matrix matrix = m_enemy.GetBone(m_swordBoneId)->GetWorldMatrix();
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(100.0f, 10.0f, 10.0f));
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");
	*/
}

void Enemy::ProcessCommonStateTransition()
{
	
	//各タイマーを初期化。
	//待機時間と追跡時間を制限するため。
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//エネミーからプレイヤーに向かうベクトルを計算する。
	Vector3 diff = m_player->GetPosition() - m_position;

	//プレイヤーを見つけたら。
	if (SearchPlayer() == true)
	{
		return;
		
		//通常攻撃できない距離なら。
		if (IsCanAttack() == false)
		{
			//ベクトルを正規化する。
			diff.Normalize();
			//移動速度を設定する。
			m_moveSpeed = diff * 250.0f;
			//乱数によって、追跡させるか魔法攻撃をするか決定する。	
			int ram = rand() % 100;
			//乱数が40以上なら。
			if (ram > 40)
			{
				//追跡ステートに遷移する。
				m_enemyState = enEnemyState_Chase;
				return;
			}
			//乱数が40未満なら。
			else {
				//魔法攻撃ステートに遷移する。
				m_enemyState = enEnemyState_MagicAttack;
				return;
			}
		}
		//攻撃できる距離なら。
		else
		{
			//乱数によって、攻撃するか待機させるかを決定する。	
			int ram = rand() % 100;
			if (ram > 30)
			{
				//攻撃ステートに遷移する。
				m_enemyState = enEnemyState_Attack;
				m_isUnderAttack = false;
				return;
			}
			else
			{
				//待機ステートに遷移する。
				m_enemyState = enEnemyState_Idle;
				return;
			}
		}
		
	}
	//プレイヤーを見つけられなければ。
	else
	{
		//待機ステートに遷移する。
		m_enemyState = enEnemyState_Idle;
		return;

	}
	
}

void Enemy::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//待機時間がある程度経過したら。
	if (m_idleTimer >= 0.9f)
	{
		//他のステートへ遷移する。
		ProcessCommonStateTransition();
	}

}

void Enemy::ProcessWalkStateTransition()
{
	//他のステートに遷移する。
	ProcessCommonStateTransition();
}

void Enemy::ProcessRunStateTransition()
{
	//他のステートに遷移する。
	ProcessCommonStateTransition();
}

void Enemy::ProcessChaseStateTransition()
{
	//攻撃できる距離なら。
	if (IsCanAttack() == true)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
		return;
	}
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//追跡時間がある程度経過したら。
	if (m_chaseTimer >= 0.8f)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}

void Enemy::ProcessAttackStateTransition()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_enemy.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}
void Enemy::ManageState()
{
	switch (m_enemyState)
	{
		//待機ステートの時。
	case enEnemyState_Idle:
		//待機ステートのステート遷移処理。
		ProcessIdleStateTransition();
		break;
		//追跡ステートの時。
	case enEnemyState_Chase:
		//追跡ステートのステート遷移処理。
		ProcessChaseStateTransition();
		break;
		//攻撃ステートの時。
	case enEnemyState_Attack:
		//攻撃ステートのステート遷移処理。
		ProcessAttackStateTransition();
		break;
	}
}
void Enemy::PlayAnimation()
{
	m_enemy.SetAnimationSpeed(1.0f);
	switch (m_enemyState)
	{
		//待機ステートの時。
	case enEnemyState_Idle:
		//待機アニメーションを再生。
		m_enemy.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//追跡ステートの時。
	case enEnemyState_Chase:
		//歩きアニメーションを再生。
		m_enemy.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
		//攻撃ステートの時。
	case enEnemyState_Attack:
		//攻撃アニメーションを再生。
		m_enemy.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
		//ダウンステートの時。
	case enEnemyState_Down:
		//ダウンアニメーションを再生。
		m_enemy.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}

const bool Enemy::IsCanAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//エネミーとプレイヤーの距離が近かったら。
	if (diff.LengthSq() <= 100.0f * 100.0f)
	{
		//攻撃できる！
		return true;
	}
	//攻撃できない。
	return false;
}

void Enemy::Render(RenderContext& rc)
{
	m_enemy.Draw(rc);
}