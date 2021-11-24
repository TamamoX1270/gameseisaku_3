#include "stdafx.h"
#include "BackGround.h"
#include "Game.h"

BackGround::BackGround()
{

}

BackGround::~BackGround()
{

}
bool BackGround::Start()
{

	m_backGround.Init("Assets/modelData/stage/background.tkm");
	m_backGround.SetScale(m_scale);
	m_backGround.Update();

	m_backGround2.Init("Assets/modelData/stage/background.tkm");
	m_backGround2.SetScale(m_scale);
	m_backGround2.Update();

	//PhysicsStaticObject‚ð‰Šú‰»B
	m_physicsStaticObject.CreateFromModel(
	m_backGround.GetModel(),
	m_backGround.GetModel().GetWorldMatrix());
	//“–‚½‚è”»’è‚Ì‰ÂŽ‹‰»B
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	return true;
}
void BackGround::Render(RenderContext& rc)
{
	if (FindGO<Game>("game")->GetWorldState() == 0)
	{
		m_backGround.Draw(rc);
	}
	if (FindGO<Game>("game")->GetWorldState() == 1)
	{
		m_backGround2.Draw(rc);
	}
}