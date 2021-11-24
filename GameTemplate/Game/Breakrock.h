#pragma once


class  Breakrock : public IGameObject
{
public:
    Breakrock();
    ~Breakrock();
    bool Start();
    void Update();                                    //�X�V����
    void Render(RenderContext& rc);                  //�`��֐��B
    void Create();
    void GetCreate();
    void Break();
	/// <summary>
    /// ���W��ݒ肷��B
    /// </summary>
    /// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// �傫����ݒ肷��B
	/// </summary>
	/// <param name="scale">�傫���B</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// ��]��ݒ肷��B
	/// </summary>
	/// <param name="rotation">��]�B</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
private:
	enum EnAnimationClip {
		enAnimationClip_Break
	};
    ModelRender             m_rock;                       //���f�������_�\�B 
    Vector3					m_position;				   	  //���W�B
    Vector3					m_scale;			          //�傫���B
    Quaternion				m_rotation;				      //��]�B
    PhysicsStaticObject		m_physicsStaticObject;	      //�ÓI�����I�u�W�F�N�g�B
	AnimationClip m_animationClips[1];	                  //�A�j���[�V�����N���b�v�̐��B

    CollisionObject* m_collisionObject;

    float m_timer = 0.0f;
    int createstate = false;//true����������Ă�Bfalse����������ĂȂ��B
};