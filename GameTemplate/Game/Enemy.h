#pragma once

class Player;

class Enemy : public IGameObject
{
public:
    Enemy();
    ~Enemy();
    bool Start();
    void Update();                                    //�X�V����
    void Render(RenderContext& rc);                  //�`��֐��B
    void Rotation();                                 //��]����
    void Chase();
    void ProcessEnemyHit();                        //�U���̓�����������̏���
    //�U������
    void Attack();
    /// <summary>
    /// �G�l�~�[�̍U���Ƃ̓����蔻�菈���B
    /// </summary>
    void Collision();
    void MakeAttackCollision2();
    /// <summary>
    /// �v���C���[��T������B
    /// </summary>
    /// <returns>�v���C���[������������true�B</returns>
    const bool SearchPlayer() const;
    //�U���̍ۂ̓����蔻��p�̃R���W�������쐬����B
    void MakeAttackCollision();
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
    //�A�j���[�V�����̍Đ�
    void PlayAnimation();
    // �A�j���[�V�����C�x���g�p�̊֐��B
    void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
    //�X�e�[�g�Ǘ��B
    void ManageState();
    /// <summary>
    /// �ǐՃX�e�[�g�̔w�J�ڏ����B
    /// </summary>
    void ProcessChaseStateTransition();
    /// <summary>
    /// �U���ł��鋗�����ǂ������ׂ�B
    /// </summary>
    /// <returns>�U���ł���Ȃ�true�B</returns>
    const bool IsCanAttack() const;

    void SetPosition(const Vector3& position)
    {
        m_position = position;
    }
    void SetRotation(const Quaternion& rotation)
    {
        m_rotation = rotation;
    }
    const Vector3& GetPosition() const
    {
        return m_position;
    }
    enum EnEnemyState {
        enEnemyState_Idle,					//�ҋ@�B
        enEnemyState_Chase,					//�ǐՁB
        enEnemyState_Attack1,				//�U��1�B
        enEnemyState_Attack2,				//�U��2�B
        enEnemyState_MagicAttack,			//���@�U���B
        enEnemyState_ReceiveDamage,			//��_���[�W�B
        enEnemyState_Down,					//�_�E���B
    };

private:
    enum EnAnimationClip {
        enAnimationClip_Idle,					//�ҋ@�A�j���[�V�����B
        enAnimationClip_Walk,					//�����A�j���[�V�����B
        enAnimationClip_Run,					//����A�j���[�V�����B
        enAnimationClip_Attack1,				//�U���A�j���[�V����1�B
        enAnimationClip_Attack2,				//�U���A�j���[�V����2�B
        enAnimationClip_MagicAttack,			//���@�U���A�j���[�V�����B
        enAnimationClip_Damage,					//��_���[�W�A�j���[�V�����B
        enAnimationClip_Down,					//�_�E���A�j���[�V�����B
        enAnimationClip_Num,					//�A�j���[�V�����̐��B
    };

    AnimationClip				m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
    CharacterController m_charaCon;                                         //�L�����N�^�[�R���g���[���[�B
    ModelRender m_enemy;                                                    //���f�������_�[
    EnEnemyState			m_enemyState = enEnemyState_Idle;		    	//�G�l�~�[�X�e�[�g�B
    Vector3					m_position;			   		    //���W�B
    Vector3					m_scale;			            //�傫���B
    Vector3                 m_moveSpeed;                    //�ړ����x�B
    Vector3                 m_playerposition;               //�v���C���[�̈ʒu
    Vector3					m_forward = Vector3::AxisZ;     //�G�l�~�[�̐��ʃx�N�g���B
    Quaternion				m_rotation;			            //��]�B
    PhysicsStaticObject		m_physicsStaticObject;          //�ÓI�����I�u�W�F�N�g�B
    CollisionObject* m_collisionObject;
    Player* m_player = nullptr;

    bool m_isUnderAttack = false;					        //�U�����Ȃ�true�B
    int m_attackstate = false;
    int m_enemyattackmotion = 0;//1���ʏ�1,0���ʏ�2					     	//�v���C���[�B
    float						m_chaseTimer = 0.0f;        //�ǐՃ^�C�}�[�B
    float						m_idleTimer = 0.0f;		    //�ҋ@�^�C�}�[�B
    int m_LeftHandId = -1;                                  //�{�[����ID�B
    int m_RightHandId = -1;                                 //�{�[����ID�B
};
