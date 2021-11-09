#pragma once

class Player : public IGameObject
{
public:
	Player();
	~Player();
	//最初に一回だけ実行
	bool Start();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);
	//移動処理
	void Move();
	//回転処理。
	void Rotation();
	void Moji();
	void Timer();

	FontRender m_fontRender;                         //文字の描画
	FontRender m_fontRender2;                       //文字の描画
	FontRender m_fontRender3;                         //文字の描画
	FontRender m_fontRender4;                         //文字の描画
	FontRender m_fontRender5;                         //文字の描画

	CharacterController characterController;       //キャラクターコントローラー。
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
	int whiteChangestate = true;
	int blackChangestate = true;
	int worldstate = 0;//０が白,１が黒
};