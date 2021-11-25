#include "stdafx.h"
#include "Player.h"
#include "game.h"
#include "Wall.h"
#include "MoveFloor.h"

#include <stdlib.h>
//CollisionObjectを使用するために、ファイルをインクルードする。
#include "collision/CollisionObject.h"

bool Player::Start()
{
	//アニメーションを読み込む
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/kachujin/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/kachujin/Dwarf_Walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/kachujin/Run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack1].Load("Assets/animData/kachujin/attack.tka");
	m_animationClips[enAnimationClip_Attack1].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack2].Load("Assets/animData/kachujin/attack2.tka");
	m_animationClips[enAnimationClip_Attack2].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack3].Load("Assets/animData/kachujin/attack3.tka");
	m_animationClips[enAnimationClip_Attack3].SetLoopFlag(false);
	m_animationClips[enAnimationClip_HitDamage1].Load("Assets/animData/kachujin/hitdamage1.tka");
	m_animationClips[enAnimationClip_HitDamage1].SetLoopFlag(false);
	m_animationClips[enAnimationClip_HitDamage2].Load("Assets/animData/kachujin/hitdamage2.tka");
	m_animationClips[enAnimationClip_HitDamage2].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down1].Load("Assets/animData/kachujin/down1.tka");
	m_animationClips[enAnimationClip_Down1].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down2].Load("Assets/animData/kachujin/down2.tka");
	m_animationClips[enAnimationClip_Down2].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down3].Load("Assets/animData/kachujin/down3.tka");
	m_animationClips[enAnimationClip_Down3].SetLoopFlag(false);
	/*
	m_animationClips[enAnimationClip_MagicAttack].Load("Assets/animData/kachujin/magicattack.tka");
	m_animationClips[enAnimationClip_MagicAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Winner].Load("Assets/animData/kachujin/winner.tka");
	m_animationClips[enAnimationClip_Winner].SetLoopFlag(false);
	*/

	//モデルを読み込む
	m_modelRender.Init("Assets/modelData/Kachujin.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	m_modelRender2.Init("Assets/modelData/blackKachujin.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
	m_modelRender.SetScale(m_scale);
	m_modelRender2.SetPosition(m_position2);
	m_modelRender2.SetScale(m_scale);
	m_modelRender2.Update();

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});

	//「LeftHand」ボーンのID(番号)を取得する。
	m_LeftHandId = m_modelRender.FindBoneID(L"LeftHand");
	//「RightHand」ボーンのID(番号)を取得する。
	m_RightHandId = m_modelRender.FindBoneID(L"RightHand");

	//コライダーを初期化。（スフィアになってる)
	m_sphereCollider.Create(4.8f);

	//キャラコンを初期化する。
	m_characterController.Init(2.5f, 7.0f, m_position);

	return true;
}

Player::Player()
{

}

Player::~Player()
{
}

void Player::Update() //常に1秒間に60回呼び出される
{
	Moji();
	Move();
	ManageState();
	PlayAnimation();
	Collision();
	Attack();
	Timer();
	Rotation();
	//絵描きさんの更新処理。
	m_modelRender.Update();
	m_modelRender2.Update();
}

void Player::Move()
{
	m_position2 = m_position;
	//リフトが動いてない時の処理。
	if (FindGO<MoveFloor>("movefloor")->GetMoveFloor() == false)
	{
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;
		//移動できない状態であれば、移動処理はしない。
		if (IsEnableMove() == false)
		{
			IsEnableMoveTask();
		}
		else
		{
			Movetask();
		}
	}
	else
	{
		//移動できない状態であれば、移動処理はしない。
		if (IsEnableMove() == false)
		{
			//重力を発生させる。
			m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();
			IsEnableMoveTask();
		}
		else
		{
			m_lstick.x = 0.0f;
			m_lstick.z = 0.0f;
			Movetask();
		}
	}
	//絵描きさんに座標を教える。
	m_modelRender.SetPosition(m_position);
	m_modelRender2.SetPosition(m_position);
	//もしリフトが動いているなら
	if (FindGO<MoveFloor>("movefloor")->GetMoveFloor() == true)
	{
		//ここでmovespeedを0にする。
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;
	}
}
void Player::ProcessIdleStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

void Player::ProcessWalkStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

void Player::ProcessAttackStateTransition()
{
	//攻撃アニメーションの再生が終わったなら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}
}

void Player::ProcessHitDamageStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//被ダメージクールタイムをfalseにする。
		hitdamagecooltime = false;
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}
}

void Player::ProcessDownStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		DeleteGO(FindGO<Game>("game"));
	}
}
void Player::ManageState()
{
	switch (m_playerState)
	{
		//待機ステートの時。
	case enPlayerState_Idle:
		//待機ステートのステート遷移処理。
		ProcessIdleStateTransition();
		break;
		//歩きステートの時。
	case enPlayerState_Walk:
		//歩きステートのステート遷移処理。
		ProcessWalkStateTransition();
		break;
		//走りステートの時。
	case enPlayerState_Run:
		//走りステートのステート遷移処理。
		ProcessRunStateTransition();
		break;
		//攻撃ステート1の時。
	case enPlayerState_Attack1:
		//攻撃ステートのステート遷移処理。
		ProcessAttackStateTransition();
		break;
		//攻撃ステート2の時。
	case enPlayerState_Attack2:
		//攻撃ステートのステート遷移処理。
		ProcessAttackStateTransition();
		break;
		//攻撃ステート3の時。
	case enPlayerState_Attack3:
		//攻撃ステートのステート遷移処理。
		ProcessAttackStateTransition();
		break;
		//被ダメージ時ステートの時。
	case enPlayerState_HitDamage1:
		//被ダメージ時ステートのステート遷移処理。
		ProcessHitDamageStateTransition();
		break;
		//被ダメージ時ステート2の時。
	case enPlayerState_HitDamage2:
		//被ダメージ時ステートのステート遷移処理2。
		ProcessHitDamageStateTransition();
		break;
		//プレイヤーステートがenPlayerState_Down1だったら。
	case enPlayerState_Down1:
		//ダウンステートの遷移処理。
		ProcessDownStateTransition();
		//プレイヤーステートがenPlayerState_Down2だったら。
	case enPlayerState_Down2:
		//ダウンステートの遷移処理。
		ProcessDownStateTransition();
		//プレイヤーステートがenPlayerState_Down3だったら。
	case enPlayerState_Down3:
		//ダウンステートの遷移処理。
		ProcessDownStateTransition();
		break;
	}
}

void Player::ProcessCommonStateTransition()
{
	if (m_playerhp == 0)
	{
		//キャラコンの剛体を破棄する。
		m_characterController.RemoveRigidBoby();
		//乱数によって、ダウンモーションを決定する。
		int ram = rand() % 100;
		if (ram > 20)
		{
			if (ram > 20 && ram <= 60)
			{
				//ダウン１を再生
				m_playerState = enPlayerState_Down1;
			}
			else
			{
				//ダウン2を再生
				m_playerState = enPlayerState_Down2;
			}
		}
		else
		{
			//ダウン3を再生
			m_playerState = enPlayerState_Down3;
		}
	}
	else
	{
		//RB1ボタンが押されたら。
		if (g_pad[0]->IsTrigger(enButtonRB1) && player_attackmotion == 0)
		{
			//m_attacktimerを0.21fにする。(0.3fの猶予を持たせるため)
			m_attacktimer = 0.21f;
			//攻撃中にする。
			m_attackstate = true;
			//攻撃ステートに移行する。
			m_playerState = enPlayerState_Attack1;
			//player_atacckmotionを+1する。
			player_attackmotion++;
			//フラグをfalseにする。
			m_isUnderAttack = false;
			return;
		}
		else if (g_pad[0]->IsTrigger(enButtonRB1) && player_attackmotion == 1)
		{
			//m_attacktimerを0.2fにする。(0.3fの猶予を持たせるため)
			m_attacktimer = 0.2f;
			//攻撃中にする。
			m_attackstate = true;
			//攻撃ステートに移行する。
			m_playerState = enPlayerState_Attack2;
			//player_atacckmotionを+1する。
			player_attackmotion++;
			//フラグをfalseにする。
			m_isUnderAttack = false;
			return;
		}
		else if (g_pad[0]->IsTrigger(enButtonRB1) && player_attackmotion == 2)
		{
			//m_attacktimerを0.36fにする。(0.3fの猶予を持たせるため)
			m_attacktimer = 0.36f;
			//攻撃中にする。
			m_attackstate = true;
			//攻撃ステートに移行する。
			m_playerState = enPlayerState_Attack3;
			//player_atacckmotionをリセットする。
			player_attackmotion = 0;
			//フラグをfalseにする。
			m_isUnderAttack = false;
			return;
		}
		if (m_attackstate == false)
		{
			//xかzの移動速度があったら(スティックの入力があったら)。
			if (fabsf(lStick_x) >= 0.01f || fabsf(lStick_y) >= 0.01f)
			{
				//ステートを2(歩き)にする。
				m_playerState = enPlayerState_Walk;
			}
			/*
			if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
			{
				//ステートを2(歩き)にする。
				m_playerState = enPlayerState_Walk;
			}
			*/


			//xとzの移動速度が無かったら(スティックの入力が無かったら)。
			else
			{
				//ステートを0(待機)にする。
				m_playerState = enPlayerState_Idle;
			}
		}
		else if (m_attackstate == true)
		{
			if (m_attacktimer >= 3.0f)
			{
				//xかzの移動速度があったら(スティックの入力があったら)。
				if (fabsf(lStick_x) >= 0.01f || fabsf(lStick_y) >= 0.01f)
				{
					//ステートを2(歩き)にする。
					m_playerState = enPlayerState_Walk;
				}
				/*
				if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
				{
					//ステートを2(歩き)にする。
					m_playerState = enPlayerState_Walk;
				}
				//xとzの移動速度が無かったら(スティックの入力が無かったら)。
				else
				{
					//ステートを0(待機)にする。
					m_playerState = enPlayerState_Idle;
				}
				*/
			}
		}
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//キーの名前が「attack_start」の時。
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		//攻撃中にする。
		m_isUnderAttack = true;
	}
	//キーの名前が「attack_end」の時。
	else if (wcscmp(eventName, L"attack_end") == 0)
	{
		//攻撃を終わる。
		m_isUnderAttack = false;
	}

}

void Player::PlayAnimation()
{

	//switch文。
	switch (m_playerState) {
		//プレイヤーステートがidleだったら。
	case enPlayerState_Idle:
		//待機アニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		m_modelRender2.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//プレイヤーステートがwalkだったら。
	case enPlayerState_Walk:
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		m_modelRender2.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
		//プレイヤーステートがHitDamageだったら。
	case enAnimationClip_HitDamage1:
		//被ダメージモーションを再生
		m_modelRender.PlayAnimation(enAnimationClip_HitDamage1, 0.3f);
		m_modelRender2.PlayAnimation(enAnimationClip_HitDamage1, 0.3f);
		break;
		//プレイヤーステートがHitDamage2だったら。
	case enAnimationClip_HitDamage2:
		//被ダメージモーションを再生
		m_modelRender.PlayAnimation(enAnimationClip_HitDamage2, 0.3f);
		m_modelRender2.PlayAnimation(enAnimationClip_HitDamage2, 0.3f);
		break;
		//プレイヤーステートがenPlayerState_Attack1だったら。
	case enPlayerState_Attack1:
		//attackモーションを再生
		m_modelRender.PlayAnimation(enAnimationClip_Attack1, 0.1f);
		m_modelRender2.PlayAnimation(enAnimationClip_Attack1, 0.1f);
		break;
		//プレイヤーステートがenPlayerState_Attack2だったら。
	case enPlayerState_Attack2:
		//attackモーションを再生
		m_modelRender.PlayAnimation(enAnimationClip_Attack2, 0.1f);
		m_modelRender2.PlayAnimation(enAnimationClip_Attack2, 0.1f);
		break;
		//プレイヤーステートがenPlayerState_Attack3だったら。
	case enPlayerState_Attack3:
		//attackモーションを再生
		m_modelRender.PlayAnimation(enAnimationClip_Attack3, 0.1f);
		m_modelRender2.PlayAnimation(enAnimationClip_Attack3, 0.1f);
		break;
		//プレイヤーステートがenPlayerState_Down1だったら。
	case enPlayerState_Down1:
		//downモーションを再生
		m_modelRender.PlayAnimation(enAnimationClip_Down1, 0.1f);
		m_modelRender2.PlayAnimation(enAnimationClip_Down1, 0.1f);
		break;
		//プレイヤーステートがenPlayerState_Down2だったら。
	case enPlayerState_Down2:
		//downモーションを再生
		m_modelRender.PlayAnimation(enAnimationClip_Down2, 0.1f);
		m_modelRender2.PlayAnimation(enAnimationClip_Down2, 0.1f);
		break;
		//プレイヤーステートがenPlayerState_Down3だったら。
	case enPlayerState_Down3:
		//downモーションを再生
		m_modelRender.PlayAnimation(enAnimationClip_Down3, 0.1f);
		m_modelRender2.PlayAnimation(enAnimationClip_Down3, 0.1f);
		break;
	}
	
}

void Player::Attack()
{
	if (player_attackmotion == 1)
	{
		//攻撃中でないなら、処理をしない。
		if (m_playerState != enPlayerState_Attack1)
		{
			return;
		}
	}
	else if (player_attackmotion == 2)
	{
		//攻撃中でないなら、処理をしない。
		if (m_playerState != enPlayerState_Attack2)
		{
			return;
		}
	}
	else if (player_attackmotion == 0)
	{
		//攻撃中でないなら、処理をしない。
		if (m_playerState != enPlayerState_Attack3)
		{
			return;
		}
	}

	//攻撃判定中でかつplayer_attackmotionが1であれば。
	if (m_isUnderAttack == true && player_attackmotion == 1)
	{
		//攻撃用のコリジョンを作成する。
		MakeAttackCollision();
	}
	//攻撃判定中でかつplayer_attackmotionが2であれば。
	if (m_isUnderAttack == true && player_attackmotion == 2)
	{
		//攻撃用のコリジョンを作成する。
		MakeAttackCollision2();
	}
	//攻撃判定中でかつplayer_attackmotionが0であれば。
	if (m_isUnderAttack == true && player_attackmotion == 0)
	{
		//攻撃用のコリジョンを作成する。
		MakeAttackCollision();
	}

}

void Player::MakeAttackCollision()
{
	//コリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//座標をプレイヤーの少し前に設定する。
	collisionPosition += m_forward * 50.0f;
	//球状のコリジョンを作成する。
	collisionObject->CreateSphere(collisionPosition,        //座標。
		Quaternion::Identity,                               //回転。
		1.5f                                               //半径。
	);
	collisionObject->SetName("player_attack");

	//「LeftHand」ボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_LeftHandId)->GetWorldMatrix();
	//「LeftHand」ボーンのワールド行列をコリジョンに適用する。
	collisionObject->SetWorldMatrix(matrix);
}

void Player::MakeAttackCollision2()
{
	//コリジョンオブジェクトを作成する。
	auto collisionObject2 = NewGO<CollisionObject>(1);

	Vector3 collisionPosition2 = m_position;
	//座標をプレイヤーの少し前に設定する。
	collisionPosition2 += m_forward * 50.0f;
	//球状のコリジョンを作成する。
	collisionObject2->CreateSphere(collisionPosition2,        //座標。
		Quaternion::Identity,                               //回転。
		1.5f                                               //半径。
	);
	collisionObject2->SetName("player_attack2");

	//「RightHand」ボーンのワールド行列を取得する。
	Matrix matrix2 = m_modelRender.GetBone(m_RightHandId)->GetWorldMatrix();
	//「RightHand」ボーンのワールド行列をコリジョンに適用する。
	collisionObject2->SetWorldMatrix(matrix2);
}

void Player::Timer()
{
	//被ダメージステートがtrueならタイマーを取得する。
	if (m_hitdamagestate == true)
	{
		m_hitdamagetimer += g_gameTime->GetFrameDeltaTime();
	}
	//攻撃中タイマーを取得する
	if (m_attackstate == true)
	{
		m_attacktimer += g_gameTime->GetFrameDeltaTime();
	}
	//攻撃中かつm_attacktimerが1.3fを越えていたら(攻撃終了後0.5秒経過で)
	if (m_attackstate == true && m_attacktimer >= 1.3f)
	{
		//m_attackstateをfalseにしてm_attacktimerを0に戻す
		m_attackstate = false;
		m_attacktimer = 0.0f;
	}
	//被ダメージステートがtrueかつタイマーが3.0fを越えていたら
	if (m_hitdamagestate == true && m_hitdamagetimer >= 3.0f)
	{
		//m_hitdamagestateをfalseにしてm_hitdamagetimerを0に戻す
		m_hitdamagestate = false;
		m_hitdamagetimer = 0.0f;
	}

	wchar_t wcsbuf4[256];
	swprintf_s(wcsbuf4, 256, L"黒チェンジ%d秒", int(blacktimer));

	//表示するテキストを設定。
	m_fontRender4.SetText(wcsbuf4);
	//フォントの位置を設定。
	m_fontRender4.SetPosition(Vector3(-500.0f, 520.0f, 0.0f));
	//フォントの大きさを設定。
	m_fontRender4.SetScale(2.1f);
	//黒色に設定
	m_fontRender4.SetColor(g_vec4White);

	wchar_t wcsbuf5[256];
	swprintf_s(wcsbuf5, 256, L"白チェンジ%d秒", int(whitetimer));

	//表示するテキストを設定。
	m_fontRender5.SetText(wcsbuf5);
	//フォントの位置を設定。
	m_fontRender5.SetPosition(Vector3(500.0f, 520.0f, 0.0f));
	//フォントの大きさを設定。
	m_fontRender5.SetScale(2.1f);
	//黒色に設定
	m_fontRender5.SetColor(g_vec4White);
}

void Player::Rotation()
{
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(lStick_x) >= 0.01f || fabsf(lStick_y) >= 0.01f)
	{
		//キャラクターの方向を変える。
		m_rotation.SetRotationYFromDirectionXZ(m_lstick);
		//絵描きさんに回転を教える。
		m_modelRender.SetRotation(m_rotation);
		m_modelRender2.SetRotation(m_rotation);
		//プレイヤーの前方向のベクトルを設定する。
		m_forward = Vector3(0.0f, 0.0f, 1.0f);
		//ベクトルにクウォータニオンを適応してプレイヤーの向きに回転させる
		m_rotation.Apply(m_forward);
	}
	
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//キャラクターの方向を変える。
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		//絵描きさんに回転を教える。
		m_modelRender.SetRotation(m_rotation);
		m_modelRender2.SetRotation(m_rotation);
		//プレイヤーの前方向のベクトルを設定する。
		m_forward = Vector3(0.0f, 0.0f, 1.0f);
		//ベクトルにクウォータニオンを適応してプレイヤーの向きに回転させる
		m_rotation.Apply(m_forward);
	}
	
}

void Player::Collision()
{
	//被ダメージ、あるいはダウンステートの時、あるいはプレイヤーのHPが0の時あるいは被ダメステートがtrueの時
	// あるいはダウンステートの時。
	//当たり判定処理はしない。
	if (m_playerState == enPlayerState_HitDamage1 ||
		m_playerState == enPlayerState_HitDamage2 ||
		m_playerState == enPlayerState_Down1 ||
		m_playerState == enPlayerState_Down2 ||
		m_playerState == enPlayerState_Down3 ||
		m_playerhp == 0||
		m_hitdamagestate ==true)
	{
		return;
	}
	//Enemyが作成した、攻撃のヒット判定用のコリジョンの配列を取得。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	//for文で配列を回す。
	for (auto collision : collisions)
	{
		//攻撃のコリジョンと敵のキャラコンが。
		//衝突していたら。
		if (collision->IsHit(m_characterController) == true)
		{
			//hitdamagecooltimeがfalseの時
			if (hitdamagecooltime == false)
			{
				//hpを１減らす
				m_playerhp--;
			}
			//被ダメクールタイムをtrueにする。
			hitdamagecooltime = true;
			//被ダメステートをtrueにする。
			m_hitdamagestate = true;
			//被ダメージステートに遷移する。
			m_playerState = enPlayerState_HitDamage1;
		}
	}
	//Enemyが作成した、攻撃のヒット判定用のコリジョン2の配列を取得。
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("enemy_attack2");
	//for文で配列を回す。
	for (auto collision : collisions2)
	{
		//攻撃のコリジョンと敵のコリジョンが。
		//衝突していたら。
		if (collision->IsHit(m_characterController) == true)
		{
			//hitdamagecooltimeがfalseの時
			if (hitdamagecooltime == false)
			{
				//hpを１減らす
				m_playerhp--;
			}
			//被ダメクールタイムをtrueにする。
			hitdamagecooltime = true;
			//被ダメステートをtrueにする。
			m_hitdamagestate = true;
			//被ダメージステートに遷移する。
			m_playerState = enPlayerState_HitDamage2;
		}
	}
}

void Player::Moji()
{

int x = m_position.x;
wchar_t wcsbuf[256];
swprintf_s(wcsbuf, 256, L"x:%d", x);

//表示するテキストを設定。
m_fontRender.SetText(wcsbuf);
//フォントの位置を設定。
m_fontRender.SetPosition(Vector3(-500.0f, 200.0f, 0.0f));
//フォントの大きさを設定。
m_fontRender.SetScale(1.0f);

int y = m_position.y;
wchar_t wcsbuf2[256];
swprintf_s(wcsbuf2, 256, L"y:%d", y);

//表示するテキストを設定。
m_fontRender2.SetText(wcsbuf2);
//フォントの位置を設定。
m_fontRender2.SetPosition(Vector3(-500.0f, 150.0f, 0.0f));
//フォントの大きさを設定。
m_fontRender2.SetScale(1.0f);

int z = m_position.z;
wchar_t wcsbuf3[256];
swprintf_s(wcsbuf3, 256, L"z:%d", z);

//表示するテキストを設定。
m_fontRender3.SetText(wcsbuf3);
//フォントの位置を設定。
m_fontRender3.SetPosition(Vector3(-500.0f, 100.0f, 0.0f));
//フォントの大きさを設定。
m_fontRender3.SetScale(1.0f);

int plahp = m_playerhp;
wchar_t wcsbuf6[256];
swprintf_s(wcsbuf6, 256, L"プレHP:%d", plahp);

//表示するテキストを設定。
m_fontRender6.SetText(wcsbuf6);
//フォントの位置を設定。
m_fontRender6.SetPosition(Vector3(-300.0f, 0.0f, 0.0f));
//フォントの大きさを設定。
m_fontRender6.SetScale(1.0f);

}

void Player::Render(RenderContext& rc)
{
	//描画する。
	m_fontRender.Draw(rc);
	m_fontRender2.Draw(rc);
	m_fontRender3.Draw(rc);
	//m_fontRender4.Draw(rc);
	//m_fontRender5.Draw(rc);
	m_fontRender6.Draw(rc);
	if (FindGO<Game>("game")->GetWorldState() == 0)
	{
		m_modelRender.Draw(rc);
	}
	if (FindGO<Game>("game")->GetWorldState() == 1)
	{
		m_modelRender2.Draw(rc);
	}
}