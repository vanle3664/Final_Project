#include "GSHighScore.h"

extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine


extern int screenHeight;
extern int screenWidth;
extern int ySpeed;
extern int highScore[];



GSHighScore::GSHighScore()
{
}

GSHighScore::~GSHighScore()
{
}

void GSHighScore::Init()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("back2");

	std::shared_ptr<Sprite2D> background = std::make_shared<Sprite2D>(model, shader, texture);
	background->Set2DPosition(screenWidth / 2, screenHeight / 2);
	background->SetSize(screenWidth, screenHeight);
	m_background.push_back(background);

	texture = ResourceManagers::GetInstance()->GetTexture("largeground");
	background = std::make_shared<Sprite2D>(model, shader, texture);
	background->Set2DPosition(screenWidth / 2, 264);
	background->SetSize(960, 112);
	m_background.push_back(background);


	//button menu
	texture = ResourceManagers::GetInstance()->GetTexture("button_home");
	std::shared_ptr<GameButton> m_button = std::make_shared<GameButton>(model, shader, texture);
	m_button->Set2DPosition(450, 25);
	m_button->SetOnClick([]()
	{
		//ResourceManagers::GetInstance()->PlaySounds("menu", true);
		GameStateMachine::GetInstance()->PopState();
	});
	m_listButton.push_back(m_button);

	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("GAME");
	m_text = std::make_shared<Text>(shader, font, "High score", TEXT_COLOR::RED, 2.0);
	m_text->Set2DPosition(Vector2(120, 100));

	m_textNo1 = std::make_shared<Text>(shader, font, "Score " + std::to_string(highScore[0]), TEXT_COLOR::RED, 1);
	m_textNo1->Set2DPosition(Vector2(180, 150));

	m_textNo2 = std::make_shared<Text>(shader, font, "Score " + std::to_string(highScore[1]), TEXT_COLOR::RED, 1);
	m_textNo2->Set2DPosition(Vector2(180, 180));

	m_textNo3 = std::make_shared<Text>(shader, font, "Score " + std::to_string(highScore[2]), TEXT_COLOR::RED, 1);
	m_textNo3->Set2DPosition(Vector2(180, 210));

	//shader = ResourceManagers::GetInstance()->GetShader("Animation");
	//texture = ResourceManagers::GetInstance()->GetTexture("player_crouch");
	//m_animation = std::make_shared<SpriteAnimation>(model, shader, texture, 2, 0.1);
	//m_animation->Set2DPosition(80, 182);
	//m_animation->SetSize(52, 52);

	//texture = ResourceManagers::GetInstance()->GetTexture("frog_idle");
	//m_animation1 = std::make_shared<SpriteAnimation>(model, shader, texture, 4, 0.1);
	//m_animation1->Set2DPosition(400, 178);
	//m_animation1->SetSize(60, 60);
}

void GSHighScore::Exit()
{
}

void GSHighScore::Pause()
{
}

void GSHighScore::Resume()
{
}

void GSHighScore::HandleEvents()
{
}

void GSHighScore::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSHighScore::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	for (auto it : m_listButton)
	{
		(it)->HandleTouchEvents(x, y, bIsPressed);
		if ((it)->IsHandle()) break;
	}
}

void GSHighScore::Update(float deltaTime)
{
	//m_animation->Update(deltaTime);
	//m_animation1->Update(deltaTime);
	for (auto it : m_background)
	{
		it->Update(deltaTime);
	}
	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
	m_text->Update(deltaTime);
	m_textNo1->Update(deltaTime);
	m_textNo2->Update(deltaTime);
	m_textNo3->Update(deltaTime);
}

void GSHighScore::Draw()
{
	for (auto it : m_background)
	{
		it->Draw();
	}
	for (auto it : m_listButton)
	{
		it->Draw();
	}
	m_text->Draw();
	m_textNo1->Draw();
	m_textNo2->Draw();
	m_textNo3->Draw();
	//m_animation->Draw();
	//m_animation1->Draw();
}

void GSHighScore::Mute()
{
}

void GSHighScore::Sound()
{
}
