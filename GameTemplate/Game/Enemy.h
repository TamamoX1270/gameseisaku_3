#pragma once
class Enemy : public IGameObject
{
public:
    Enemy();
    ~Enemy();
    bool Start();
    void Update();                                    //更新処理
    void Render(RenderContext& rc);                  //描画関数。
    void Move();                                    //移動処理
    void Rotation();                               //回転処理
    void ProcessEnemyHit();                        //攻撃の当たった判定の処理

    CharacterController m_characterController;                 //キャラクターコントローラー。
    ModelRender m_enemy;                                      //モデルレンダー
    //SphereCollider m_sphereCollider;		                //円型のコライダー。
    Vector3					m_position;			   		  //座標。
    Vector3					m_scale;			         //大きさ。
    Vector3                 m_moveSpeed;                   //移動速度。
    Vector3                 m_positionlength;             //位置の長さ
    Vector3                 m_playerposition;            //プレイヤーの位置
    Vector3					m_forward = Vector3::AxisZ; //エネミーの正面ベクトル。
    Quaternion				m_rotation;			     //回転。
    PhysicsStaticObject		m_physicsStaticObject;  //静的物理オブジェクト。

private:
    CollisionObject* m_collisionObject;
};
