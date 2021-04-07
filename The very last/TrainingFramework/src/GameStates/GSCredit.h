#pragma once
#include "gamestatebase.h"
#include "GameButton.h"

class GSCredit :
	public GameStateBase
{
public:
	GSCredit();
	~GSCredit();

	void	Init();
	void	Exit();

	void	Pause();
	void	Resume();

	void	HandleEvents();
	void	HandleKeyEvents(int key, bool bIsPressed);
	void	HandleTouchEvents(int x, int y, bool bIsPressed);
	void	Update(float deltaTime);
	void	Draw();
	void	Mute();

	void	Sound();

private:
	std::shared_ptr<GameButton> m_button;
	std::shared_ptr<Text> m_text;
	std::shared_ptr<Text> m_t1;
	std::shared_ptr<Text> m_t2;
	std::shared_ptr<Text> m_t3;
	std::shared_ptr<Text> m_t4;

	std::shared_ptr<Sprite2D> m_background;
};

