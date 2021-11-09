#include "stdafx.h"
#include "Player.h"
#include "game.h"

bool Player::Start()
{
	//モデルを読み込む
	m_modelRender.Init("Assets/modelData/irukaChan.tkm");
	m_modelRender2.Init("Assets/modelData/blackirukaChan.tkm");

	m_modelRender.SetScale({ 0.6f,0.6f,0.6f });
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetPosition({ 1.0f,0.0f,1.0f });
	m_modelRender.Update();
	m_modelRender2.SetScale({ 0.6f,0.6f,0.6f });
	m_modelRender2.SetPosition(m_position2);
	m_modelRender2.SetPosition({ 1.0f,0.0f,1.0f });
	m_modelRender2.Update();

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
	Timer();
	Rotation();
	//絵描きさんの更新処理。
	m_modelRender.Update();
	m_modelRender2.Update();
}

void Player::Move()
{
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

void Player::Timer()
{
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
	swprintf_s(wcsbuf4, 256, L"黒%d秒", int(blacktimer));

	//表示するテキストを設定。
	m_fontRender4.SetText(wcsbuf4);
	//フォントの位置を設定。
	m_fontRender4.SetPosition(Vector3(-500.0f, 520.0f, 0.0f));
	//フォントの大きさを設定。
	m_fontRender4.SetScale(2.1f);
	//黒色に設定
	m_fontRender4.SetColor(g_vec4White);

	wchar_t wcsbuf5[256];
	swprintf_s(wcsbuf5, 256, L"白%d秒", int(whitetimer));

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
	m_fontRender4.Draw(rc);
	m_fontRender5.Draw(rc);
	if (worldstate == 0)
	{
		m_modelRender.Draw(rc);
	}
	if (worldstate == 1)
	{
		m_modelRender2.Draw(rc);
	}
}