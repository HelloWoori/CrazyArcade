#include "stdafx.h"
#include "mainGame.h"

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

//초기화는 여기서 한다
HRESULT MainGame::init()
{
    GameNode::init(true);

    return S_OK;
}

//메모리 해제는 여기서
void MainGame::release()
{
    /*
    예시
    _alphaImage->release();
    SAFE_DELETE(_alphaImage);
    */
    //_playBgImg->release();
    //SAFE_DELETE(_playBgImg);
    GAMEOBJMANGER->releaseObj();
    GameNode::release();
}


//계속 무한반복 되어야 할 것들은 여기서
void MainGame::update()
{
    //참고할것: PtInRect
    GameNode::update();
	SOUNDMANAGER->update();
    SCENEMANAGER->updateCurrentScene();
    GAMEOBJMANGER->updateObj();
}

//그려주는 건 여기서
void MainGame::render()
{
    //GameNode::render();
    //흰색 빈 비트맵
    PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
    // ===========================================================

    SCENEMANAGER->renderCurrentScene(getMemDC());
    GAMEOBJMANGER->renderObj(getMemDC());
    //Text(15, 870, 51, "마우스좌표: " + to_string(ptMouse.x) + ", " + to_string(ptMouse.y), WHITE)(getMemDC());
    TIMEMANAGER->render(getMemDC());

    // ===========================================================
    // 백버퍼에 내용을 HDC에 그린다(이것도 지우지 마시오)
    this->getBackBuffer()->render(getHDC());
}
