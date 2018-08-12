#include "stdafx.h"
#include "loginScene.h"

LoginScene::LoginScene()
	: _check(false)
{
    _rc = makeRect(414, 66, 197, 167);
}

LoginScene::~LoginScene()
{
}

void LoginScene::init()
{
	SOUNDMANAGER->play(static_cast<int>(SoundTypeTag::LoginScene), SoundTypeTag::LoginScene);
}

void LoginScene::release()
{
}

void LoginScene::update()
{
	SOUNDMANAGER->repeatPlay(static_cast<int>(SoundTypeTag::LoginScene), SoundTypeTag::LoginScene);
	if (PtInRect(&_rc, ptMouse))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play(static_cast<int>(SoundTypeTag::Click), SoundTypeTag::Click);
			SCENEMANAGER->changeScene(SceneTag::ModeSelect);
		}

		if (!_check)
		{
			_check = true;
			SOUNDMANAGER->play(static_cast<int>(SoundTypeTag::PtInRect), SoundTypeTag::PtInRect);
		}
		
	}
	else
	{
		_check = false;
	}
}

void LoginScene::render(HDC hdc)
{
    IMAGEMANAGER->findImage("loginSceneBg")->render(hdc, 0, 0);
    //Text(15, 10, WINSIZEY - 22, "Login Scene (가장 첫 화면)")(hdc);
    //drawRect(hdc, _rc.left, _rc.top, _rc.right - _rc.left, _rc.bottom - _rc.top);
    //DrawText(hdc, "들어가기", -1, &_rc, DT_CENTER);
}
