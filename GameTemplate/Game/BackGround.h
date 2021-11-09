#pragma once
class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();
	bool Start();
	void Render(RenderContext& rc);

	ModelRender m_backGround;                    //モデルレンダ―。
	ModelRender m_backGround2;                    //モデルレンダ―。
	PhysicsStaticObject m_physicsStaticObject;  //静的物理オブジェクト。

	int worldstate = 0;//０が白,１が黒
};