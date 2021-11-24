#pragma once


class  Breakrock : public IGameObject
{
public:
    Breakrock();
    ~Breakrock();
    bool Start();
    void Update();                                    //更新処理
    void Render(RenderContext& rc);                  //描画関数。
    void Create();
    void GetCreate();
    void Break();
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
	enum EnAnimationClip {
		enAnimationClip_Break
	};
    ModelRender             m_rock;                       //モデルレンダ―。 
    Vector3					m_position;				   	  //座標。
    Vector3					m_scale;			          //大きさ。
    Quaternion				m_rotation;				      //回転。
    PhysicsStaticObject		m_physicsStaticObject;	      //静的物理オブジェクト。
	AnimationClip m_animationClips[1];	                  //アニメーションクリップの数。

    CollisionObject* m_collisionObject;

    float m_timer = 0.0f;
    int createstate = false;//trueが生成されてる。falseが生成されてない。
};