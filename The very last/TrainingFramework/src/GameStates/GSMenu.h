#pragma once
#include "gamestatebase.h"
#include "GameButton.h"
#include "SpriteAnimation.h"
class GSMenu :
	public GameStateBase
{
public:

	GSMenu();

	~GSMenu();
	
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

	void	CheckSound();

private:
	
	std::list<std::shared_ptr<Sprite2D>> m_listBackGround;
	
	std::vector<std::shared_ptr<GameButton>> m_listButton;

	std::list<std::shared_ptr<SpriteAnimation>> m_listAnimation;

	std::shared_ptr<Text> m_Text_gameName;
	
};

