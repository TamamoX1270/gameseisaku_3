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
	/// 移動処理。
	/// </summary>
	void Move();
	Vector3					m_position;
	Vector3					m_firstPosition;
	Vector3                 m_scale;
	Quaternion              m_rotation;
	ModelRender				m_modelRender;
	Player* m_player;
	PhysicsStaticObject		m_physicsStaticObject;	      //静的物理オブジェクト。
	Quaternion			    rotation;					  //クォータニオン。

	FontRender m_fontRender;                              //文字の描画
	FontRender m_fontRender2;                             //文字の描画
	FontRender m_fontRender3;                             //文字の描画

	CollisionObject* m_collisionObject = nullptr;		  //コリジョンオブジェクト。
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