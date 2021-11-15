#pragma once
class Enemy : public IGameObject
{
public:
    Enemy();
    ~Enemy();
    bool Start();
    void Update();                                    //�X�V����
    void Render(RenderContext& rc);                  //�`��֐��B
    void Move();                                    //�ړ�����
    void Rotation();                               //��]����
    void ProcessEnemyHit();                        //�U���̓�����������̏���

    CharacterController m_characterController;                 //�L�����N�^�[�R���g���[���[�B
    ModelRender m_enemy;                                      //���f�������_�[
    //SphereCollider m_sphereCollider;		                //�~�^�̃R���C�_�[�B
    Vector3					m_position;			   		  //���W�B
    Vector3					m_scale;			         //�傫���B
    Vector3                 m_moveSpeed;                   //�ړ����x�B
    Vector3                 m_positionlength;             //�ʒu�̒���
    Vector3                 m_playerposition;            //�v���C���[�̈ʒu
    Vector3					m_forward = Vector3::AxisZ; //�G�l�~�[�̐��ʃx�N�g���B
    Quaternion				m_rotation;			     //��]�B
    PhysicsStaticObject		m_physicsStaticObject;  //�ÓI�����I�u�W�F�N�g�B

private:
    CollisionObject* m_collisionObject;
};
