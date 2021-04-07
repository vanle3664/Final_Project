#include "Item.h"

extern int xSpeed;

Item::Item(std::shared_ptr<Models> model, std::shared_ptr<Texture> texture, GLint x, GLint y, int score)
{
	m_pModel = model;

	auto shader = ResourceManagers::GetInstance()->GetShader("Animation");
	m_pActive = std::make_shared<SpriteAnimation>(model, shader, texture, 7, 0.1f);
	m_pActive->SetSize(20, 20);

	texture = ResourceManagers::GetInstance()->GetTexture("feedback");
	m_pFeedback = std::make_shared<SpriteAnimation>(model, shader, texture, 4, 0.2f);
	m_pFeedback->SetSize(30, 30);

	m_pAnimation = m_pActive;
	m_pAnimation->Set2DPosition(x, y);

	m_vPosition = m_pAnimation->Get2DPosition();
	m_iScore = score;

	m_isFed = false;
	m_timeToDraw = 0;
}

Item::~Item()
{
}

void Item::Update(GLfloat deltatime)
{
	m_vPosition = m_pAnimation->Get2DPosition();
	m_vPosition.x += deltatime * xSpeed;

	if (m_isFed)
	{
		m_pAnimation = m_pFeedback;
		m_timeToDraw += deltatime;
	}
	GetAnimation()->Set2DPosition(m_vPosition);
	GetAnimation()->Update(deltatime);
}
