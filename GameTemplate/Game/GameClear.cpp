#include "stdafx.h"
#include "GameClear.h"
#include "Title.h"
#include "Game.h"

bool GameClear::Start()
{
    spriteRender.Init("Assets/sprite/gameclear.dds", 1920.0f, 1080.0f);
    return true;
}

GameClear::GameClear()
{
}

GameClear::~GameClear()
{
}

void GameClear::Update()
{
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        NewGO<Title>(0, "title");
        DeleteGO(this);
    }
}

void GameClear::Render(RenderContext& rc)
{
    //‰æ‘œ‚ğ•`‰æ‚·‚éB
    spriteRender.Draw(rc);
}