#pragma once

class Player;

class MoveFloorWall : public IGameObject
{
public:
	MoveFloorWall();
	~MoveFloorWall();
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
private:
	/// <summary>
	/// �ړ������B
	/// </summary>
	void Move();
	Vector3					m_position;
	Vector3                 m_scale;
	Quaternion              m_rotation;
	ModelRender				m_modelRender;
	Player* m_player;
	PhysicsStaticObject		m_physicsStaticObject;	      //�ÓI�����I�u�W�F�N�g�B

	CollisionObject* m_collisionObject = nullptr;		  //�R���W�����I�u�W�F�N�g�B
};