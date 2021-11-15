#pragma once


class Wall : public IGameObject
{
public:

    Wall();
    ~Wall();
    bool Start();
    void Update();                                    //更新処理
    void Render(RenderContext& rc);                  //描画関数。
    void Create();
    void GetWorld();

    ModelRender             m_wall;                       //モデルレンダ―。 
    Vector3					m_position;					//座標。
    Vector3					m_scale;			       //大きさ。
    Quaternion				m_rotation;				  //回転。
    PhysicsStaticObject		m_physicsStaticObject;	 //静的物理オブジェクト。

    int worldstate = 0;//０が白,１が黒
    int createstate = false;//trueが生成されてる。falseが生成されてない。
};
