#include "stdafx.h"
#include "Title.h"
#include "Game.h"

bool Title::Start()
{
	spriteRender.Init("Assets/sprite/Title.dds", 1920.0f, 1080.0f);

	return true;
}

Title::Title()
{
}

Title::~Title()
{
}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_game = NewGO<Game>(0, "game");
		DeleteGO(this);
	}
}

void Title::Render(RenderContext& rc)
{
	spriteRender.Draw(rc);                 //ƒ^ƒCƒgƒ‹‚Ì•`‰æ
}