#include "Opossum.h"

extern int screenWidth; //need get on Graphic engine
extern int screenHeight;
extern int xSpeed;


Opossum::Opossum(std::shared_ptr<Models> model, GLint x, GLint y)
{
	m_xDistance = 1000;
	m_xSpeed = 160;
	m_ySpeed = 0;
	m_isActive = false;
	m_isAlive = true;
	m_isInAir = true;
	m_timeToDraw = 0;
	m_iScore = 20;

	auto shader = ResourceManagers::GetInstance()->GetShader("Animation");
	auto texture = ResourceManagers::GetInstance()->GetTexture("opossum");
	m_pLeft = std::make_shared<SpriteAnimation>(model, shader, texture, 6, 0.1f);
	m_pLeft->SetSize(54, 42);
	
	texture = ResourceManagers::GetInstance()->GetTexture("opossum_right");
	m_pRight = std::make_shared<SpriteAnimation>(model, shader, texture, 6, 0.1f);
	m_pRight->SetSize(54, 42);

	texture = ResourceManagers::GetInstance()->GetTexture("death");
	m_pDeath = std::make_shared<SpriteAnimation>(model, shader, texture, 6, 0.1f);
	m_pDeath->SetSize(54, 42);

	m_pAnimation = m_pLeft;
	m_pAnimation->Set2DPosition(x, y);
}

Opossum::~Opossum()
{
}

void Opossum::Update(GLfloat deltatime) 
{
	
	m_vPosition = GetAnimation()->Get2DPosition();

	if (m_isAlive)
	{
		Move(deltatime);
	}
	else
	{
		m_vPosition.x += deltatime * xSpeed;
		m_pAnimation = m_pDeath;
		m_timeToDraw += deltatime;
	}

	GetAnimation()->Set2DPosition(m_vPosition);
	GetAnimation()->Update(deltatime);
}

void Opossum::Detect(std::shared_ptr<Player> player)
{
	Vector2 playerPos = player->GetAnimation()->Get2DPosition();
	Vector2 oposumPos = GetAnimation()->Get2DPosition();

	m_xDistance = oposumPos.x - playerPos.x;

	if (m_xDistance != 0 && abs(playerPos.y - oposumPos.y) < 40) {
		m_Direction = m_xDistance / abs(m_xDistance);
	}

	if (abs(m_xDistance) - 27 < screenWidth / 2) {
		m_isActive = true;
	}
	else {
		m_isActive = false;
	}
	if (oposumPos.y - 21 > screenHeight) {
		m_isActive = false;
	}

}

void Opossum::CheckFlatform(std::shared_ptr<Flatform> flatform)
{
	Vector2 pos = GetAnimation()->Get2DPosition();
	Vector2 fPos = flatform->Get2DPosition();
	Vector2 fSize = flatform->GetSize();
	
	if ((pos.y + 21 <= fPos.y - fSize.y * 0.5 + 11) && (pos.y + 21 >= fPos.y - fSize.y * 0.5) && abs(fPos.x - pos.x) < fSize.x * 0.5)
	{
		m_vPosition.y = fPos.y - 0.5 * fSize.y - 21;
		GetAnimation()->Set2DPosition(m_vPosition);
		m_isInAir = false;
	}

	else
	{
		m_isInAir = true;
	}
	if (pos.y + 21 > screenHeight)
	{
		m_isAlive = false;
	}
	
}

void Opossum::Move(GLfloat deltatime)
{
	if (m_Direction >= 0) {
		m_pAnimation = m_pLeft;
	}
	else {
		m_pAnimation = m_pRight;
	}

	if (m_isActive) {
		m_vPosition.x = m_vPosition.x - deltatime * (m_xSpeed * m_Direction - xSpeed);
		if (m_isInAir) {
			m_ySpeed += gravity;
		}
		else {
			m_ySpeed = 0;
		}
		m_vPosition.y += m_ySpeed * deltatime;
	}
	else {
		m_vPosition.x = m_vPosition.x + deltatime * xSpeed;
	}

}
