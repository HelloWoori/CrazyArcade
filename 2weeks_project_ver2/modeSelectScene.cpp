#include "stdafx.h"
#include "modeSelectScene.h"
#include "lobbyScene.h"
ModeSelectScene::ModeSelectScene()
	: _check(false)
{
    _rc1 = makeRect(377, 579, 265, 80);
    //_rc2 = makeRectCenter(WINSIZEX / 2, WINSIZEY / 2 + 80, 400, 100);//���͸�� (���߿� �����ʿ�)
}

ModeSelectScene::~ModeSelectScene()
{
}

void ModeSelectScene::init()
{
	//������ �ִ� �κ���� ������ ���� �����Ѵ�
	SCENEMANAGER->deleteScene(SceneTag::Lobby);
	LobbyScene* lobby = new LobbyScene;
	SCENEMANAGER->registerScene(SceneTag::Lobby, lobby);
}

void ModeSelectScene::release()
{
}

void ModeSelectScene::update()
{
	SOUNDMANAGER->repeatPlay(static_cast<int>(SoundTypeTag::LoginScene), SoundTypeTag::LoginScene);
    if (PtInRect(&_rc1, ptMouse))
    {	
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play(static_cast<int>(SoundTypeTag::Click), SoundTypeTag::Click);
			SOUNDMANAGER->stop(static_cast<int>(SoundTypeTag::LoginScene));
			
			SCENEMANAGER->changeScene(SceneTag::Lobby, vector<int>{static_cast<int>(ModeTypeTag::TwoPlayer)});
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

	/*TODO: ���� ��� �����ϸ� �ּ� ����*/
    //if (PtInRect(&_rc2, ptMouse))
    //{
	//	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	//	{
	//		SOUNDMANAGER->play(1, SoundTypeTag::Click);
	//		SCENEMANAGER->changeScene(SceneTag::Lobby, vector<int>{static_cast<int>(ModeTypeTag::Monster)});
	//	}
	//}
	//else
	//{
	//	SOUNDMANAGER->play(1, SoundTypeTag::PtInRect);
	//}
}

void ModeSelectScene::render(HDC hdc)
{
    IMAGEMANAGER->findImage("modeSelectSceneBg")->render(hdc, 0, 0);
    //Text(15, 10, WINSIZEY - 22, "Mode Select Scene (��� ���� ȭ��)")(hdc);
    //drawRect(hdc, _rc1.left, _rc1.top, _rc1.right - _rc1.left, _rc1.bottom - _rc1.top);
    //DrawText(hdc, "2�� ���", -1, &_rc1, DT_CENTER);
	/*TODO: ���� ��� �����ϸ� �ּ� ����*/
	//drawRect(hdc, _rc2.left, _rc2.top, _rc2.right - _rc2.left, _rc2.bottom - _rc2.top);
    //DrawText(hdc, "���� ���", -1, &_rc2, DT_CENTER);
}
