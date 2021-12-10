#pragma once

class GameClear : public IGameObject
{
public:
    GameClear();
    ~GameClear();

    bool Start();
    void Update();
    void Render(RenderContext& rc);

private:
    SpriteRender spriteRender;   //�X�v���C�g�����_�\�B
};