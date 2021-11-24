#pragma once
class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();
	bool Start();
	void Render(RenderContext& rc);
	/// <summary>
    /// 座標を設定する。
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
	ModelRender m_backGround;                    //モデルレンダ―。
	ModelRender m_backGround2;                    //モデルレンダ―。
	Vector3						m_position;						//座標。
	Vector3						m_scale = Vector3::One;			//大きさ。
	Quaternion					m_rotation;						//回転。
	PhysicsStaticObject m_physicsStaticObject;  //静的物理オブジェクト。

	int worldstate = 0;//０が白,１が黒
};