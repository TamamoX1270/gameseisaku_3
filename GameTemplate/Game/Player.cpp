#include "stdafx.h"
#include "Player.h"
#include "game.h"

bool Player::Start()
{
	//���f����ǂݍ���
	m_modelRender.Init("Assets/modelData/irukaChan.tkm");
	m_modelRender2.Init("Assets/modelData/blackirukaChan.tkm");

	m_modelRender.SetScale({ 0.6f,0.6f,0.6f });
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetPosition({ 1.0f,0.0f,1.0f });
	m_modelRender.Update();
	m_modelRender2.SetScale({ 0.6f,0.6f,0.6f });
	m_modelRender2.SetPosition(m_position2);
	m_modelRender2.SetPosition({ 1.0f,0.0f,1.0f });
	m_modelRender2.Update();

	//�R���C�_�[���������B�i�X�t�B�A�ɂȂ��Ă�)
	m_sphereCollider.Create(48.0f);

	//�L�����R��������������B
	characterController.Init(25.0f, 25.0f, m_position);

	return true;
}

Player::Player()
{

}

Player::~Player()
{
}

void Player::Update() //���1�b�Ԃ�60��Ăяo�����
{
	Moji();
	Move();
	Timer();
	Rotation();
	//�G�`������̍X�V�����B
	m_modelRender.Update();
	m_modelRender2.Update();
}

void Player::Move()
{
	m_position2 = m_position;
	//xz�̈ړ����x��0.0f�ɂ���B
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ��擾�B
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	//y�����ɂ͈ړ������Ȃ��B
	forward.y = 0.0f;
	right.y = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ�120.0f����Z�B
	right *= stickL.x * 700.0f;
	forward *= stickL.y * 700.0f;

	//�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	m_moveSpeed += right + forward;

	//�L�����N�^�[�R���g���[���[���g���č��W���ړ�������B
	m_position = characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	//���̐��E�̎������̃`�F���W�X�e�[�g��true����LB2�������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonLB2)&& worldstate==0 && whiteChangestate == true)
	{
		//���̐��E�ɂ���
		worldstate = 1;
		//���^�C�}�[��0.0f�ɂ���B
		blacktimer = 0.0f;
		//���̃`�F���W�X�e�[�g��false�ɂ���B
		blackChangestate = false;
	}
	//���̐��E�̎������̃`�F���W�X�e�[�g��true����LB2�������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonLB1) && worldstate == 1&& blackChangestate ==true )
	{
		//���̐��E�ɂ���
		worldstate = 0;
		//���^�C�}�[��0.0f�ɂ���B
		whitetimer = 0.0f;
		//���̃`�F���W�X�e�[�g��false�ɂ���B
		whiteChangestate = false;
	}

	//�G�`������ɍ��W��������B
	m_modelRender.SetPosition(m_position);
	m_modelRender2.SetPosition(m_position);
}

void Player::Timer()
{
	//���̐��E�̎������̃`�F���W�X�e�[�g��false�Ȃ甒�^�C�}�[���擾����B
	if (worldstate == 0&&whiteChangestate==false)
	{
		whitetimer += g_gameTime->GetFrameDeltaTime();
	}
	//���̐��E�̃^�C�}�[��2.0f���z���Ă�����
	if (whitetimer >= 2.0f&&whiteChangestate == false)
	{
		//���̃`�F���W�X�e�[�g��true�ɂ���B
		whiteChangestate = true;
	}
	//���̐��E�̎������̃`�F���W�X�e�[�g��false�Ȃ獕�^�C�}�[���擾����B
	if (worldstate == 1&& blackChangestate == false)
	{
		blacktimer += g_gameTime->GetFrameDeltaTime();
	}
	//���̐��E�̃^�C�}�[��2.0f���z���Ă�����
	if (blacktimer >= 2.0f&&blackChangestate == false)
	{
		blackChangestate = true;
	}

	wchar_t wcsbuf4[256];
	swprintf_s(wcsbuf4, 256, L"��%d�b", int(blacktimer));

	//�\������e�L�X�g��ݒ�B
	m_fontRender4.SetText(wcsbuf4);
	//�t�H���g�̈ʒu��ݒ�B
	m_fontRender4.SetPosition(Vector3(-500.0f, 520.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_fontRender4.SetScale(2.1f);
	//���F�ɐݒ�
	m_fontRender4.SetColor(g_vec4White);

	wchar_t wcsbuf5[256];
	swprintf_s(wcsbuf5, 256, L"��%d�b", int(whitetimer));

	//�\������e�L�X�g��ݒ�B
	m_fontRender5.SetText(wcsbuf5);
	//�t�H���g�̈ʒu��ݒ�B
	m_fontRender5.SetPosition(Vector3(500.0f, 520.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_fontRender5.SetScale(2.1f);
	//���F�ɐݒ�
	m_fontRender5.SetColor(g_vec4White);
}

void Player::Rotation()
{
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//�L�����N�^�[�̕�����ς���B
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		//�G�`������ɉ�]��������B
		m_modelRender.SetRotation(m_rotation);
		m_modelRender2.SetRotation(m_rotation);
		//�v���C���[�̑O�����̃x�N�g����ݒ肷��B
		m_forward = Vector3(0.0f, 0.0f, 1.0f);
		//�x�N�g���ɃN�E�H�[�^�j�I����K�����ăv���C���[�̌����ɉ�]������
		m_rotation.Apply(m_forward);
	}
}

void Player::Moji()
{

int x = m_position.x;
wchar_t wcsbuf[256];
swprintf_s(wcsbuf, 256, L"x:%d", x);

//�\������e�L�X�g��ݒ�B
m_fontRender.SetText(wcsbuf);
//�t�H���g�̈ʒu��ݒ�B
m_fontRender.SetPosition(Vector3(-500.0f, 200.0f, 0.0f));
//�t�H���g�̑傫����ݒ�B
m_fontRender.SetScale(1.0f);

int y = m_position.y;
wchar_t wcsbuf2[256];
swprintf_s(wcsbuf2, 256, L"y:%d", y);

//�\������e�L�X�g��ݒ�B
m_fontRender2.SetText(wcsbuf2);
//�t�H���g�̈ʒu��ݒ�B
m_fontRender2.SetPosition(Vector3(-500.0f, 150.0f, 0.0f));
//�t�H���g�̑傫����ݒ�B
m_fontRender2.SetScale(1.0f);

int z = m_position.z;
wchar_t wcsbuf3[256];
swprintf_s(wcsbuf3, 256, L"z:%d", z);

//�\������e�L�X�g��ݒ�B
m_fontRender3.SetText(wcsbuf3);
//�t�H���g�̈ʒu��ݒ�B
m_fontRender3.SetPosition(Vector3(-500.0f, 100.0f, 0.0f));
//�t�H���g�̑傫����ݒ�B
m_fontRender3.SetScale(1.0f);

}

void Player::Render(RenderContext& rc)
{
	//�`�悷��B
	m_fontRender.Draw(rc);
	m_fontRender2.Draw(rc);
	m_fontRender3.Draw(rc);
	m_fontRender4.Draw(rc);
	m_fontRender5.Draw(rc);
	if (worldstate == 0)
	{
		m_modelRender.Draw(rc);
	}
	if (worldstate == 1)
	{
		m_modelRender2.Draw(rc);
	}
}