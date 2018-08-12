#include "stdafx.h"
#include "mainGame.h"

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

//�ʱ�ȭ�� ���⼭ �Ѵ�
HRESULT MainGame::init()
{
    GameNode::init(true);

    return S_OK;
}

//�޸� ������ ���⼭
void MainGame::release()
{
    /*
    ����
    _alphaImage->release();
    SAFE_DELETE(_alphaImage);
    */
    //_playBgImg->release();
    //SAFE_DELETE(_playBgImg);
    GAMEOBJMANGER->releaseObj();
    GameNode::release();
}


//��� ���ѹݺ� �Ǿ�� �� �͵��� ���⼭
void MainGame::update()
{
    //�����Ұ�: PtInRect
    GameNode::update();
	SOUNDMANAGER->update();
    SCENEMANAGER->updateCurrentScene();
    GAMEOBJMANGER->updateObj();
}

//�׷��ִ� �� ���⼭
void MainGame::render()
{
    //GameNode::render();
    //��� �� ��Ʈ��
    PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
    // ===========================================================

    SCENEMANAGER->renderCurrentScene(getMemDC());
    GAMEOBJMANGER->renderObj(getMemDC());
    //Text(15, 870, 51, "���콺��ǥ: " + to_string(ptMouse.x) + ", " + to_string(ptMouse.y), WHITE)(getMemDC());
    TIMEMANAGER->render(getMemDC());

    // ===========================================================
    // ����ۿ� ������ HDC�� �׸���(�̰͵� ������ ���ÿ�)
    this->getBackBuffer()->render(getHDC());
}
