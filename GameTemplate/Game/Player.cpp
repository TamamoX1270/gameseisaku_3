#include "stdafx.h"
#include "Player.h"
#include "game.h"
#include "Wall.h"

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
	/*
	m_animationClips[enAnimationClip_MagicAttack].Load("Assets/animData/kachujin/magicattack.tka");
	m_animationClips[enAnimationClip_MagicAttack].SetLoopFlag(false);
	*/
	m_animationClips[enAnimationClip_HitDamage].Load("Assets/animData/kachujin/receivedamage.tka");
	m_animationClips[enAnimationClip_HitDamage].SetLoopFlag(false);
	/*
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/kachujin/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Winner].Load("Assets/animData/kachujin/winner.tka");
	m_animationClips[enAnimationClip_Winner].SetLoopFlag(false);
	*/

	//モデルを読み込む
	m_modelRender.Init("Assets/modelData/Kachujin.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	m_modelRender2.Init("Assets/modelData/blackKachujin.tkm", m_animationClips, enAnimationClip_Num, enModelUpAxisZ);

	m_modelRender.SetScale({ 1.5f,1.5f,1.5f });
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetPosition({ 1.0f,0.0f,1.0f });
	m_modelRender.Update();
	m_modelRender2.SetScale({ 1.5f,1.5f,1.5f });
	m_modelRender2.SetPosition(m_position2);
	m_modelRender2.SetPosition({ 1.0f,0.0f,1.0f });
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
	m_sphereCollider.Create(48.0f);

	//キャラコンを初期化する。
	characterController.Init(25.0f, 25.0f, m_position);

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
	Attack();
	Timer();
	Rotation();
	//絵描きさんの更新処理。
	m_modelRender.Update();
	m_modelRender2.Update();
}

void Player::Move()
{
	//移動できない状態であれば、移動処理はしない。
	if (IsEnableMove() == false)
	{
		return;
	}

	m_position2 = m_position;
	//xzの移動速度を0.0fにする。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//左スティックの入力量を取得。
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;

	//左スティックの入力量と120.0fを乗算。
	right *= stickL.x * 700.0f;
	forward *= stickL.y * 700.0f;

	//移動速度にスティックの入力量を加算する。
	m_moveSpeed += right + forward;

	//キャラクターコントローラーを使って座標を移動させる。
	m_position = characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	//白の世界の時かつ白のチェンジステートがtrueかつLB2を押されたら
	if (g_pad[0]->IsTrigger(enButtonLB2)&& worldstate==0 && whiteChangestate == true)
	{
		//黒の世界にして
		worldstate = 1;
		//黒タイマーを0.0fにする。
		blacktimer = 0.0f;
		//黒のチェンジステートをfalseにする。
		blackChangestate = false;

		m_wall = NewGO<Wall>(0, "wall");
	}
	//黒の世界の時かつ黒のチェンジステートがtrueかつLB2を押されたら
	if (g_pad[0]->IsTrigger(enButtonLB1) && worldstate == 1&& blackChangestate ==true )
	{
		//白の世界にして
		worldstate = 0;
		//白タイマーを0.0fにする。
		whitetimer = 0.0f;
		//白のチェンジステートをfalseにする。
		whiteChangestate = false;
	}

	//絵描きさんに座標を教える。
	m_modelRender.SetPosition(m_position);
	m_modelRender2.SetPosition(m_position);
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

void Player::ProcessReceiveDamageStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
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
	case enPlayerState_ReceiveDamage:
		//被ダメージ時ステートのステート遷移処理。
		ProcessReceiveDamageStateTransition();
		break;
	}
}

void Player::ProcessCommonStateTransition()
{
	//RB1ボタンが押されたら。
	if (g_pad[0]->IsTrigger(enButtonRB1)&& player_attackmotion == 0)
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
		player_attackmotion=0;
		//フラグをfalseにする。
		m_isUnderAttack = false;
		return;
	}
	if (m_attackstate == false)
	{
		//xかzの移動速度があったら(スティックの入力があったら)。
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
	}
	else if (m_attackstate == true)
	{
		if (m_attacktimer >= 3.0f)
		{
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
	case enAnimationClip_HitDamage:
		//被ダメージモーションを再生
		m_modelRender.PlayAnimation(enAnimationClip_HitDamage, 0.1f);
		m_modelRender2.PlayAnimation(enAnimationClip_HitDamage, 0.1f);
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
		m_modelRender.PlayAnimation(enAnimationClip_Attack2);
		m_modelRender2.PlayAnimation(enAnimationClip_Attack2);
		break;
		//プレイヤーステートがenPlayerState_Attack3だったら。
	case enPlayerState_Attack3:
		//attackモーションを再生
		m_modelRender.PlayAnimation(enAnimationClip_Attack3);
		m_modelRender2.PlayAnimation(enAnimationClip_Attack3);
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
		15.0f                                               //半径。
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
		15.0f                                               //半径。
	);
	collisionObject2->SetName("player_attack2");

	//「RightHand」ボーンのワールド行列を取得する。
	Matrix matrix2 = m_modelRender.GetBone(m_RightHandId)->GetWorldMatrix();
	//「RightHand」ボーンのワールド行列をコリジョンに適用する。
	collisionObject2->SetWorldMatrix(matrix2);
}

void Player::Timer()
{
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
	//白の世界の時かつ白のチェンジステートがfalseなら白タイマーを取得する。
	if (worldstate == 0&&whiteChangestate==false)
	{
		whitetimer += g_gameTime->GetFrameDeltaTime();
	}
	//白の世界のタイマーが2.0fを越えていたら
	if (whitetimer >= 2.0f&&whiteChangestate == false)
	{
		//白のチェンジステートをtrueにする。
		whiteChangestate = true;
	}
	//黒の世界の時かつ黒のチェンジステートがfalseなら黒タイマーを取得する。
	if (worldstate == 1&& blackChangestate == false)
	{
		blacktimer += g_gameTime->GetFrameDeltaTime();
	}
	//黒の世界のタイマーが2.0fを越えていたら
	if (blacktimer >= 2.0f&&blackChangestate == false)
	{
		blackChangestate = true;
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

}

void Player::Render(RenderContext& rc)
{
	//描画する。
	m_fontRender.Draw(rc);
	m_fontRender2.Draw(rc);
	m_fontRender3.Draw(rc);
	//m_fontRender4.Draw(rc);
	//m_fontRender5.Draw(rc);
	if (worldstate == 0)
	{
		m_modelRender.Draw(rc);
	}
	if (worldstate == 1)
	{
		m_modelRender2.Draw(rc);
	}
}