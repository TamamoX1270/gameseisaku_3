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
	/// 0�����A1����
	/// </summary>
	/// <returns></returns>
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
	MoveFloorWall* m_movefloorwall;               //�������̕ǁB
	GameCamera* m_gameCamera;                     //�Q�[���J����
	BackGround* m_backGround;                     //�w�i�B
	ModelRender m_modelRender;
	FontRender m_fontRender;
	LevelRender m_levelRender;                  //���x���B


	int worldstate = 0;//�O����,�P����
	int m_enemykillcount = 0;
	float whitetimer = 0.0f;//���̃^�C�}�[
	float blacktimer = 0.0f;//���̃^�C�}�[
	float m_gamecameratimer = 0.0f; //�Q�[���J�����J�ڗp�̃^�C�}�[
	int m_MediumbossNewGO = false;
	int whiteChangestate = true;
	int blackChangestate = true;
	int m_BossKill = false;
	int m_movefloorwallstate = false;
	int m_floorwallcount = false;
	int	m_numEnemy = 0;					//�G�l�~�[�̐��B
};

