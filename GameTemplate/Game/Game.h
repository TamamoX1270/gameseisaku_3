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

	Player* m_player;                              //�v���C���[�B
	GameCamera* m_gameCamera;                       //�Q�[���J����
	BackGround* m_backGround;                     //�w�i�B

private:
	ModelRender m_modelRender;
};

