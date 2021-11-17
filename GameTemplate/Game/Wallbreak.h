#pragma once


class Wallbreak : public IGameObject
{
public:
    Wallbreak();
    ~Wallbreak();
    bool Start();
    void Update();                                    //�X�V����
    void Render(RenderContext& rc);                  //�`��֐��B
    void Create();
    void GetCreate();
    void Break();
private:
    ModelRender             m_wall;                       //���f�������_�\�B 
    Vector3					m_position;					//���W�B
    Vector3					m_scale;			       //�傫���B
    Quaternion				m_rotation;				  //��]�B
    PhysicsStaticObject		m_physicsStaticObject;	 //�ÓI�����I�u�W�F�N�g�B

    CollisionObject* m_collisionObject;

    float m_timer = 0.0f;
    int worldstate = 0;//�O����,�P����
    int createstate = false;//true����������Ă�Bfalse����������ĂȂ��B
};