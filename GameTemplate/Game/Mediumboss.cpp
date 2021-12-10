#include "stdafx.h"
#include "Mediumboss.h"
#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "Game.h"

#include <time.h>
#include <stdlib.h>

//CollisionObjectを使用するために、ファイルをインクルードする。
#include "collision/CollisionObject.h"
//EffectEmitterを使用する時はファイルをインクルードする必要がある。
#include "graphics/effect/EffectEmitter.h"

bool Mediumboss::Start()
{
	//アニメーションを読み込む
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

	//エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/blood.efk");

	//敵のモデルを読み込む
	m_enemy.Init("Assets/modelData/maw_j_laygo.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	m_enemy2.Init("Assets/modelData/maw_j_laygo.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	//座標を設定する。
	Vector3 position = m_position;
	Vector3 efpos = m_position;
	m_enemy.SetPosition(position);
	m_enemy.SetScale(m_scale);
	//回転を設定する。
	m_enemy.SetRotation(m_rotation);
	m_enemy.Update();
	m_enemy2.SetPosition(position);
	m_enemy2.SetScale(m_scale);
	//回転を設定する。
	m_enemy2.SetRotation(m_rotation);
	m_enemy2.Update();
	//キャラクターコントローラーを初期化。
	m_charaCon.Init(
		2.5f,			//半径。
		8.0f,			//高さ。
		position		//座標。
	);

	m_player = FindGO<Player>("player");

	//乱数を初期化。
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	//「LeftHand」ボーンのID(番号)を取得する。
	m_LeftHandId = m_enemy.FindBoneID(L"LeftHand");
	//「RightHand」ボーンのID(番号)を取得する。
	m_RightHandId = m_enemy.FindBoneID(L"RightHand");

	//アニメーションイベント用の関数を設定する。
	m_enemy.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});

	m_enemy.PlayAnimation(enAnimationClip_Roar, 0.1f);

	//NewGO時に再生する。
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

	//表示するテキストを設定。
	m_fontRender.SetText(wcsbuf);
	//フォントの位置を設定。
	m_fontRender.SetPosition(Vector3(-30.0f, 20.0f, 0.0f));
	//フォントの大きさを設定。
	m_fontRender.SetScale(1.0f);

	Chase();
	Attack();
	Rotation();
	Collision();
	PlayAnimation();
	ManageState();

	//モデルの更新処理。
	m_enemy.Update();
	m_enemy2.Update();
}

void Mediumboss::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//キーの名前が「attack_start」の時。
	if (wcscmp(eventName, L"attack_start") == 0) {
		//攻撃中判定をtrueにする。
		m_isUnderAttack = true;
	}
	//キーの名前が「attack_end」の時。
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//攻撃中判定をfalseにする。
		m_isUnderAttack = false;
	}
}


void Mediumboss::Rotation()
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
	m_enemy2.SetRotation(m_rotation);

	//プレイヤーの前ベクトルを計算する。
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

	//追跡ステートでない時、追跡処理はしない。
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
		m_enemy2.SetPosition(modelPosition);
}

void Mediumboss::Collision()
{
	//被ダメージ、あるいはダウンステートの時は。
	//当たり判定処理はしない。
	if (m_enemyState == enEnemyState_HitDamage1 ||
		m_enemyState == enEnemyState_HitDamage2 ||
		m_enemyState == enEnemyState_Down ||
		m_hp == 0)
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
			//hitdamagecooltimeがfalseの時
			if (hitdamagecooltime == false)
			{
				//hpを１減らす
				m_hp--;
			}
			hitdamagecooltime = true;
			//被ダメージステートに遷移する。
			m_enemyState = enEnemyState_HitDamage1;
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
			//hitdamagecooltimeがfalseの時
			if (hitdamagecooltime == false)
			{
				//hpを１減らす
				m_hp--;
			}
			hitdamagecooltime = true;
			//被ダメージステートに遷移する。
			m_enemyState = enEnemyState_HitDamage2;
		}
	}
}

void Mediumboss::Attack()
{

	if (m_enemyattackmotion == 1)
	{
		//攻撃ステートでないなら処理をしない。
		if (m_enemyState != enEnemyState_Attack1)
		{
			return;
		}
	}
	else if (m_enemyattackmotion == 0)
	{
		//攻撃中でないなら、処理をしない。
		if (m_enemyState != enEnemyState_Attack2)
		{
			return;
		}
	}

	//攻撃中であれば。
	if (m_isUnderAttack == true && m_enemyattackmotion == 1)
	{
		//攻撃用のコリジョンを作成する。
		MakeAttackCollision();
	}
	if (m_isUnderAttack == true && m_enemyattackmotion == 0)
	{
		//攻撃用のコリジョンを作成する。
		MakeAttackCollision2();
	}
}

const bool Mediumboss::SearchPlayer() const
{
	//プレイヤーのHPが0の時は
	//処理をしない。
	if (m_player->GetPlayerhp() == 0)
	{
		return false;
	}
	//エントリーステートがfalseの時しか動けないようにする。
	if (m_entrystate == false)
	{
		//エネミーからプレイヤーに向かうベクトルを求める。
		Vector3 diff = m_player->GetPosition() - m_position;

		//プレイヤーにある程度近かったら。
		if (diff.LengthSq() <= 100.0 * 100.0f)
		{
			//プレイヤーに向かってくる
			return true;
			/*
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
			*/
		}
		else
		{
			//プレイヤーを見つけられなかった。
			return false;
		}
	}
}

void Mediumboss::MakeAttackCollision()
{

	//攻撃当たり判定用のコリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//座標をエネミーの少し前に設定する。
	collisionPosition += m_forward * 50.0f;
	//球状のコリジョンを作成する。
	collisionObject->CreateSphere(collisionPosition,        //座標。
		Quaternion::Identity,                               //回転。
		2.0f                                               //半径。
	);
	collisionObject->SetName("enemy_attack");

	//剣のボーンのワールド行列を取得する。
	Matrix matrix = m_enemy.GetBone(m_RightHandId)->GetWorldMatrix();
	//「LeftHand」ボーンのワールド行列をコリジョンに適用する。
	collisionObject->SetWorldMatrix(matrix);
}

void Mediumboss::MakeAttackCollision2()
{

	//攻撃当たり判定用のコリジョンオブジェクトを作成する。
	auto collisionObject2 = NewGO<CollisionObject>(0);
	Vector3 collisionPosition2 = m_position;
	//座標をエネミーの少し前に設定する。
	collisionPosition2 += m_forward * 50.0f;
	//球状のコリジョンを作成する。
	collisionObject2->CreateSphere(collisionPosition2,        //座標。
		Quaternion::Identity,                               //回転。
		3.0f                                               //半径。
	);
	collisionObject2->SetName("enemy_attack2");

	//剣のボーンのワールド行列を取得する。
	Matrix matrix2 = m_enemy.GetBone(m_LeftHandId)->GetWorldMatrix();
	//「RightHand」ボーンのワールド行列をコリジョンに適用する。
	collisionObject2->SetWorldMatrix(matrix2);
}

void Mediumboss::ProcessCommonStateTransition()
{
	//各タイマーを初期化。
	//待機時間と追跡時間を制限するため。
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//エネミーからプレイヤーに向かうベクトルを計算する。
	Vector3 diff = m_player->GetPosition() - m_position;

	if (m_hp == 0)
	{
		//キャラコンの剛体を破棄する。
		m_charaCon.RemoveRigidBoby();
		//乱数によって、ダウンモーションを決定する。
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
		//プレイヤーを見つけたら。
		if (SearchPlayer() == true)
		{
			//通常攻撃できない距離なら。
			if (IsCanAttack() == false)
			{
				//ベクトルを正規化する。
				diff.Normalize();
				//移動速度を設定する。
				m_moveSpeed = diff * 50.0f;
				//y座標には移動させない
				m_moveSpeed.y = 0.0f;
				//追跡ステートに遷移する。
				//m_enemyState = enEnemyState_MagicAttack;
				m_enemyState = enEnemyState_Chase;
				return;
			}
			//攻撃できる距離なら。
			else
			{
				//乱数によって、攻撃を決定する。	
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
						//待機ステートに遷移する。
						m_enemyState = enEnemyState_Idle;
						*/
						m_enemyState = enEnemyState_Attack2;
						m_enemyattackmotion = 0;
						m_isUnderAttack = false;
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
}

void Mediumboss::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//待機時間がある程度経過したら。
	if (m_idleTimer >= 0.9f)
	{
		//他のステートへ遷移する。
		ProcessCommonStateTransition();
	}

}

void Mediumboss::ProcessRunStateTransition()
{
	//他のステートに遷移する。
	ProcessCommonStateTransition();
}

void Mediumboss::ProcessChaseStateTransition()
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

void Mediumboss::ProcessAttackStateTransition()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_enemy.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}
void Mediumboss::ProcessHitDamageStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
	if (m_enemy.IsPlayingAnimation() == false)
	{
		//hitダメージクールタイムをfalseにする。
		hitdamagecooltime = false;
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}
void Mediumboss::ProcessDownStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
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
	case enEnemyState_Attack1:
		//攻撃ステートのステート遷移処理。
		ProcessAttackStateTransition();
		break;
		//攻撃ステート2の時。
	case enEnemyState_Attack2:
		//攻撃ステートのステート遷移処理。
		ProcessAttackStateTransition();
		break;
		//エネミーステートがenPlayerState_HitDamage1だったら。
	case  enEnemyState_HitDamage1:
		//被ダメージステートのステート遷移処理。
		ProcessHitDamageStateTransition();
		break;
		//エネミーステートがenPlayerState_HitDamage2だったら。
	case  enEnemyState_HitDamage2:
		//被ダメージステートのステート遷移処理。
		ProcessHitDamageStateTransition();
		break;
		//エネミーステートがenPlayerState_Down1だったら。
	case  enEnemyState_Down1:
		//ダウンステートのステート遷移処理。
		ProcessDownStateTransition();
		break;
		//エネミーステートがenPlayerState_Down2だったら。
	case  enEnemyState_Down2:
		//ダウンステートのステート遷移処理。
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
			//待機ステートの時。
		case enEnemyState_Idle:
			//待機アニメーションを再生。
			m_enemy.PlayAnimation(enAnimationClip_Idle, 0.5f);
			m_enemy2.PlayAnimation(enAnimationClip_Idle, 0.5f);
			break;
			//追跡ステートの時。
		case enEnemyState_Chase:
			//走りアニメーションを再生。
			m_enemy.PlayAnimation(enAnimationClip_Run, 0.1f);
			m_enemy2.PlayAnimation(enAnimationClip_Run, 0.1f);
			break;
			//エネミーステートがenPlayerState_Attack1だったら。
		case enEnemyState_Attack1:
			//attackモーションを再生
			m_enemy.PlayAnimation(enAnimationClip_Attack1, 0.1f);
			m_enemy2.PlayAnimation(enAnimationClip_Attack1, 0.1f);
			break;
			//エネミーステートがenPlayerState_Attack2だったら。
		case enEnemyState_Attack2:
			//attackモーションを再生
			m_enemy.PlayAnimation(enAnimationClip_Attack2, 0.1f);
			m_enemy2.PlayAnimation(enAnimationClip_Attack2, 0.1f);
			break;
			//エネミーステートがenPlayerState_HitDamage1だったら。
		case  enEnemyState_HitDamage1:
			m_enemy.SetAnimationSpeed(1.3f);
			//HitDamageモーションを再生
			m_enemy.PlayAnimation(enAnimationClip_HitDamage1, 0.1f);
			m_enemy2.PlayAnimation(enAnimationClip_HitDamage1, 0.1f);
			break;
			//エネミーステートがenPlayerState_HitDamage2だったら。
		case  enEnemyState_HitDamage2:
			m_enemy.SetAnimationSpeed(1.3f);
			//HitDamageモーションを再生
			m_enemy.PlayAnimation(enAnimationClip_HitDamage2, 0.1f);
			m_enemy2.PlayAnimation(enAnimationClip_HitDamage2, 0.1f);
			break;
			//エネミーステートがenPlayerState_Down1だったら。
		case  enEnemyState_Down1:
			//HitDamageモーションを再生
			m_enemy.PlayAnimation(enAnimationClip_Down1, 0.1f);
			m_enemy2.PlayAnimation(enAnimationClip_Down1, 0.1f);
			break;
			//エネミーステートがenPlayerState_Down2だったら。
		case  enEnemyState_Down2:
			//HitDamageモーションを再生
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
	//プレイヤーのHPが0なら
	if (m_player->GetPlayerhp() == 0)
	{
		//攻撃できない
		return false;
	}
	//エネミーからプレイヤーに向かうベクトルを求める
	Vector3 diff = m_player->GetPosition() - m_position;
	//エネミーからプレイヤーに向かうベクトルを求める。
	Vector3 diff2 = m_player->GetPosition() - m_position;
	//エネミーとプレイヤーの距離が近かったら。
	if (diff.LengthSq() <= 8.0f * 8.0f)
	{

		//エネミーからプレイヤーに向かうベクトルを正規化(大きさを1)する。
		diff2.Normalize();
		//エネミーの正面のベクトルと、エネミーからプレイヤーに向かうベクトルの。
		//内積(cosθ)を求める。
		float cos = m_forward.Dot(diff2);
		//内積(cosθ)から角度(θ)を求める。
		float angle = acosf(cos);
		//角度(θ)が60°より小さければ。
		if (angle <= (Math::PI / 180.0f) * 60.0f)
		{
			//攻撃できる！
			return true;
		}
	}
	//攻撃できない。
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