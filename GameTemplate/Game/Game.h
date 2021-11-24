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
	SkyCube* m_skyCube;		                      //�X�J�C�L���[�u�B
	Player* m_player;                             //�v���C���[�B
	Wall* m_wall;                                 //�ǁB
	Enemy* m_enemy;
	Wallbreak* m_wallbreak;                       //�ǁB
	Breakrock* m_breakrock;                       //�󂹂��B
	MoveFloor* m_movefloor;                       //������
	GameCamera* m_gameCamera;                     //�Q�[���J����
	BackGround* m_backGround;                     //�w�i�B
	ModelRender m_modelRender;
	LevelRender             m_levelRender;                  //���x���B

	int worldstate = 0;//�O����,�P����
	float whitetimer = 0.0f;//���̃^�C�}�[
	float blacktimer = 0.0f;//���̃^�C�}�[
	int whiteChangestate = true;
	int blackChangestate = true;
	int						m_numEnemy = 0;					//�G�l�~�[�̐��B
};

