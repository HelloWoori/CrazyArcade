#include "stdafx.h"
#include "lobbyScene.h"
#include "playScene.h"
LobbyScene::LobbyScene()
    : _mapType(MapTypeTag::Not)
    , _mode(ModeTypeTag::Not)
	, _isMapSet(false)
	, _check1(false)
	, _check2(false)
{
    _rc = makeRect(666, 641, 250, 74);
    _rcBack = makeRect(16,743, 40, 32);
}

LobbyScene::~LobbyScene()
{
}

void LobbyScene::init()
{
	//기존에 있던 플레이씬을 지우고 새로운 플레이씬을 생성한다
	SCENEMANAGER->deleteScene(SceneTag::Play);
	PlayScene* play = new PlayScene;
	SCENEMANAGER->registerScene(SceneTag::Play, play);
}

void LobbyScene::release()
{
}

void LobbyScene::update()
{
	SOUNDMANAGER->repeatPlay(static_cast<int>(SoundTypeTag::LobbyScene), SoundTypeTag::LobbyScene);
	//TODO: 버튼에 따라
    //캐릭터 선택도 넘겨준다
	if (!_isMapSet)
	{
		_mapType = MapTypeTag::Forest; //우선 포레스트로 셋
	}

	if (PtInRect(&_rc, ptMouse))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play(static_cast<int>(SoundTypeTag::Click), SoundTypeTag::Click);		
			SOUNDMANAGER->stop(static_cast<int>(SoundTypeTag::LobbyScene));
			SOUNDMANAGER->play(static_cast<int>(SoundTypeTag::GameStart), SoundTypeTag::GameStart);
			SCENEMANAGER->changeScene(SceneTag::Play, vector<int>{static_cast<int>(_mode), static_cast<int>(_mapType)});
		}
		if (!_check1)
		{
			_check1 = true;
			SOUNDMANAGER->play(static_cast<int>(SoundTypeTag::PtInRect), SoundTypeTag::PtInRect);
		}
	}
	else
	{
		_check1 = false;
	}

	if (PtInRect(&_rcBack, ptMouse))
    {
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play(static_cast<int>(SoundTypeTag::Click), SoundTypeTag::Click);
			SOUNDMANAGER->stop(static_cast<int>(SoundTypeTag::LobbyScene));
			
			SCENEMANAGER->changeScene(SceneTag::ModeSelect);
		}
		if (!_check2)
		{
			_check2 = true;
			SOUNDMANAGER->play(static_cast<int>(SoundTypeTag::PtInRect), SoundTypeTag::PtInRect);
		}
    }
	else
	{
		_check2 = false;
	}
    
}

void LobbyScene::render(HDC hdc)
{
    IMAGEMANAGER->findImage("lobbySceneBg")->render(hdc, 0, 0);
    //Text(15, 10, WINSIZEY - 22, "Lobby Scene")(hdc);
    //if (_mode == ModeTypeTag::TwoPlayer)
    //{
    //    Text(15, 100, WINSIZEY - 22, "(2인 모드)")(hdc);
    //}
    //else if (_mode == ModeTypeTag::Monster)
    //{
    //    Text(15, 100, WINSIZEY - 22, "(몬스터 모드)")(hdc);
    //}
    //drawRect(hdc, _rc.left, _rc.top, _rc.right - _rc.left, _rc.bottom - _rc.top);
    //DrawText(hdc, "시   작", -1, &_rc, DT_CENTER);

    //drawRect(hdc, _rcBack.left, _rcBack.top, _rcBack.right - _rcBack.left, _rcBack.bottom - _rcBack.top);
    //DrawText(hdc, "뒤로", -1, &_rcBack, DT_CENTER);
}

void LobbyScene::handleArgs(vector<int> args)
{
    _mode = static_cast<ModeTypeTag>(args.at(0));
}
