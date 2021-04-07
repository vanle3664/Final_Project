/*
#include "GSIntro.h"
#include "GSMenu.h"
*/
#include "GSPlay.h"
#include "GSIntro.h"
#include "GSMenu.h"
#include "GSCredit.h"
#include "GSGameOver.h"
#include "GSDone.h"
#include "GameStatebase.h"
#include "GSHighScore.h"

std::shared_ptr<GameStateBase> GameStateBase::CreateState(StateTypes stt)
{
	std::shared_ptr<GameStateBase> gs = nullptr;
	switch (stt)
	{
	case STATE_INVALID:
		break;
	case STATE_Intro:
		gs = std::make_shared<GSIntro>();
		break;
	case STATE_Menu:
		gs = std::make_shared<GSMenu>();
		break;
	case STATE_Play:
		gs = std::make_shared<GSPlay>();
		break;
	case STATE_Credit:
		gs = std::make_shared<GSCredit>();
		break;
	case STATE_Done:
		gs = std::make_shared<GSDone>();
		break;
	case STATE_Finish:
		gs = std::make_shared<GSGameOver>();
		break;;
	case STATE_High:
		gs = std::make_shared<GSHighScore>();
		break;
	default:
		break;
	}
	return gs;
}