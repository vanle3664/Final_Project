#include "GSIntro.h"


extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine


extern int screenHeight;
extern int screenWidth;
extern int xSpeed;
extern int score;
GSIntro::GSIntro()
{
	m_timeToDraw = 0;
	xSpeed = 100;
}

GSIntro::~GSIntro()
{
}

void GSIntro::Init()
{

	LoadHighScore();
	ResourceManagers::GetInstance()->PlaySounds("menu", true);

	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("intro_back");

	m_background = std::make_shared<Sprite2D>(model, shader, texture);
	m_background->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_background->SetSize(screenWidth, screenHeight);


	//shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	//std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("GAME");
	//m_text = std::make_shared<Text>(shader, font, "Jungle Adventure", TEXT_COLOR::RED, 2.0);
	//m_text->Set2DPosition(Vector2(100, 100));

	//m_score = std::make_shared<Text>(shader, font, "Score " + std::to_string(score), TEXT_COLOR::RED, 1.5);
	//m_score->Set2DPosition(Vector2(160, 160));

	//shader = ResourceManagers::GetInstance()->GetShader("Animation");
	//texture = ResourceManagers::GetInstance()->GetTexture("player_run");
	//m_animation = std::make_shared<SpriteAnimation>(model, shader, texture, 6, 0.1);
	//m_animation->Set2DPosition(10, 182);
	//m_animation->SetSize(52, 52);

	//texture = ResourceManagers::GetInstance()->GetTexture("player_idle_left1");
	//m_animation1 = std::make_shared<SpriteAnimation>(model, shader, texture, 4, 0.1);
	//m_animation1->Set2DPosition(400, 182);
	//m_animation1->SetSize(52, 52);

	//shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	//texture = ResourceManagers::GetInstance()->GetTexture("largeground");
	//std::shared_ptr<Sprite2D> obj = std::make_shared<Sprite2D>(model, shader, texture);
	//obj->SetSize(960, 112);
	//obj->Set2DPosition(screenWidth * 0.5, 264);
	//m_listSprite.push_back(obj);

	//texture = ResourceManagers::GetInstance()->GetTexture("tree");
	//obj = std::make_shared<Sprite2D>(model, shader, texture);
	//obj->SetSize(105, 93);
	//obj->Set2DPosition(100, 162);
	//m_listSprite.push_back(obj);

	//texture = ResourceManagers::GetInstance()->GetTexture("logo");
	//obj = std::make_shared<Sprite2D>(model, shader, texture);
	//obj->SetSize(62, 44);
	//obj->Set2DPosition(screenWidth * 0.5, 50);
	//m_listSprite.push_back(obj);

	//texture = ResourceManagers::GetInstance()->GetTexture("house");
	//obj = std::make_shared<Sprite2D>(model, shader, texture);
	//obj->SetSize(83, 108);
	//obj->Set2DPosition(400, 154);
	//m_listSprite.push_back(obj);
}

void GSIntro::Exit()
{
}

void GSIntro::Pause()
{
}

void GSIntro::Resume()
{
}

void GSIntro::HandleEvents()
{
}

void GSIntro::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSIntro::HandleTouchEvents(int x, int y, bool bIsPressed)
{
}

void GSIntro::Update(float deltaTime)
{
	//Vector2 pos = m_animation->Get2DPosition();

	//pos.x += xSpeed * deltaTime;
	//m_animation->Set2DPosition(pos.x, pos.y);

	//if (pos.x >= 390) 
	//{
	//	GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Menu);
	//}
	//m_animation->Update(deltaTime);

	//m_animation1->Update(deltaTime);

	m_background->Update(deltaTime);

	m_timeToDraw += deltaTime;


	if (m_timeToDraw > 5)
	{
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Menu);
		m_timeToDraw = 0;
	}
}

void GSIntro::Draw()
{
	m_background->Draw();

}

void GSIntro::LoadHighScore()
{
	printf("load \n");

	//fstream f;                      // (2)
	//f.open("highscore.txt", ios::out); // (2)

	//string data1 = "board: modern c++\n"
	//	"website: www.stdio.vn/modern-cpp\n"
	//	"mo ta: lap trinh c++"; // (3)

	//int data[3] = { 1,2,3 };

	//for (int i = 0; i < 3; i++)
	//{

	//	f << data[i] <<endl;
	//}                        // (3)

	//f.close(); // (4)


	fstream f;
	f.open("highscore.txt", ios::in);

	string data;
	int i = 0;
	string line;
	while (!f.eof())          // (1)
	{
		getline(f, line); // (2)
		data += line + " ";
	}

	f.close();
	while (data.find(" "))
	{
		highScore[i++] = std::stoi(data.substr(0, data.find(" ")));
		data = data.substr(data.find(" ") + 1, data.length());
	}

	for (int i = 0; i < 3; i++)
	{
		printf("%d ", highScore[i]);
	}
}

void GSIntro::Mute()
{
}

void GSIntro::Sound()
{
}
