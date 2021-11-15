#pragma once


class Wall : public IGameObject
{
public:

    Wall();
    ~Wall();
    bool Start();
    void Update();                                    //�X�V����
    void Render(RenderContext& rc);                  //�`��֐��B
    void Create();
    void GetWorld();

    ModelRender             m_wall;                       //���f�������_�\�B 
    Vector3					m_position;					//���W�B
    Vector3					m_scale;			       //�傫���B
    Quaternion				m_rotation;				  //��]�B
    PhysicsStaticObject		m_physicsStaticObject;	 //�ÓI�����I�u�W�F�N�g�B

    int worldstate = 0;//�O����,�P����
    int createstate = false;//true����������Ă�Bfalse����������ĂȂ��B
};
