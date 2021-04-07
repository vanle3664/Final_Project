#include "Player.h"


Player::Player(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture)
	: Sprite2D(model, shader, texture)
{
	m_isInAir = false;
	m_xSpeed = 200;
	m_ySpeed = -280;
	m_isRight = true;
	m_isAlive = true;

	shader = ResourceManagers::GetInstance()->GetShader("Animation");
	texture = ResourceManagers::GetInstance()->GetTexture("player_idle");
	m_pIdle = std::make_shared<SpriteAnimation>(model, shader, texture, 4, 0.1f);
	m_pIdle->SetSize(52, 52);

	texture = ResourceManagers::GetInstance()->GetTexture("player_idle_left");
	m_pIdleLeft = std::make_shared<SpriteAnimation>(model, shader, texture, 4, 0.1f);
	m_pIdleLeft->SetSize(52, 52);

	texture = ResourceManagers::GetInstance()->GetTexture("player_run");
	m_pRun = std::make_shared<SpriteAnimation>(model, shader, texture, 6, 0.1f);
	m_pRun->SetSize(52, 52);

	texture = ResourceManagers::GetInstance()->GetTexture("player_run_left");
	m_pRunLeft = std::make_shared<SpriteAnimation>(model, shader, texture, 6, 0.1f);
	m_pRunLeft->SetSize(52, 52);

	texture = ResourceManagers::GetInstance()->GetTexture("player_jump_up");
	m_pJumpUp = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 0.1f);
	m_pJumpUp->SetSize(52, 52);

	texture = ResourceManagers::GetInstance()->GetTexture("player_jump_down");
	m_pJumpDown = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 0.1f);
	m_pJumpDown->SetSize(52, 52);

	texture = ResourceManagers::GetInstance()->GetTexture("player_jump_letf_up");
	m_pJumpLeftUp = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 0.1f);
	m_pJumpLeftUp->SetSize(52, 52);

	texture = ResourceManagers::GetInstance()->GetTexture("player_jump_left_down");
	m_pJumpLeftDown = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 0.1f);
	m_pJumpLeftDown->SetSize(52, 52);

	texture = ResourceManagers::GetInstance()->GetTexture("player_hurt");
	m_pHurt = std::make_shared<SpriteAnimation>(model, shader, texture, 2, 0.1f);
	m_pHurt->SetSize(52, 52);

	m_pAnimation = m_pIdle;

	m_vPosition.x = 240;

	m_vPosition.y = 100;
}


Player::~Player()
{
	m_pAnimation = NULL;
	m_pRun = NULL;
}


int keyPressed = 0;
void Player::HandleKeyEvents(GLbyte key, bool bIsPressed)
{

	if (m_isAlive)
	{
		if (key == KEY_UP || key == KEY_MOVE_FORWORD || key == KEY_SPACE) {
			if (!m_isInAir)
			{
				ResourceManagers::GetInstance()->PlaySounds("jump", false);
				m_isInAir = true;
				m_ySpeed = -300;
			}

		}
		
		
		if (key == KEY_RIGHT || key == KEY_MOVE_RIGHT) {
			m_isRight = true;
			xSpeed = -m_xSpeed;

			if (!bIsPressed)
			{
				xSpeed = 0;
			}

		}

		if (key == KEY_LEFT || key == KEY_MOVE_LEFT) {
			m_isRight = false;
			xSpeed = m_xSpeed;

			if (!bIsPressed)
			{
				xSpeed = 0;
			}
		}
		
		
	}
}


void Player::Run() {

	if (m_isRight) {
		m_pAnimation = m_pRun;
	}
	else {
		m_pAnimation = m_pRunLeft;
	}
	
	
}

void Player::Idle() {
	if (m_isRight) {
		m_pAnimation = m_pIdle;
	}
	else {
		m_pAnimation = m_pIdleLeft;
	}
}

void Player::CheckFlatform(std::shared_ptr<Flatform> flatform) {

	Vector2 pos = GetAnimation()->Get2DPosition();
	Vector2 fPos = flatform->Get2DPosition();
	Vector2 fSize = flatform->GetSize();

	if (pos.y > screenHeight - 10) 
	{
	
		m_isAlive = false;
		
		m_ySpeed = 100;
	}
	
	if((pos.y + 26 <= fPos.y - fSize.y * 0.5 + 16) && (pos.y + 26 >= fPos.y - fSize.y * 0.5 - 2) && (pos.x + 10 >= fPos.x - fSize.x * 0.5 && pos.x - 10 <= fPos.x + fSize.x * 0.5))
	{
		m_vPosition.y = fPos.y - 0.5*fSize.y - 26;
		
		m_isInAir = false;
	}
	else m_isInAir = true;
}

void Player::CheckCollision(std::shared_ptr<Opossum> opossum)
{
	Vector2 pos = GetAnimation()->Get2DPosition();
	Vector2 oPos = opossum->GetAnimation()->Get2DPosition();
	if (oPos.y - pos.y < 26 && abs(pos.x - oPos.x) < 45)
	{
		m_isAlive = false;
	}
	else if (oPos.y - pos.y > 26 && oPos.y - pos.y < 35 && abs(pos.x - oPos.x) < 40)
	{
		ResourceManagers::GetInstance()->PlaySounds("hit", false);
		m_ySpeed = -100;
		score += opossum->GetScore();
		opossum->Death();
	}
}

void Player::CheckItem(std::shared_ptr<Item> item)
{
	Vector2 pos = GetAnimation()->Get2DPosition();
	Vector2 iPos = item->GetAnimation()->Get2DPosition();
	if (sqrt((pos.x - iPos.x)*(pos.x - iPos.x) + (pos.y - iPos.y)*(pos.y - iPos.y)) <= 20) {
		{
			ResourceManagers::GetInstance()->PlaySounds("item", false);
			score += item->GetScore();
			item->IsFed();
		}
	}
}

void Player::Move(GLfloat deltatime)
{
	if (m_isAlive) {
		if (m_ySpeed > 0) {

			if (m_isRight)
				m_pAnimation = m_pJumpDown;
			else
				m_pAnimation = m_pJumpLeftDown;
		}
		else {
			if (m_isRight) {
				m_pAnimation = m_pJumpUp;
			}
			else
				m_pAnimation = m_pJumpLeftUp;
		}

		if (m_isInAir) {

			if (m_ySpeed < 400)
			{
				m_ySpeed += gravity;
			}
		}
		else {
			m_ySpeed = 0;

			if (xSpeed == 0) {
				Idle();
			}
			else {
				Run();
			}
		}
	}

	else
	{
		m_ySpeed = 100;
		m_pAnimation = m_pHurt;
	}
	m_vPosition.y += m_ySpeed * deltatime;
}


void Player::Update(GLfloat deltatime)
{
	
	Move(deltatime);
	GetAnimation()->Update(deltatime);
	m_pAnimation->Set2DPosition(m_vPosition);
}


