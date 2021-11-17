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
private:
	Player* m_player;                              //プレイヤー。
	Wall* m_wall;                                  //壁。
	Wallbreak* m_wallbreak;                                  //壁。
	GameCamera* m_gameCamera;                      //ゲームカメラ
	BackGround* m_backGround;                     //背景。
	ModelRender m_modelRender;
};

