#pragma once

class Player;

class Enemy : public IGameObject
{
public:
    Enemy();
    ~Enemy();
    bool Start();
    void Update();                                    //更新処理
    void Render(RenderContext& rc);                  //描画関数。
    void Rotation();                                 //回転処理
    void Chase();
    //攻撃処理
    void Attack();
    /// <summary>
    /// エネミーの攻撃との当たり判定処理。
    /// </summary>
    void Collision();
    void MakeAttackCollision2();
    /// <summary>
    /// プレイヤーを探索する。
    /// </summary>
    /// <returns>プレイヤーが見つかったらtrue。</returns>
    const bool SearchPlayer() const;
    //攻撃の際の当たり判定用のコリジョンを作成する。
    void MakeAttackCollision();
    //共通のステート遷移処理。
    void ProcessCommonStateTransition();
    /// <summary>
    /// 待機ステートの遷移処理。
    /// </summary>
    void ProcessIdleStateTransition();
    /// <summary>
    /// 歩きステートの遷移処理。
    /// </summary>
    void ProcessWalkStateTransition();
    /// <summary>
    /// 走りステートの遷移処理。
    /// </summary>
    void ProcessRunStateTransition();
    /// <summary>
    /// 攻撃ステートの遷移処理。
    /// </summary>
    void ProcessAttackStateTransition();
    /// <summary>
    /// 被ダメージステートの遷移処理。
    /// </summary>
    void ProcessHitDamageStateTransition();
    /// <summary>
    /// ダウンステートの遷移処理。
    /// </summary>
    void ProcessDownStateTransition();
    //アニメーションの再生
    void PlayAnimation();
    // アニメーションイベント用の関数。
    void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
    //ステート管理。
    void ManageState();
    /// <summary>
    /// 追跡ステートの背遷移処理。
    /// </summary>
    void ProcessChaseStateTransition();
    /// <summary>
    /// 攻撃できる距離かどうか調べる。
    /// </summary>
    /// <returns>攻撃できるならtrue。</returns>
    const bool IsCanAttack() const;
    /// <summary>
    /// HPを設定する。
    /// </summary>
    /// <param name="hp">HP。</param>
    void SetHP(const int hp)
    {
        m_hp = hp;
    }
    void SetPosition(const Vector3& position)
    {
        m_position = position;
    }
    void SetRotation(const Quaternion& rotation)
    {
        m_rotation = rotation;
    }
    void SetScale(const Vector3& scale)
    {
        m_scale = scale;
    }
    const Vector3& GetPosition() const
    {
        return m_position;
    }
    enum EnEnemyState {
        enEnemyState_Idle,					//待機。
        enEnemyState_Chase,					//追跡。
        enEnemyState_Attack1,				//攻撃1。
        enEnemyState_Attack2,				//攻撃2。
        enEnemyState_HitDamage1,			//被ダメージアニメーション。
        enEnemyState_HitDamage2,			//被ダメージアニメーション2。
        enEnemyState_Down1,                 //ダウンアニメーション。
        enEnemyState_Down2,                 //ダウンアニメーション2。
        enEnemyState_MagicAttack,			//魔法攻撃。
        enEnemyState_ReceiveDamage,			//被ダメージ。
        enEnemyState_Down,					//ダウン。
    };

private:
    enum EnAnimationClip {
        enAnimationClip_Idle,					//待機アニメーション。
        enAnimationClip_Walk,					//歩きアニメーション。
        enAnimationClip_Run,					//走りアニメーション。
        enAnimationClip_Attack1,				//攻撃アニメーション1。
        enAnimationClip_Attack2,				//攻撃アニメーション2。
        enAnimationClip_MagicAttack,			//魔法攻撃アニメーション。
        enAnimationClip_HitDamage1,				//被ダメージアニメーション。
        enAnimationClip_HitDamage2,				//被ダメージアニメーション。
        enAnimationClip_Down1,                  //ダウンアニメーション。
        enAnimationClip_Down2,                  //ダウンアニメーション2。
        enAnimationClip_Down,					//ダウンアニメーション。
        enAnimationClip_Num,					//アニメーションの数。
    };

    AnimationClip				m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
    CharacterController m_charaCon;                                         //キャラクターコントローラー。
    ModelRender m_enemy;                                                    //モデルレンダー
    ModelRender m_enemy2;                                                   //モデルレンダー2
    EnEnemyState			m_enemyState = enEnemyState_Idle;		    	//エネミーステート。
    Vector3					m_position;			   		    //座標。
    Vector3					m_scale;			            //大きさ。
    Vector3                 m_moveSpeed;                    //移動速度。
    Vector3                 m_playerposition;               //プレイヤーの位置
    Vector3					m_forward = Vector3::AxisZ;     //エネミーの正面ベクトル。
    Quaternion				m_rotation;			            //回転。
    PhysicsStaticObject		m_physicsStaticObject;          //静的物理オブジェクト。
    CollisionObject* m_collisionObject;
    Player* m_player = nullptr;

    FontRender m_fontRender;                                //文字の描画

    bool m_isUnderAttack = false;					        //攻撃中ならtrue。
    int m_attackstate = false;                              //攻撃中ならtrue。
    int hitdamagecooltime = false;                          //被ダメージ中ならtrue。そうでないならfalse。
    int m_enemyattackmotion = 0;                            //1が通常1,0が通常2
    float m_chaseTimer = 0.0f;                              //追跡タイマー。
    float m_idleTimer = 0.0f;		                        //待機タイマー。
    int m_LeftHandId = -1;                                  //ボーンのID。
    int m_RightHandId = -1;                                 //ボーンのID。
    int m_hp = 0;
};
