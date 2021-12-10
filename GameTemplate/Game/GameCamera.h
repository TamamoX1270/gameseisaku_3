#pragma once

class Player;                   //�N���X�錾

//�Q�[�����̃J�����𐧌䂷��
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
	Player* m_player;                                       //�v���C���[�B
	Vector3 m_toCameraPos;                               	//�����_���王�_�Ɍ������x�N�g���B
	int m_camerastate = 0;                                  //�J�����X�e�[�g�B1���v���C���[�A�Q�����{�X
};
