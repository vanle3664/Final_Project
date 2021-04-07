#include "GSPlay.h"

#include "Shaders.h"
#include "Texture.h"
#include "Models.h"
#include "Camera.h"
#include "Font.h"
#include "Sprite2D.h"
#include "Sprite3D.h"
#include "Text.h"
#include "SpriteAnimation.h"
#include "Player.h"
#include "Opossum.h"
#include "Item.h"



extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine
extern int xSpeed;
extern int ySpeed;
extern int score;
extern bool isMute;

GSPlay::GSPlay()
{
	m_isPause = false;
	m_isPass = false;
	m_timeToDraw = 0;
}


GSPlay::~GSPlay()
{
	ResourceManagers::GetInstance()->PauseSounds("play");
}


void GSPlay::Init()
{

	xSpeed = 0;
	score = 0;

	ResourceManagers::GetInstance()->PlaySounds("play", true);

	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("back2");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	
	//BackGround
	shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	std::shared_ptr<Sprite2D> bg1 = std::make_shared<Sprite2D>(model, shader, texture);
	bg1->Set2DPosition(screenWidth / 2, screenHeight / 2);
	bg1->SetSize(screenWidth, screenHeight);
	m_listBackGround.push_back(bg1);

	bg1 = std::make_shared<Sprite2D>(model, shader, texture);
	bg1->Set2DPosition(screenWidth * 1.5 - 1, screenHeight * 0.5);
	bg1->SetSize(screenWidth, screenHeight);
	m_listBackGround.push_back(bg1);

	//text game title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("GAME");
	m_score = std::make_shared<Text>(shader, font, "score " + std::to_string(score), TEXT_COLOR::RED, 1.0);
	m_score->Set2DPosition(Vector2(5, 25));
	

	//new Player
	m_Player = std::make_shared<Player>(model, shader, texture);
	

	//new Opossum
	SetOpossum(model);

	//new Frog
	//SetFrog(model);

	//ground
	shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	SetMap(model, shader);
	
	//item
	SetItem(model);


	//button
	texture = ResourceManagers::GetInstance()->GetTexture("button_back");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(450, 25);
	button->SetOnClick([]() 
	{
		ResourceManagers::GetInstance()->PlaySounds("menu", true);
		GameStateMachine::GetInstance()->PopState();
	});

	m_listButton.push_back(button);
	texture = ResourceManagers::GetInstance()->GetTexture("button_play");button = std::make_shared<GameButton>(model, shader, texture);
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(350, 25);
	button->SetActive(false);
	button->SetOnClick([]()
	{
		GameStateMachine::GetInstance()->CurrentState()->Resume();
	});

	m_listButton.push_back(button);
	texture = ResourceManagers::GetInstance()->GetTexture("button_pause"); button = std::make_shared<GameButton>(model, shader, texture);
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(350, 25);
	button->SetOnClick([]()
	{
		GameStateMachine::GetInstance()->CurrentState()->Pause();
	});
	m_listButton.push_back(button);
	
	//mute button
	texture = ResourceManagers::GetInstance()->GetTexture("button_mute");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(400, 25);
	button->SetActive(!isMute);
	button->SetOnClick([]() {
		//ResourceManagers::GetInstance()->PauseSounds("menu");
		GameStateMachine::GetInstance()->CurrentState()->Mute();
	});
	m_listButton.push_back(button);

	//sound button
	texture = ResourceManagers::GetInstance()->GetTexture("button_sound");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(400, 25);
	button->SetActive(isMute);
	button->SetOnClick([]() {
		//ResourceManagers::GetInstance()->PauseSounds("menu");
		GameStateMachine::GetInstance()->CurrentState()->Sound();

	});
	m_listButton.push_back(button);
	
}

void GSPlay::Exit()
{

}


void GSPlay::Pause()
{
	m_isPause = true;
	m_listButton[1]->SetActive(true);
	m_listButton[2]->SetActive(false);
	ResourceManagers::GetInstance()->m_Soloud.setPauseAll(true);
}

void GSPlay::Resume()
{
	m_isPause = false;
	m_listButton[1]->SetActive(false);
	m_listButton[2]->SetActive(true);
	ResourceManagers::GetInstance()->m_Soloud.setPauseAll(false);
}


void GSPlay::HandleEvents()
{
	for (auto gr : m_listFlatform)
	{
		Vector2 pos = gr->Get2DPosition();
		if (gr->GetIsInScreen())
		{
			m_Player->CheckFlatform(gr);
			if (!m_Player->GetInAir())
			{
				break;
			}
		}
	}
	
	for (auto item : m_listItem) {
		if (!item->ChechIsFed()&&item->CheckInScreen()) {
			
			m_Player->CheckItem(item);
		
		}
	}

	for (auto op : m_listOpossum) 
	{
		if (op->CheckAlive()&& op->GetAcvite())
		{
			//if (op->GetAcvite())
			{
				m_Player->CheckCollision(op);

				for (auto gr : m_listFlatform)
				{
					if (gr->GetIsInScreen())
					{
						op->CheckFlatform(gr);
						if (!op->GetInAir())
						{
							break;
						}
					}
				}
			}
		}
	}

	
}

void GSPlay::HandleKeyEvents(int key, bool bIsPressed)
{
	m_Player->HandleKeyEvents(key, bIsPressed);
}

void GSPlay::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	for (auto it : m_listButton)
	{
		if (it->GetActive()) {
			(it)->HandleTouchEvents(x, y, bIsPressed);
			if ((it)->IsHandle()) break;
		}
	}
}

void GSPlay::Update(float deltaTime)
{
	//printf("%f\n", deltaTime);
	m_pHome->CheckInScreen();
	if (m_pHome->GetIsInScreen()) {
		if (CheckPass())
		{
			ResourceManagers::GetInstance()->PauseSounds("play");

			GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Done);
		}
	}	
	if(!m_isPause)
	{
		m_score->setText("score " + std::to_string(score));

		m_Player->Update(deltaTime);

		if (m_Player->CheckAlive())
		{
			
			m_pHome->Update(deltaTime);

			for (auto gr : m_listFlatform)
			{
				gr->CheckInScreen();
				gr->Update(deltaTime);
			}

			/*for (auto bg : m_listBackGround) {
				Vector2 pos = bg->Get2DPosition();
				pos.x = pos.x - 50 * deltaTime;
				if (pos.x < -screenWidth / 2) {
					pos.x = screenWidth * 1.5 - 1;
				}
				bg->Set2DPosition(pos);
			}*/

			for (auto opossum : m_listOpossum)
			{
				if (opossum->GetTimeToDraw() < 0.5)
				{
					opossum->Detect(m_Player);
					opossum->Update(deltaTime);
				}
				else
				{
					opossum.reset();
				}
			}

			//for (auto frog : m_listFrog)
			//{
			//	if (frog->GetTimeToDraw() < 0.5)
			//	{
			//		frog->Update(deltaTime);
			//	}
			//	else
			//	{
			//		frog.reset();
			//	}
			//}

 			for (auto item : m_listItem)
			{
				if (item->GetTimeToDraw() < 0.5)
				{
					item->Update(deltaTime);
				}
				else
				{
					item.reset();
				}
				
			}
			HandleEvents();
		}

		else {

			ResourceManagers::GetInstance()->PauseSounds("play");

			GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Finish);

		}
	}
	

	for (auto it : m_listButton)
	{
		if (it->GetActive())
		{
			it->Update(deltaTime);
		}
	}
	m_score->Update(deltaTime);
}

void GSPlay::Draw()
{

	for (auto bg : m_listBackGround) {
		bg->Draw();
	}

	for (auto gr : m_listFlatform)
	{
		gr->CheckInScreen();
		if (gr->GetIsInScreen())
		{
			gr->Draw();
		}
		
	}

	for (auto opossum : m_listOpossum)
	{
		if (opossum->GetTimeToDraw() < 0.5 && opossum->CheckInScreen())
		{
			opossum->GetAnimation()->Draw();
		}
	}


	
	for (auto item : m_listItem)
	{
		if (item->GetTimeToDraw() < 0.5 && item->CheckInScreen())
		{
			item->GetAnimation()->Draw();
		}
	}

	m_pHome->Draw();

	m_Player->GetAnimation()->Draw();

	for (auto obj : m_listButton)
	{
		if (obj->GetActive())
		{
			obj->Draw();
		}
	}

	m_score->Draw();
}

bool GSPlay::CheckPass()
{
	Vector2 pPos = m_Player->GetAnimation()->Get2DPosition();
	Vector2 hPos = m_pHome->Get2DPosition();

	if (sqrt((pPos.x - hPos.x)*(pPos.x - hPos.x) + (pPos.y - hPos.y)*(pPos.y - hPos.y)) < 54) 
	{
		return true;
	}
	return false;

}

void GSPlay::SetMap(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader)
{
	//home
	auto texture = ResourceManagers::GetInstance()->GetTexture("house");
	m_pHome = std::make_shared<Home>(model, shader, texture, 6500, 154, 87, 108);

	//shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	//std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("GAME");
	//m_win = std::make_shared<Text>(shader, font, "You win!!", TEXT_COLOR::RED, 2.0);
	//m_win->Set2DPosition(Vector2(6500, 154));

	//ground
	texture = ResourceManagers::GetInstance()->GetTexture("ground");
	std::shared_ptr<Flatform> m_Ground = std::make_shared<Flatform>(model, shader, texture, 200, 280, 336, 80);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 3000, 280, 336, 80);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 5000, 280, 336, 80);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 5500, 280, 336, 80);
	m_listFlatform.push_back(m_Ground);

	//land
	texture = ResourceManagers::GetInstance()->GetTexture("largeground");
	m_Ground = std::make_shared<Flatform>(model, shader, texture, 960, 264, 960, 112);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 2000, 264, 960, 112);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 6500, 264, 960, 112);
	m_listFlatform.push_back(m_Ground);


	//floater
	texture = ResourceManagers::GetInstance()->GetTexture("floater");

	m_Ground = std::make_shared<Flatform>(model, shader, texture,900, 150, 48, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 1000, 150, 48, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 1100, 150, 48, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 1900, 150, 48, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 2100, 80, 48, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 2300, 150, 48, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 2550, 150, 48, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 2700, 150, 48, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 3250, 180, 48, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 3500, 150, 48, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 3450, 250, 48, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 3600, 240, 48, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 3600, 100, 48, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 3800, 240, 48, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 3800, 80, 48, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4050, 200, 48, 16);
	m_listFlatform.push_back(m_Ground);
	
	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4250, 200, 48, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4350, 180, 48, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4500, 100, 48, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4600, 240, 48, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4750, 180, 48, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 5300, 150, 48, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 5250, 250, 48, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 5800, 150, 48, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 6000, 150, 48, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 6200, 150, 48, 16);//
	m_listFlatform.push_back(m_Ground);
	//small floater
	texture = ResourceManagers::GetInstance()->GetTexture("smallfloater");

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 2000, 120, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 2200, 120, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 3370, 170, 32, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 3700, 200, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 3700, 140, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 3900, 200, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 3900, 50, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4000, 180, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4000, 80, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4150, 240, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4300, 80, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4400, 100, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4400, 240, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4500, 240, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4550, 300, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4700, 240, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4700, 90, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 5200, 200, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 5200, 50, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 5300, 80, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 5400, 80, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 5400, 180, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 5500, 100, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 5600, 50, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 5700, 150, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 5650, 180, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 5900, 80, 32, 16);//
	m_listFlatform.push_back(m_Ground);

	//big floater
	texture = ResourceManagers::GetInstance()->GetTexture("largefloater");
	m_Ground = std::make_shared<Flatform>(model, shader, texture, 3300, 290, 112, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 3700, 290, 112, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4150, 140, 112, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4000, 290, 112, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4300, 290, 112, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 4550, 180, 112, 16);
	m_listFlatform.push_back(m_Ground);

	m_Ground = std::make_shared<Flatform>(model, shader, texture, 5800, 240, 112, 16);
	m_listFlatform.push_back(m_Ground);

}

void GSPlay::SetOpossum(std::shared_ptr<Models> model) {

	std::shared_ptr <Opossum> m_Opossum = std::make_shared<Opossum>(model, 800, 150);
	m_listOpossum.push_back(m_Opossum);

	//m_Opossum = std::make_shared<Opossum>(model, 1300, 150);
	//m_listOpossum.push_back(m_Opossum);

	//m_Opossum = std::make_shared<Opossum>(model, 1700, 150);
	//m_listOpossum.push_back(m_Opossum);

	//m_Opossum = std::make_shared<Opossum>(model, 1900, 150);
	//m_listOpossum.push_back(m_Opossum);

	//m_Opossum = std::make_shared<Opossum>(model, 2100, 150);
	//m_listOpossum.push_back(m_Opossum);

	//m_Opossum = std::make_shared<Opossum>(model, 2500, 150);
	//m_listOpossum.push_back(m_Opossum);

	//m_Opossum = std::make_shared<Opossum>(model, 3100, 150);
	//m_listOpossum.push_back(m_Opossum);

	//m_Opossum = std::make_shared<Opossum>(model, 3200, 150);
	//m_listOpossum.push_back(m_Opossum);

	//m_Opossum = std::make_shared<Opossum>(model, 5100, 150);
	//m_listOpossum.push_back(m_Opossum);

	//m_Opossum = std::make_shared<Opossum>(model, 4900, 150);
	//m_listOpossum.push_back(m_Opossum);

	//m_Opossum = std::make_shared<Opossum>(model, 6300, 150);
	//m_listOpossum.push_back(m_Opossum);

	//m_Opossum = std::make_shared<Opossum>(model, 6450, 150);
	//m_listOpossum.push_back(m_Opossum);

	//m_Opossum = std::make_shared<Opossum>(model, 6500, 150);
	//m_listOpossum.push_back(m_Opossum);

	//m_Opossum = std::make_shared<Opossum>(model, 6700, 150);
	//m_listOpossum.push_back(m_Opossum);
}



void GSPlay::Mute()
{
	isMute = true;
	/*ResourceManagers::GetInstance()->m_Soloud.setPauseAll(isMute);*/
	ResourceManagers::GetInstance()->m_Soloud.setGlobalVolume(0);
	m_listButton[3]->SetActive(!isMute);
	m_listButton[4]->SetActive(isMute);
}

void GSPlay::Sound()
{
	isMute = false;
	/*ResourceManagers::GetInstance()->m_Soloud.setPauseAll(isMute);*/
	ResourceManagers::GetInstance()->m_Soloud.setGlobalVolume(1);
	m_listButton[3]->SetActive(!isMute);
	m_listButton[4]->SetActive(isMute);
}

void GSPlay::SetItem(std::shared_ptr<Models> model)
{
	//gem
	auto texture = ResourceManagers::GetInstance()->GetTexture("gem");
	std::shared_ptr<Item> m_Gem = std::make_shared<Item>(model, texture, 200, 200, 20);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 400, 150, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 950, 60, 20);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 1050, 60, 20);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 2000, 60, 20);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 2200, 60, 20);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 2625, 60, 20);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 3475, 190, 20);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 3800, 120, 20);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4000, 130, 20);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4150, 180, 20);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4400, 170, 20);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4550, 280, 20);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4700, 130, 20);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 5700, 130, 20);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 5100, 130, 20);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 5400, 130, 20);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 3700, 120, 20);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 3500, 60, 20);//
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4550, 100, 20);//
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 5900, 60, 20);//
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 6100, 60, 20);//
	m_listItem.push_back(m_Gem);

	//cherry
	texture = ResourceManagers::GetInstance()->GetTexture("cherry");

	m_Gem = std::make_shared<Item>(model, texture, 100, 200, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 300, 200, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 700, 170, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 800, 170, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 900, 170, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 1100, 170, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 1200, 170, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 1300, 170, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 1600, 170, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 1700, 170, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 1800, 170, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 1900, 170, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 2000, 170, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 2100, 170, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 2200, 170, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 2300, 170, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 2400, 170, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 2900, 200, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 3000, 200, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 3100, 200, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 3205, 100, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 3500, 100, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 3600, 200, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 3600, 80, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 3700, 180, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 3800, 200, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 3900, 30, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 3900, 180,10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4050, 180, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4250, 180, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4350, 130, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4000, 50, 010);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4300, 50, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4400, 70, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4500, 80, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4500, 220, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4600, 220, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4700, 160, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4700, 50, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 4900, 200, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 5000, 200, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 5100, 200, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 5200, 30, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 5300, 130, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 5250, 30, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 5300, 50,10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 5500, 50, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 5600, 30, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 5200, 180, 10);
	m_listItem.push_back(m_Gem);

	m_Gem = std::make_shared<Item>(model, texture, 6000, 80, 10);
	m_listItem.push_back(m_Gem);
}
