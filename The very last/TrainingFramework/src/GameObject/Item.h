#pragma once
#include "Sprite2D.h"
#include "SpriteAnimation.h"
#include "ResourceManagers.h"

extern int screenWidth;
class Item :
	public Sprite2D
{
public:

	Item(std::shared_ptr<Models> model, std::shared_ptr<Texture> texture, GLint x, GLint y, int score);
	~Item();
	
	std::shared_ptr<SpriteAnimation> GetAnimation() {
		return m_pAnimation;
	}

	void Update(GLfloat deltatime);

	void IsFed() {
		m_isFed = true;
	}

	int GetScore()
	{
		return m_iScore;
	}

	float GetTimeToDraw() {
		return m_timeToDraw;
	}

	bool ChechIsFed() {
		return m_isFed;
	}

	bool CheckInScreen()
	{
		Vector2 pos = GetAnimation()->Get2DPosition();
		if (pos.x + 50 < 0 || pos.x - 50 > screenWidth)
		{
			return false;
		}
		return true;
	}

protected:

	std::shared_ptr<SpriteAnimation> m_pAnimation;

	std::shared_ptr<SpriteAnimation> m_pFeedback;
	
	std::shared_ptr<SpriteAnimation> m_pActive;
	
	bool m_isFed;

	int m_iScore;

	float m_timeToDraw;

	Vector2 m_vPosition;
};

