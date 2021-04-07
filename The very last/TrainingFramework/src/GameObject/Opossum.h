#pragma once 
#include "Sprite2D.h"
#include "ResourceManagers.h"
#include "SpriteAnimation.h";
#include "Player.h"
#include "FlatForm.h"
#include <math.h>

extern int screenWidth;

class Player;
class Flatform;

class Opossum : public Sprite2D
{

public:
	Opossum(std::shared_ptr<Models> model, GLint x, GLint y);
	~Opossum();

	int GetxSpeed() {
		return m_xSpeed;
	}

	int GetScore()
	{
		return m_iScore;
	}

	std::shared_ptr<SpriteAnimation> GetAnimation() {
		return m_pAnimation;
	}

	bool GetAcvite() {
		return m_isActive;
	}

	void Update(GLfloat deltatime);

	void Detect(std::shared_ptr<Player> player);

	void CheckFlatform(std::shared_ptr<Flatform> flatform);
	
	bool GetInAir() {
		return m_isInAir;
	}

	bool CheckAlive() {
		return m_isAlive;
	}

	void Death() {
		m_isActive = false;
		m_isAlive = false;
	}

	void Move(GLfloat deltatime);

	float GetTimeToDraw()
	{
		return m_timeToDraw;
	}
	bool CheckInScreen()
	{
		Vector2 pos = GetAnimation()->Get2DPosition();
		if (pos.x + 25 < 0 || pos.x - 25 > screenWidth)
		{
			return false;
		}
		return true;
	}
private:

	std::shared_ptr<SpriteAnimation> m_pAnimation;

	std::shared_ptr<SpriteAnimation> m_pLeft;

	std::shared_ptr<SpriteAnimation> m_pRight;

	std::shared_ptr<SpriteAnimation> m_pDeath;

	int m_iScore;

	int m_xSpeed;

	int m_ySpeed;

	bool m_isActive;

	bool m_isAlive;

	int m_xDistance;

	int m_Direction;

	bool m_isInAir;

	float m_timeToDraw;

	Vector2 m_vPosition;
};
