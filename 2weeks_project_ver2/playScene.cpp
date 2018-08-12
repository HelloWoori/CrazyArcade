#include "stdafx.h"
#include "playScene.h"
#include "itemFactory.h"

//물풍선관련 맵정보 정의 (처음엔 물풍선이 있는지 여부와, 놓은 바로직후여부 두개다 false)
CanIGo PlayScene::mapArr[BOARD_ROW][BOARD_COL] = { CanIGo() };
//아이템관련 맵정보 정의(처음엔 아이템이 있는지 여부와, 아이템의 종류가 각각 false, Not)
bool PlayScene::isItemArr[BOARD_ROW][BOARD_COL] = { false };

PlayScene::PlayScene()
	: _vvTile(BOARD_ROW, vector<Tile>(BOARD_COL, Tile()))
	, _mapType(MapTypeTag::Not)
	, _startTime(static_cast<int>(TIMEMANAGER->getWorldTime()))
	, _gameWords(nullptr)
	, _startWords(nullptr)
	, _blackBg(nullptr)
	, _gameGrayWords(nullptr)
	, _overWords(nullptr)
	, _setTime(false)
	, _setGameStateInit(false)
	, _check(false)
{
	_stopGameRect = makeRect(842, 730, 181, 40);
}

PlayScene::~PlayScene()
{
}

void PlayScene::init()
{
	float randomNum1 = static_cast<float>(RANDOM->getIntFromTo(100, 300));
	float randomNum2 = static_cast<float>(RANDOM->getIntFromTo(400, 600));
    //캐릭터 생성
	//TODO: 지정해놓은 위치에, 랜덤하게 생성되오야 한다!!
	//2인 모드로 하면 2명의 player가 생성되어야한다
	if (_mode == ModeTypeTag::TwoPlayer) //2인 모드
	{
		Player* player1 = new Player(PlayerTypeTag::Player1, randomNum1, randomNum1);
		Player* player2 = new Player(PlayerTypeTag::Player2, randomNum2, randomNum2);
		GAMEOBJMANGER->registerObj(player1);
		GAMEOBJMANGER->registerObj(player2);

	}
	else if(_mode == ModeTypeTag::Monster) //몬스터 모드
	{
		Player* player = new Player(PlayerTypeTag::SoloPlayer, 300.f, 300.f);
		GAMEOBJMANGER->registerObj(player);
	}

	//아이템 생성 -> ItemFactory에 위임
	ItemFactory::createItemAtRandomPosition();

    //레디시 그려질 애니메이션 요소들을 게임매니저를 통해 등록한다(최상단에 그려주기 위함)
    _gameWords = new DesignElement(IMAGEMANAGER->findImage("GAME"));
    GAMEOBJMANGER->registerObj(_gameWords);
    _startWords = new DesignElement(IMAGEMANAGER->findImage("START"));
    GAMEOBJMANGER->registerObj(_startWords);
    _blackBg = new DesignElement(IMAGEMANAGER->findImage("blackBg"), true);
    GAMEOBJMANGER->registerObj(_blackBg);


    _gameWords->getImage()->setX(292);   _gameWords->getImage()->setY(253);
    _startWords->getImage()->setX(175); _startWords->getImage()->setY(338);

}

void PlayScene::release()
{
    SAFE_DELETE(_gameWords);
    SAFE_DELETE(_startWords);
    SAFE_DELETE(_blackBg);
	SAFE_DELETE(_gameGrayWords);
	SAFE_DELETE(_overWords);
}

void PlayScene::update()
{
	if (!GAMESTATEMANAGER->getGameOver())
	{
		if (_startTime + 2 < static_cast<int>(TIMEMANAGER->getWorldTime()))
			SOUNDMANAGER->repeatPlay(static_cast<int>(SoundTypeTag::PlayScene), SoundTypeTag::PlayScene);
	}
	//cout << static_cast<int>(_blackBg->getAlpha()) << endl;
    if (!GAMESTATEMANAGER->getGameStart())
    {
        if (_blackBg->getAlpha() > 20)
            _blackBg->setAlpha(_blackBg->getAlpha() - 10);

        //게임시작전에 게임스타트 문구 보이기
        int currentTime = static_cast<int>(TIMEMANAGER->getWorldTime());
        int diff = currentTime - _startTime;
		if (diff > 5)
        {
            _gameWords->getImage()->setY(_gameWords->getImage()->getY() - 20.f);
            _startWords->getImage()->setY(_startWords->getImage()->getY() + 20.f);
        }
        if (diff > 6)
        {
            _blackBg->setAlpha(0);
            GAMESTATEMANAGER->setGameStart(true);
            //사용자 입력을 받을 수 있음을 화면이 밝아지는 걸로 안내
        }
    }
    else
    {
		if (!_check)
		{
			_check = true;
			//게임이 시작하면 디자인구성요소들을 게임오브젝트 매니저에서 삭제한다
			GAMEOBJMANGER->removeObj(_gameWords->getId());
			GAMEOBJMANGER->removeObj(_startWords->getId());
			GAMEOBJMANGER->removeObj(_blackBg->getId());
		}
	}

	//게임 오버 상태이면
	if (GAMESTATEMANAGER->getGameOver())
	{
		//Game Over문구 보여주기
		if (!_setTime)
		{
			SOUNDMANAGER->stop(static_cast<int>(SoundTypeTag::PlayScene));
			SOUNDMANAGER->play(static_cast<int>(SoundTypeTag::Lose), SoundTypeTag::Lose);
			_setTime = true;
			_startTime = static_cast<int>(TIMEMANAGER->getWorldTime());
			
			//게임오버시 그려질 애니메이션 요소들을 게임 매니저를 통해 등록한다
			_gameGrayWords = new DesignElement(IMAGEMANAGER->findImage("GAME_GRAY"));
			GAMEOBJMANGER->registerObj(_gameGrayWords);
			_overWords = new DesignElement(IMAGEMANAGER->findImage("OVER"));
			GAMEOBJMANGER->registerObj(_overWords);

			_gameGrayWords->getImage()->setX(242);  _gameGrayWords->getImage()->setY(-240);
			_overWords->getImage()->setX(193);      _overWords->getImage()->setY(WINSIZEY);
		}

		int currentTime = static_cast<int>(TIMEMANAGER->getWorldTime());
		int diff = currentTime - _startTime;
		if (diff <= 6)
		{
			if(_gameGrayWords->getImage()->getY() <= 240)
				_gameGrayWords->getImage()->setY(_gameGrayWords->getImage()->getY() + 10.f);
			if(_overWords->getImage()->getY() >= 349)
				_overWords->getImage()->setY(_overWords->getImage()->getY() - 10.f);
		}
		if (diff > 6)//8초 뒤씬을 로비화면으로 변경시킨다
		{
			if (!_setGameStateInit)
			{
				//GAME STATE 재설정
				GAMESTATEMANAGER->setGameStart(false);
				GAMESTATEMANAGER->setGameOver(false);
				_setGameStateInit = true;
			}
			GAMEOBJMANGER->removeObjAll(); //게임오브젝트들 다 날리기
			SCENEMANAGER->changeScene(SceneTag::Lobby); //씬 바꾸기
		}
	}

	//나가기 버튼을 누르면
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&_stopGameRect, ptMouse))
		{
			SOUNDMANAGER->stop(static_cast<int>(SoundTypeTag::PlayScene));
			//GAME STATE 재설정
			GAMESTATEMANAGER->setGameStart(false);
			GAMESTATEMANAGER->setGameOver(false);

			GAMEOBJMANGER->removeObjAll(); //게임오브젝트들 다 날리기
			SCENEMANAGER->changeScene(SceneTag::ModeSelect); //씬 바꾸기
		}
	}

}

void PlayScene::render(HDC hdc)
{
    //배경
    IMAGEMANAGER->findImage("playBg")->render(hdc);

    //타일
    for (int i = 0; i < BOARD_ROW; ++i)
    {
        for (int j = 0; j < BOARD_COL; ++j)
        {
            IMAGEMANAGER->findImage(_vvTile[i][j].getStrKey())->render(hdc, BOARD_STARTX + (BOARD_RECTSIZE * j), BOARD_STARTY + (BOARD_RECTSIZE * i));
            /* 격자 출력하는 거 요기따!!!! */
			//drawRect(hdc, BOARD_STARTX + (BOARD_RECTSIZE * j), BOARD_STARTY + (BOARD_RECTSIZE * i), BOARD_RECTSIZE, BOARD_RECTSIZE);
        }
    }

    //화면 정보
    //Text(15, 10, WINSIZEY - 22, "Play Scene")(hdc);
    //if (_mode == ModeTypeTag::TwoPlayer)
    //{
    //    Text(15, 100, WINSIZEY - 22, "(2인 모드)")(hdc);
    //}
    //else if (_mode == ModeTypeTag::Monster)
    //{
    //    Text(15, 100, WINSIZEY - 22, "(몬스터 모드)")(hdc);
    //}

    //맵 정보
    if (_mapType == MapTypeTag::Forest)
    {
        Text(18, 62, 10, "포레스트", WHITE)(hdc);
    }

	//디버그용
	//debug(hdc);
}

void PlayScene::handleArgs(vector<int> args)
{
    assert(args.size() == 2); //반드시 지켜져야할 사항, 선예외처리 #include <cassert>, 디버그용 x -> 안정성용
    _mapType = static_cast<MapTypeTag>(args.at(1));
    _mode = static_cast<ModeTypeTag>(args.at(0));
    loadTile();
}

void PlayScene::loadTile()
{
    _vvTile = TileFactory::makeMapTile(_mapType);
}

void PlayScene::changeMapArr(int row, int col, bool isBomb, bool rightAfter)
{
    mapArr[row][col].isBomb = isBomb;
    mapArr[row][col].rightAfter = rightAfter;
}

void PlayScene::setIsItem(int row, int col, bool isItem)
{
	isItemArr[row][col] = isItem;
}

bool PlayScene::getIsItem(int row, int col)
{
	return isItemArr[row][col];
}

void PlayScene::debug(HDC hdc)
{
	//디버깅용 맵 위에 갈 수 있는지 없는 지 여부 나타내기
	//for (int i = 0; i < BOARD_ROW; ++i)
	//{
	//    for (int j = 0; j < BOARD_COL; ++j)
	//    {
	//        Text(10, BOARD_STARTX + j * BOARD_RECTSIZE, BOARD_STARTY + i * BOARD_RECTSIZE, to_string(PlayScene::mapArr[i][j].isBomb))(hdc);
	//        Text(10, BOARD_STARTX + j * BOARD_RECTSIZE, BOARD_STARTY + 10 + i * BOARD_RECTSIZE, to_string(PlayScene::mapArr[i][j].rightAfter))(hdc);
	//        //TextOut(hdc, BOARD_STARTX + j * BOARD_RECTSIZE, BOARD_STARTY + i * BOARD_RECTSIZE, to_string(PlayScene::mapArr[i][j].isBomb).c_str(), to_string(PlayScene::mapArr[i][j].isBomb).size());
	//        //TextOut(hdc, BOARD_STARTX + j * BOARD_RECTSIZE, BOARD_STARTY + 10 + i * BOARD_RECTSIZE, to_string(PlayScene::mapArr[i][j].rightAfter).c_str(), to_string(PlayScene::mapArr[i][j].rightAfter).size());
	//    }
	//}
	drawRect(hdc, _stopGameRect.left, _stopGameRect.top, _stopGameRect.right - _stopGameRect.left, _stopGameRect.bottom - _stopGameRect.top);
}