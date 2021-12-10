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
	void SetCameraState(const int camera)
	{
		m_camerastate = camera;
	}
	const int GetCameraState() const
	{
		return m_camerastate;
	}
private:
	Vector3 m_pos;
	Quaternion qRot;
	Quaternion qRot2{ 0.0f,0.0f,0.0f,0.0f };
	Player* m_player;                                       //プレイヤー。
	Vector3 m_toCameraPos;                               	//注視点から視点に向かうベクトル。
	int m_camerastate = 0;                                  //カメラステート。1がプレイヤー、２が中ボス
};
