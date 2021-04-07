#include "GSCredit.h"

extern int screenHeight;
extern int screenWidth;

GSCredit::GSCredit()
{
}

GSCredit::~GSCredit()
{
}

void GSCredit::Init()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("back2");

	m_background = std::make_shared<Sprite2D>(model, shader, texture);
	m_background->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_background->SetSize(screenWidth, screenHeight);

	texture = ResourceManagers::GetInstance()->GetTexture("button_back");
	m_button = std::make_shared<GameButton>(model, shader, texture);
	m_button->Set2DPosition(450, 25);
	m_button->SetOnClick([]()
	{
		//ResourceManagers::GetInstance()->PlaySounds("menu", true);
		GameStateMachine::GetInstance()->PopState();
	});
	
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("GAME");
	m_text = std::make_shared<Text>(shader, font, "how to play", TEXT_COLOR::RED, 2.0);
	m_text->Set2DPosition(Vector2(100, 50));

	std::shared_ptr<Font> font2 = ResourceManagers::GetInstance()->GetFont("times");
	m_t1 = std::make_shared<Text>(shader, font2, "1. Use ->,<- or d,a to move", TEXT_COLOR::WHILE, 1.0);
	m_t1->Set2DPosition(Vector2(80, 100));

	m_t2 = std::make_shared<Text>(shader, font2, "| or w to jump", TEXT_COLOR::WHILE, 1.0);
	m_t2->Set2DPosition(Vector2(80, 160));

	m_t3 = std::make_shared<Text>(shader, font2, "2. Die if getting hit", TEXT_COLOR::WHILE, 1.0);
	m_t3->Set2DPosition(Vector2(80, 220));

	m_t4 = std::make_shared<Text>(shader, font2, "3. Jump higher to survive", TEXT_COLOR::WHILE, 1.0);
	m_t4->Set2DPosition(Vector2(80, 280));
}

void GSCredit::Exit()
{
}

void GSCredit::Pause()
{
}

void GSCredit::Resume()
{
}

void GSCredit::HandleEvents()
{
}

void GSCredit::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSCredit::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	m_button->HandleTouchEvents(x, y, bIsPressed);
}

void GSCredit::Update(float deltaTime)
{
	m_background->Update(deltaTime);
	m_button->Update(deltaTime);
	m_text->Update(deltaTime);
	m_t1->Update(deltaTime);
	m_t2->Update(deltaTime);
	m_t3->Update(deltaTime);
	m_t4->Update(deltaTime);
}

void GSCredit::Draw()
{
	m_background->Draw();
	m_button->Draw();
	m_text->Draw();
	m_t1->Draw();
	m_t2->Draw();
	m_t3->Draw();
	m_t4->Draw();
}

void GSCredit::Mute()
{
}

void GSCredit::Sound()
{
}
