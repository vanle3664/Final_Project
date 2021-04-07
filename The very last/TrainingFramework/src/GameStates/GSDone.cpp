#include "GSDone.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

extern int screenHeight;
extern int screenWidth;
extern int score;
extern int highScore[];
extern bool isMute;
GSDone::GSDone()
{
}

GSDone::~GSDone()
{
	HighScore();
	ResourceManagers::GetInstance()->PauseSounds("win");
}

void GSDone::Init()
{
	
	ResourceManagers::GetInstance()->PlaySounds("win", true);

	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("back2");

	m_background = std::make_shared<Sprite2D>(model, shader, texture);
	m_background->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_background->SetSize(screenWidth, screenHeight);

	//back to menu
	texture = ResourceManagers::GetInstance()->GetTexture("button_home");
	std::shared_ptr<GameButton> m_button = std::make_shared<GameButton>(model, shader, texture);
	m_button->Set2DPosition(450, 25);
	m_button->SetOnClick([]()
	{
		ResourceManagers::GetInstance()->PlaySounds("menu", true);
		GameStateMachine::GetInstance()->PopState();
		GameStateMachine::GetInstance()->PopState();
	});
	m_listButton.push_back(m_button);


	//button replay
	texture = ResourceManagers::GetInstance()->GetTexture("button_replay");
	m_button = std::make_shared<GameButton>(model, shader, texture);
	m_button->Set2DPosition(400, 25);
	m_button->SetOnClick([]()
	{
		GameStateMachine::GetInstance()->PopState();
		GameStateMachine::GetInstance()->PopState();
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Play);
	});
	m_listButton.push_back(m_button);

	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("GAME");
	m_text = std::make_shared<Text>(shader, font, "You Win!!", TEXT_COLOR::RED, 2.0);
	m_text->Set2DPosition(Vector2(150, 100));

	m_score = std::make_shared<Text>(shader, font, "Score "+std::to_string(score), TEXT_COLOR::RED, 1.5);
	m_score->Set2DPosition(Vector2(160, 160));

	shader = ResourceManagers::GetInstance()->GetShader("Animation");
	texture = ResourceManagers::GetInstance()->GetTexture("player_idle");
	m_animation = std::make_shared<SpriteAnimation>(model, shader, texture, 4, 0.1);
	m_animation->Set2DPosition(220, 182);
	m_animation->SetSize(52, 52);

	//texture = ResourceManagers::GetInstance()->GetTexture("player_idle_left1");
	//m_animation1 = std::make_shared<SpriteAnimation>(model, shader, texture, 4, 0.1);
	//m_animation1->Set2DPosition(260, 182);
	//m_animation1->SetSize(52, 52);

	shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	texture = ResourceManagers::GetInstance()->GetTexture("largeground");
	std::shared_ptr<Sprite2D> obj = std::make_shared<Sprite2D>(model, shader, texture);
	obj->SetSize(960, 112);
	obj->Set2DPosition(screenWidth * 0.5, 264);
	m_listSprite.push_back(obj);

	//texture = ResourceManagers::GetInstance()->GetTexture("tree");
	//obj = std::make_shared<Sprite2D>(model, shader, texture);
	//obj->SetSize(105, 93);
	//obj->Set2DPosition(100, 162);
	//m_listSprite.push_back(obj);

	//texture = ResourceManagers::GetInstance()->GetTexture("house");
	//obj = std::make_shared<Sprite2D>(model, shader, texture);
	//obj->SetSize(83, 108);
	//obj->Set2DPosition(400, 154);
	//m_listSprite.push_back(obj);
}

void GSDone::Exit()
{
}

void GSDone::Pause()
{
}

void GSDone::Resume()
{
}

void GSDone::HandleEvents()
{
}

void GSDone::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSDone::HighScore()
{
	for (int i = 0; i < 3; i++)
	{
		if (score >= highScore[i])
		{
			int t = highScore[i];
			highScore[i] = score;
			score = t;
		}
	}

	fstream f;                      // (2)
	f.open("highscore.txt", ios::out); // (2)

	for (int i = 0; i < 3; i++)
	{

		f << highScore[i] << endl;
	}                        // (3)

	f.close();
}

void GSDone::Mute()
{
}

void GSDone::Sound()
{
}

void GSDone::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	for (auto it : m_listButton)
	{
		(it)->HandleTouchEvents(x, y, bIsPressed);
		if ((it)->IsHandle()) break;
	}
}

void GSDone::Update(float deltaTime)
{
	//m_animation->Update(deltaTime);

	//m_animation1->Update(deltaTime);

	for (auto obj : m_listSprite)
	{
		obj->Update(deltaTime);
	}

	m_background->Update(deltaTime);
	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
	m_text->Update(deltaTime);
	m_score->Update(deltaTime);
}

void GSDone::Draw()
{
	m_background->Draw();
	for (auto it : m_listButton)
	{
		it->Draw();
	}
	m_text->Draw();
	m_score->Draw();
	for (auto obj : m_listSprite)
	{
		obj->Draw();
	}
	//m_animation->Draw();
	//m_animation1->Draw();
}

