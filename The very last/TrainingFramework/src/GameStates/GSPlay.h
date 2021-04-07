#pragma once
#include "gamestatebase.h"
#include "GameButton.h"
#include "FlatForm.h"

class Sprite2D;
class Sprite3D;
class Text;
class SpriteAnimation;
class Player;
class Opossum;
class Ground;
class Platform;
class Item;
class Home;
class Frog;

class GSPlay :
	public GameStateBase
{
public:
	GSPlay();
	
	~GSPlay();

	void	Init();
	
	void	Exit();
	
	void	Pause();
	
	void	Resume();

	void	HandleEvents();
	
	void	HandleKeyEvents(int key, bool bIsPressed);

	void	HandleTouchEvents(int x, int y, bool bIsPressed);
	
	void	Update(float deltaTime);
	 
	void	Draw();

	bool	CheckPass();
	
	//void	SetNewPostionForBullet();

	void	SetMap(std::shared_ptr<Models> model, std::shared_ptr<Shaders>);

	void	SetOpossum(std::shared_ptr<Models> model);

	void	SetItem(std::shared_ptr<Models> model);

	void	SetFrog(std::shared_ptr<Models> model);

	void	SetIsPause(bool p) {
		m_isPause = p;
	}

	void	Mute();

	void	Sound();

	std::vector<std::shared_ptr<GameButton>>	m_listButton;

private:

	std::list<std::shared_ptr<Sprite2D>> m_listBackGround;
	
	std::shared_ptr<Text>  m_score;
	
	std::vector < std::shared_ptr<SpriteAnimation>> m_listSpriteAnimations;
	
	std::vector < std::shared_ptr<Sprite2D>> m_listSprite2D;
	
	std::shared_ptr<Player> m_Player;
	
	std::list<std::shared_ptr<Opossum>> m_listOpossum;

	std::list<std::shared_ptr<Frog>> m_listFrog;
	
	std::list<std::shared_ptr<Flatform>> m_listFlatform;

	std::list<std::shared_ptr<Item>> m_listItem;

	std::shared_ptr<Home> m_pHome;

	std::shared_ptr<Text>  m_win;

	bool	m_isPause;

	bool	m_timeToDraw;

	bool	m_isPass;
};


