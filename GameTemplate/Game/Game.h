#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
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

	Player* m_player;                              //プレイヤー。
	GameCamera* m_gameCamera;                       //ゲームカメラ
	BackGround* m_backGround;                     //背景。

private:
	ModelRender m_modelRender;
};

