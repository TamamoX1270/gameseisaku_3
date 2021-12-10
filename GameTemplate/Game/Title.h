#pragma once

class Game;

class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	Game* m_game;
	SpriteRender spriteRender;
	ModelRender m_modelRender;                     //ƒ‚ƒfƒ‹ƒŒƒ“ƒ_[
};

