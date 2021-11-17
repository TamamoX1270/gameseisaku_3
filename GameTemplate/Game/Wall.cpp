#include "stdafx.h"
#include "Wall.h"
#include "Player.h"

Wall::Wall()
{

}

Wall::~Wall()
{


}
bool  Wall::Start()
{
	m_wall.Init("Assets/modelData/stage/woodwall.tkm");
	m_wall.SetPosition(m_position);
	m_wall.SetRotation(m_rotation);
	m_wall.SetScale(m_scale);
	m_wall.SetScale(Vector3::One * 20.0f);
	m_wall.Update();

	return true;
}

void Wall::GetWorld()
{
	if (FindGO<Player>("player")->GetWorldState() == 0 && createstate == false)
	{
		createstate = true;
		Create();
	}
	else if (FindGO<Player>("player")->GetWorldState() == 1)
	{
		m_physicsStaticObject.Release();
		createstate = false;
	}
}

void Wall::Create()
{
	if (createstate == true)
	{
		
		//PhysicsStaticObjectを初期化。
		m_physicsStaticObject.CreateFromModel(
			m_wall.GetModel(),
			m_wall.GetModel().GetWorldMatrix());
	}
}

void  Wall::Update()
{
	GetWorld();
	//モデルの更新処理。
	m_wall.Update();
}

void  Wall::Render(RenderContext& rc)
{
	if (FindGO<Player>("player")->GetWorldState() == 0)
	{
		m_wall.Draw(rc);    //モデルを描画する。
	}
}