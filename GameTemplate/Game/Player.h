#pragma once

class Collision;
class Wall;

class Player : public IGameObject
{
public:
	Player();
	~Player();
	//最初に一回だけ実行
	bool Start();
	//動ける状態(ステート)かどうかを取得。
    //動ける状態(ステート)ならtrue。
	bool IsEnableMove() const
	{
		return m_playerState != enPlayerState_Attack1 &&
			m_playerState != enPlayerState_Attack2 &&
			m_playerState != enPlayerState_Attack3 &&
			m_playerState != enPlayerState_MagicAttack &&
			m_playerState != enPlayerState_ReceiveDamage &&
			m_playerState != enPlayerState_Down &&
			m_playerState != enPlayerState_Clear;
	}
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);
	//移動処理
	void Move();
	//回転処理。
	void Rotation();
	//攻撃処理
	void Attack();
    //攻撃の際の当たり判定用のコリジョンを作成する。
	void MakeAttackCollision();
	void MakeAttackCollision2();
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
	void ProcessReceiveDamageStateTransition();

	//アニメーションの再生
	void PlayAnimation();
	// アニメーションイベント用の関数。
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//ステート管理。
	void ManageState();
	void Moji();
	void Timer();

	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	const int GetWorldState() const
	{
		return worldstate;
	}
	const CharacterController&GetCharacon() const
	{
		m_characterController;
	}
	enum EnPlayerState {
		enPlayerState_Idle,					//待機。
		enPlayerState_Walk,					//歩き。
		enPlayerState_Run,					//走る。
		enPlayerState_Attack1,				//攻撃。
		enPlayerState_Attack2,				//攻撃。
		enPlayerState_Attack3,				//攻撃。
		enPlayerState_MagicAttack,			//魔法攻撃。
		enPlayerState_ReceiveDamage,		//ダメ―ジ受けた。
		enPlayerState_Down,					//HPが0。
		enPlayerState_Clear					//クリアー。
	};

private:
	enum EnAnimationClip {
		enAnimationClip_Idle,				//待機アニメーション。	
		enAnimationClip_Walk,				//歩きアニメーション。
		enAnimationClip_Run,				//走りアニメーション。
		enAnimationClip_Attack1,				//攻撃アニメーション。
		enAnimationClip_Attack2,				//攻撃アニメーション。
		enAnimationClip_Attack3,				//攻撃アニメーション。
		enAnimationClip_MagicAttack,		//魔法攻撃アニメーション。
		enAnimationClip_HitDamage,				//被ダメージアニメーション。
		enAnimationClip_Down,				//ダウンアニメーション。
		enAnimationClip_PushLever,			//レバーを押すアニメーション。
		enAnimationClip_Winner,				//勝利アニメーション。
		enAnimationClip_Num,				//アニメーションの数。
	};

	AnimationClip		m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	FontRender m_fontRender;                         //文字の描画
	FontRender m_fontRender2;                       //文字の描画
	FontRender m_fontRender3;                         //文字の描画
	FontRender m_fontRender4;                         //文字の描画
	FontRender m_fontRender5;                         //文字の描画
	EnPlayerState m_playerState = enPlayerState_Idle;	//プレイヤーのステート(状態)を表す変数。

	CharacterController m_characterController;       //キャラクターコントローラー。
	SphereCollider m_sphereCollider;		      //円型のコライダー。
	Vector3 m_position;			                 //座標。
	Vector3 m_position2;			             //座標2。
	Vector3 m_moveSpeed;				        //移動速度。
	Quaternion m_rotation;                  //クウォータニオン
	ModelRender m_modelRender;             //モデルレンダー
	ModelRender m_modelRender2;             //モデルレンダー2
	Vector3 m_forward;                    //キャラクターの前方向のベクトル

	float whitetimer = 0.0f;
	float blacktimer = 0.0f;
	float m_attacktimer = 0.0f;
	bool m_isUnderAttack = false;					//攻撃中ならtrue。
	bool m_isUnderAttack2 = false;					//攻撃中ならtrue。
	int m_attackstate = false;
	int whiteChangestate = true;
	int blackChangestate = true;
	int m_LeftHandId = -1;                   //ボーンのID。
	int m_RightHandId = -1;                    //ボーンのID。
	int worldstate = 0;//０が白,１が黒
	int player_attackmotion = 0;//1が通常1,2が通常2,0が通常3
};