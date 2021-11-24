#pragma once
#pragma once

class Player;

class MoveFloor : public IGameObject
{
public:
	MoveFloor();
	~MoveFloor();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	/// <summary>
	/// ���W��ݒ�B
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
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const int GetMoveFloor() const
	{
		return m_MoveFloorState;
	}
private:
	/// <summary>
	/// �ړ������B
	/// </summary>
	void Move();
	Vector3					m_position;
	Vector3					m_firstPosition;
	Vector3                 m_scale;
	Quaternion              m_rotation;
	ModelRender				m_modelRender;
	Player* m_player;
	PhysicsStaticObject		m_physicsStaticObject;	      //�ÓI�����I�u�W�F�N�g�B
	Quaternion			    rotation;					  //�N�H�[�^�j�I���B

	FontRender m_fontRender;                              //�����̕`��
	FontRender m_fontRender2;                             //�����̕`��
	FontRender m_fontRender3;                             //�����̕`��

	CollisionObject* m_collisionObject = nullptr;		  //�R���W�����I�u�W�F�N�g�B
	int		m_MoveFloorState = false;

	int foolstate = 0;
	float m_timer = 0.0f;
	enum enMovingFloorState
	{
		enMovingFloor_Y,
		enMovingFloor_Z,
		enMovingFloor_None
	};
	enMovingFloorState		m_MovingFloorState = enMovingFloor_Y;

};