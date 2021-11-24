#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class Enemy;
class Wall;
class Wallbreak;
class MoveFloor;
class Breakrock;
class BackGround;
class GameCamera;

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
	GameCamera* m_gameCamera;                     //ゲームカメラ
	BackGround* m_backGround;                     //背景。
	ModelRender m_modelRender;
	LevelRender             m_levelRender;                  //レベル。

	int worldstate = 0;//０が白,１が黒
	float whitetimer = 0.0f;//白のタイマー
	float blacktimer = 0.0f;//黒のタイマー
	int whiteChangestate = true;
	int blackChangestate = true;
	int						m_numEnemy = 0;					//エネミーの数。
};

