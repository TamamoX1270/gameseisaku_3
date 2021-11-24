#pragma once

class Collision;
class Wall;

class Player : public IGameObject
{
public:
	Player();
	~Player();
	//�ŏ��Ɉ�񂾂����s
	bool Start();
	//��������(�X�e�[�g)���ǂ������擾�B
    //��������(�X�e�[�g)�Ȃ�true�B
	bool IsEnableMove() const
	{
		return m_playerState != enPlayerState_Attack1 &&
			m_playerState != enPlayerState_Attack2 &&
			m_playerState != enPlayerState_Attack3 &&
			m_playerState != enPlayerState_MagicAttack &&
			m_playerState != enPlayerState_HitDamage1 &&
			m_playerState != enPlayerState_HitDamage2 &&
			m_playerState != enPlayerState_Down1 &&
			m_playerState != enPlayerState_Down2 &&
			m_playerState != enPlayerState_Down3 &&
			m_playerState != enPlayerState_Clear;
	}
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);
	//�ړ�����
	void Move();
	//��]�����B
	void Rotation();
	//�U������
	void Attack();
	/// <summary>
    ///�U���Ƃ̓����蔻�菈���B
    /// </summary>
	void Collision();
    //�U���̍ۂ̓����蔻��p�̃R���W�������쐬����B
	void MakeAttackCollision();
	void MakeAttackCollision2();
	//���ʂ̃X�e�[�g�J�ڏ����B
	void ProcessCommonStateTransition();
	/// <summary>
	/// �ҋ@�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// �����X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// ����X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	/// �U���X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// ��_���[�W�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessHitDamageStateTransition();
	/// <summary>
    /// �_�E���X�e�[�g�̑J�ڏ����B
    /// </summary>
	void ProcessDownStateTransition();

	//�A�j���[�V�����̍Đ�
	void PlayAnimation();
	// �A�j���[�V�����C�x���g�p�̊֐��B
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//�X�e�[�g�Ǘ��B
	void ManageState();
	void Moji();
	void Timer();

	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
    /// �ړ����x�����Z�B
    /// </summary>
    /// <param name="addMoveSpeed">���Z����ړ����x�B</param>
	void AddMoveSpeed(const Vector3& addMoveSpeed)
	{
		m_moveSpeed += addMoveSpeed;
	}
	/// <summary>
    /// �L�����N�^�[�R���g���[���[���擾�B
    /// </summary>
    /// <returns>�L�����N�^�[�R���g���[���[�B</returns>
	CharacterController& GetCharacterController()
	{
		return m_characterController;
	}
	const int GetPlayerhp() const
	{
		return m_playerhp;
	}
	enum EnPlayerState {
		enPlayerState_Idle,					//�ҋ@�B
		enPlayerState_Walk,					//�����B
		enPlayerState_Run,					//����B
		enPlayerState_Attack1,				//�U���B
		enPlayerState_Attack2,				//�U���B
		enPlayerState_Attack3,				//�U���B
		enPlayerState_MagicAttack,			//���@�U���B
		enPlayerState_HitDamage1,		    //�_���\�W�󂯂��B
		enPlayerState_HitDamage2,	    	//�_���\�W�󂯂��B
		enPlayerState_Down1,				//HP��0�B
		enPlayerState_Down2,				//HP��0�B
		enPlayerState_Down3,				//HP��0�B
		enPlayerState_Clear					//�N���A�[�B
	};

private:
	enum EnAnimationClip {
		enAnimationClip_Idle,				//�ҋ@�A�j���[�V�����B	
		enAnimationClip_Walk,				//�����A�j���[�V�����B
		enAnimationClip_Run,				//����A�j���[�V�����B
		enAnimationClip_Attack1,			//�U���A�j���[�V�����B
		enAnimationClip_Attack2,			//�U���A�j���[�V�����B
		enAnimationClip_Attack3,			//�U���A�j���[�V�����B
		enAnimationClip_MagicAttack,		//���@�U���A�j���[�V�����B
		enAnimationClip_HitDamage1,			//��_���[�W�A�j���[�V�����B
		enAnimationClip_HitDamage2,			//��_���[�W�A�j���[�V�����B
		enAnimationClip_Down1,				//HP��0�B
		enAnimationClip_Down2,				//HP��0�B
		enAnimationClip_Down3,				//HP��0�B
		enAnimationClip_PushLever,			//���o�[�������A�j���[�V�����B
		enAnimationClip_Winner,				//�����A�j���[�V�����B
		enAnimationClip_Num,				//�A�j���[�V�����̐��B
	};

	AnimationClip m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	FontRender m_fontRender;                          //�����̕`��
	FontRender m_fontRender2;                         //�����̕`��
	FontRender m_fontRender3;                         //�����̕`��
	FontRender m_fontRender4;                         //�����̕`��
	FontRender m_fontRender5;                         //�����̕`��
	FontRender m_fontRender6;                         //�����̕`��
	EnPlayerState m_playerState = enPlayerState_Idle;	//�v���C���[�̃X�e�[�g(���)��\���ϐ��B

	CharacterController m_characterController;       //�L�����N�^�[�R���g���[���[�B
	SphereCollider m_sphereCollider;		         //�~�^�̃R���C�_�[�B
	Vector3 m_position;			                     //���W�B
	Vector3 m_position2;			                 //���W2�B
	Vector3 m_moveSpeed=Vector3::Zero;				 //�ړ����x�B
	Vector3 m_scale;                                 //�傫���B
	Quaternion m_rotation;                           //�N�E�H�[�^�j�I��
	ModelRender m_modelRender;                       //���f�������_�[
	ModelRender m_modelRender2;                      //���f�������_�[2
	Vector3 m_forward;                               //�L�����N�^�[�̑O�����̃x�N�g��

	float whitetimer = 0.0f;
	float blacktimer = 0.0f;
	float m_attacktimer = 0.0f;
	float m_hitdamagetimer = 0.0f;
	bool m_isUnderAttack = false;					//�U�����Ȃ�true�B
	bool m_isUnderAttack2 = false;					//�U�����Ȃ�true�B
	int m_hitdamagestate = false;                   //��_������������true�ɂ���B
	int m_attackstate = false;
	int hitdamagecooltime = false;                 //��_���[�W���Ȃ�true�B�����łȂ��Ȃ�false�B
	int m_LeftHandId = -1;                         //�{�[����ID�B
	int m_RightHandId = -1;                        //�{�[����ID�B
	int m_playerhp = 5;//�v���C���[��HP
	int player_attackmotion = 0;//1���ʏ�1,2���ʏ�2,0���ʏ�3
};