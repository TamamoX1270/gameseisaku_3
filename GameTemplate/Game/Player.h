#pragma once

class Player : public IGameObject
{
public:
	Player();
	~Player();
	//�ŏ��Ɉ�񂾂����s
	bool Start();
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);
	//�ړ�����
	void Move();
	//��]�����B
	void Rotation();
	void Moji();
	void Timer();

	FontRender m_fontRender;                         //�����̕`��
	FontRender m_fontRender2;                       //�����̕`��
	FontRender m_fontRender3;                         //�����̕`��
	FontRender m_fontRender4;                         //�����̕`��
	FontRender m_fontRender5;                         //�����̕`��

	CharacterController characterController;       //�L�����N�^�[�R���g���[���[�B
	SphereCollider m_sphereCollider;		      //�~�^�̃R���C�_�[�B
	Vector3 m_position;			                 //���W�B
	Vector3 m_position2;			             //���W2�B
	Vector3 m_moveSpeed;				        //�ړ����x�B
	Quaternion m_rotation;                  //�N�E�H�[�^�j�I��
	ModelRender m_modelRender;             //���f�������_�[
	ModelRender m_modelRender2;             //���f�������_�[2
	Vector3 m_forward;                    //�L�����N�^�[�̑O�����̃x�N�g��

	float whitetimer = 0.0f;
	float blacktimer = 0.0f;
	int whiteChangestate = true;
	int blackChangestate = true;
	int worldstate = 0;//�O����,�P����
};