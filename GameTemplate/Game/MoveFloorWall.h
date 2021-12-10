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
	/// 座標を設定。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 大きさを設定する。
	/// </summary>
	/// <param name="scale">大きさ。</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// 回転を設定する。
	/// </summary>
	/// <param name="rotation">回転。</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
private:
	/// <summary>
	/// 移動処理。
	/// </summary>
	void Move();
	Vector3					m_position;
	Vector3                 m_scale;
	Quaternion              m_rotation;
	ModelRender				m_modelRender;
	Player* m_player;
	PhysicsStaticObject		m_physicsStaticObject;	      //静的物理オブジェクト。

	CollisionObject* m_collisionObject = nullptr;		  //コリジョンオブジェクト。
};