#include "stdafx.h"
#include "MoveFloor.h"
#include "Movefloorwall.h"
#include "stdafx.h"
#include "Player.h"

MoveFloorWall::MoveFloorWall()
{

}

MoveFloorWall::~MoveFloorWall()
{
}

bool MoveFloorWall::Start()
{
	m_modelRender.Init("Assets/modelData/stage/movefloorwall.tkm");
	//m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
	//PhysicsStaticObjectÇèâä˙âªÅB
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	return true;
}

void MoveFloorWall::Update()
{
	Move();
	m_modelRender.Update();
	if (FindGO<MoveFloor>("movefloor")->GetMoveFloor() == true)
	{
		m_physicsStaticObject.SetPosition(FindGO<MoveFloor>("movefloor")->GetPosition());
	}
	else if (FindGO<MoveFloor>("movefloor")->GetMoveFloor() == false)
	{
		//çÑëÃÇÃîjä¸
		m_physicsStaticObject.Release();
	}
}

void MoveFloorWall::Move()
{
	m_modelRender.SetPosition(FindGO<MoveFloor>("movefloor")->GetPosition());
}

void MoveFloorWall::Render(RenderContext& rc)
{
	if (FindGO<MoveFloor>("movefloor")->GetMoveFloor() == true)
	{
		m_modelRender.Draw(rc);
	}
}