#pragma once
class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();
	bool Start();
	void Render(RenderContext& rc);

	ModelRender m_backGround;                    //���f�������_�\�B
	ModelRender m_backGround2;                    //���f�������_�\�B
	PhysicsStaticObject m_physicsStaticObject;  //�ÓI�����I�u�W�F�N�g�B

	int worldstate = 0;//�O����,�P����
};