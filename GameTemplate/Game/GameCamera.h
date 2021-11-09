#pragma once

class Player;                   //クラス宣言

//ゲーム中のカメラを制御する
class GameCamera : public IGameObject
{
public:
	GameCamera() {};
	~GameCamera() {};
	bool Start();
	void Update();

	Player* m_player;        //プレイヤー。
	Vector3 m_toCameraPos;	//注視点から視点に向かうベクトル。
};
