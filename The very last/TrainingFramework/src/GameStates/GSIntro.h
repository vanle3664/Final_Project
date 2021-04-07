#pragma once
#include "GameStatebase.h"
#include "GameButton.h"
#include "SpriteAnimation.h"
#include <string>
#include <fstream>
#include <iostream>

extern int highScore[];
using namespace std;

class GSIntro :
	public GameStateBase
{
public:
	GSIntro();
	~GSIntro();

	void	Init();
	void	Exit();

	void	Pause();
	void	Resume();

	void	HandleEvents();
	void	HandleKeyEvents(int key, bool bIsPressed);
	void	HandleTouchEvents(int x, int y, bool bIsPressed);
	void	Update(float deltaTime);
	void	Draw();

	void	LoadHighScore();
	
	void	Mute();

	void	Sound();
private:
	std::list<std::shared_ptr<Sprite2D>> m_listSprite;
	std::shared_ptr<Text> m_text;
	std::shared_ptr<Sprite2D> m_background;
	std::shared_ptr<SpriteAnimation> m_animation;
	std::shared_ptr<SpriteAnimation> m_animation1;
	float m_timeToDraw;
};


