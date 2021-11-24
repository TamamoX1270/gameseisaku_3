#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Wall.h"
#include "Wallbreak.h"
#include "Breakrock.h"
#include "Enemy.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "MoveFloor.h"

//スカイキューブ。
#include "nature/SkyCube.h"

bool Game::Start()
{
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetType(enSkyCubeType_DayToon_2);
	m_skyCube->SetType2(enSkyCubeType_NightToon_2);
	m_skyCube->SetScale({ 700.0f });
	m_skyCube->SetScale2({ 700.0f });
	m_skyCube->SetLuminance({ 0.1f });
	m_skyCube->SetLuminance2({ 0.1f });

	//m_wall = NewGO<Wall>(0, "wall");
	//m_wallbreak = NewGO<Wallbreak>(0, "wallbreak");
	//m_player = NewGO<Player>(0, "player");
	//m_backGround = NewGO<BackGround>(0, "backGround");
	//m_enemy = NewGO<Enemy>(0, "enemy");

	//レベルを構築する。
	m_levelRender.Init("Assets/level3D/stagelevel.tkl",
		[&](LevelObjectData& objData)
	{
		//名前がhumanだったら。
		if (objData.EqualObjectName(L"kachujin_g_rosales") == true)
		{
			//プレイヤーのオブジェクトを作成する。
			m_player = NewGO<Player>(1, "player");
			//座標を設定する。
			m_player->SetPosition(objData.position);
			//回転を設定する。
			m_player->SetRotation(objData.rotation);
			//大きさを設定する。
			m_player->SetScale(objData.scale);
			//trueにすると、レベルの方でモデルが読み込まれます。
			return true;
		}
		//名前がbackgroundだったら。
		else if (objData.EqualObjectName(L"background") == true)
		{
			//背景オブジェクトを作成する。
			m_backGround = NewGO<BackGround>(0, "background");
			//座標を設定する。
			m_backGround->SetPosition(objData.position);
			//大きさを設定する。
			m_backGround->SetScale(objData.scale);
			//回転を設定する。
			m_backGround->SetRotation(objData.rotation);
			return true;
		}
		
		//名前がenemyだったら。
		else if (objData.EqualObjectName(L"vampire_a_lusth") == true)
		{
			//エネミーのオブジェクトを作成する。
			Enemy* enemy = NewGO<Enemy>(0, "enemy");
			//座標を設定する。
			enemy->SetPosition(objData.position);
			//回転を設定する。
			enemy->SetRotation(objData.rotation);
			//大きさを設定する。
			enemy->SetScale(objData.scale);
			//HPを5に設定する。
			enemy->SetHP(5);
			//作成したエネミーの和を数えたいので、+1する。
			m_numEnemy++;
			return true;
		}
		
		//名前がmovefloorだったら。
		else if (objData.EqualObjectName(L"movefloor") == true)
		{
			//動く床オブジェクトを作成する。
			m_movefloor = NewGO<MoveFloor>(0, "movefloor");
			//座標を設定する。
			m_movefloor->SetPosition(objData.position);
			//大きさを設定する。
			m_movefloor->SetScale(objData.scale);
			//回転を設定する。
			m_movefloor->SetRotation(objData.rotation);
			return true;
		}
		//名前がbreakrockだったら。
		else if (objData.EqualObjectName(L"breakrock") == true)
		{
			//壊せる岩オブジェクトを作成する。
			m_breakrock = NewGO<Breakrock>(0, "background");
			//座標を設定する。
			m_breakrock->SetPosition(objData.position);
			//大きさを設定する。
			m_breakrock->SetScale(objData.scale);
			//回転を設定する。
			m_breakrock->SetRotation(objData.rotation);
			return true;
		}
		return true;
	});

	return true;
}
Game::~Game()
{
	DeleteGO(m_backGround);
	DeleteGO(m_wallbreak);
	DeleteGO(m_gameCamera);
	DeleteGO(m_enemy);
	DeleteGO(m_player);
	DeleteGO(m_skyCube);
}


void Game::Update()
{
	Timer();
	WorldChanege();
}

void Game::Timer()
{
	//白の世界の時かつ白のチェンジステートがfalseなら白タイマーを取得する。
	if (worldstate == 0 && whiteChangestate == false)
	{
		whitetimer += g_gameTime->GetFrameDeltaTime();
	}
	//白の世界のタイマーが2.0fを越えていたら
	if (whitetimer >= 2.0f && whiteChangestate == false)
	{
		//白のチェンジステートをtrueにする。
		whiteChangestate = true;
	}
	//黒の世界の時かつ黒のチェンジステートがfalseなら黒タイマーを取得する。
	if (worldstate == 1 && blackChangestate == false)
	{
		blacktimer += g_gameTime->GetFrameDeltaTime();
	}
	//黒の世界のタイマーが2.0fを越えていたら
	if (blacktimer >= 2.0f && blackChangestate == false)
	{
		blackChangestate = true;
	}
}
void Game::WorldChanege()
{
	//白の世界の時かつ白のチェンジステートがtrueかつLB2を押されたら
	if (g_pad[0]->IsTrigger(enButtonLB2) && worldstate == 0 && whiteChangestate == true)
	{
		//黒の世界にして
		worldstate = 1;
		m_skyCube->SetWorldState({ 1 });//０が白,１が黒;
		//黒タイマーを0.0fにする。
		blacktimer = 0.0f;
		//黒のチェンジステートをfalseにする。
		blackChangestate = false;
	}
	//黒の世界の時かつ黒のチェンジステートがtrueかつLB2を押されたら
	if (g_pad[0]->IsTrigger(enButtonLB1) && worldstate == 1 && blackChangestate == true)
	{
		//白の世界にして
		worldstate = 0;
		m_skyCube->SetWorldState({ 0 });//０が白,１が黒;
		//白タイマーを0.0fにする。
		whitetimer = 0.0f;
		//白のチェンジステートをfalseにする。
		whiteChangestate = false;
	}
}

void Game::Render(RenderContext& rc)
{
}