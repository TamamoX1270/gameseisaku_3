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

	Player* m_player;        //�v���C���[�B
	Vector3 m_toCameraPos;	//�����_���王�_�Ɍ������x�N�g���B
};
