#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class Wall;
class Wallbreak;
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
	Player* m_player;                              //プレイヤー。
	Wall* m_wall;                                  //壁。
	Wallbreak* m_wallbreak;                                  //壁。
	GameCamera* m_gameCamera;                      //ゲームカメラ
	BackGround* m_backGround;                     //背景。
	ModelRender m_modelRender;

	int worldstate = 0;//０が白,１が黒
	float whitetimer = 0.0f;//白のタイマー
	float blacktimer = 0.0f;//黒のタイマー
	int whiteChangestate = true;
	int blackChangestate = true;
};

