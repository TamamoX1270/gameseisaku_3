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
	Player* m_player;                              //�v���C���[�B
	Wall* m_wall;                                  //�ǁB
	Wallbreak* m_wallbreak;                                  //�ǁB
	GameCamera* m_gameCamera;                      //�Q�[���J����
	BackGround* m_backGround;                     //�w�i�B
	ModelRender m_modelRender;
};

