#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class Enemy;
class Wall;
class Wallbreak;
class MoveFloor;
class MoveFloorWall;
class Breakrock;
class BackGround;
class GameCamera;
class Skycube;

class Game : public IGameObject
{
public:
	Game() {}
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Timer();
	void WorldChanege();
	void BossNewGO();
	void MoveFloorWallNewGO();
	void KnockCount();

	void SetKillCount()
	{
		m_enemykillcount++;
	}
	void SetBosskill(const int kill)
	{
		m_BossKill = kill;
	}
	void SetMoveFloorState(const int floorstate)
	{
		m_movefloorwallstate = floorstate;
	}
	/// <summary>
	/// 0が白、1が黒
	/// </summary>
	/// <returns></returns>
	const int GetWorldState() const
	{
		return worldstate;
	}
private: 
	SkyCube* m_skyCube;		                      //スカイキューブ。
	Player* m_player;                             //プレイヤー。
	Wall* m_wall;                                 //壁。
	Enemy* m_enemy;
	Wallbreak* m_wallbreak;                       //壁。
	Breakrock* m_breakrock;                       //壊せる岩。
	MoveFloor* m_movefloor;                       //動く床
	MoveFloorWall* m_movefloorwall;               //動く床の壁。
	GameCamera* m_gameCamera;                     //ゲームカメラ
	BackGround* m_backGround;                     //背景。
	ModelRender m_modelRender;
	FontRender m_fontRender;
	LevelRender m_levelRender;                  //レベル。


	int worldstate = 0;//０が白,１が黒
	int m_enemykillcount = 0;
	float whitetimer = 0.0f;//白のタイマー
	float blacktimer = 0.0f;//黒のタイマー
	float m_gamecameratimer = 0.0f; //ゲームカメラ遷移用のタイマー
	int m_MediumbossNewGO = false;
	int whiteChangestate = true;
	int blackChangestate = true;
	int m_BossKill = false;
	int m_movefloorwallstate = false;
	int m_floorwallcount = false;
	int	m_numEnemy = 0;					//エネミーの数。
};

