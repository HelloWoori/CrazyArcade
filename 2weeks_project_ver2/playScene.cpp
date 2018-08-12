#include "stdafx.h"
#include "playScene.h"
#include "itemFactory.h"

//��ǳ������ ������ ���� (ó���� ��ǳ���� �ִ��� ���ο�, ���� �ٷ����Ŀ��� �ΰ��� false)
CanIGo PlayScene::mapArr[BOARD_ROW][BOARD_COL] = { CanIGo() };
//�����۰��� ������ ����(ó���� �������� �ִ��� ���ο�, �������� ������ ���� false, Not)
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
    //ĳ���� ����
	//TODO: �����س��� ��ġ��, �����ϰ� �����ǿ��� �Ѵ�!!
	//2�� ���� �ϸ� 2���� player�� �����Ǿ���Ѵ�
	if (_mode == ModeTypeTag::TwoPlayer) //2�� ���
	{
		Player* player1 = new Player(PlayerTypeTag::Player1, randomNum1, randomNum1);
		Player* player2 = new Player(PlayerTypeTag::Player2, randomNum2, randomNum2);
		GAMEOBJMANGER->registerObj(player1);
		GAMEOBJMANGER->registerObj(player2);

	}
	else if(_mode == ModeTypeTag::Monster) //���� ���
	{
		Player* player = new Player(PlayerTypeTag::SoloPlayer, 300.f, 300.f);
		GAMEOBJMANGER->registerObj(player);
	}

	//������ ���� -> ItemFactory�� ����
	ItemFactory::createItemAtRandomPosition();

    //����� �׷��� �ִϸ��̼� ��ҵ��� ���ӸŴ����� ���� ����Ѵ�(�ֻ�ܿ� �׷��ֱ� ����)
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

        //���ӽ������� ���ӽ�ŸƮ ���� ���̱�
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
            //����� �Է��� ���� �� ������ ȭ���� ������� �ɷ� �ȳ�
        }
    }
    else
    {
		if (!_check)
		{
			_check = true;
			//������ �����ϸ� �����α�����ҵ��� ���ӿ�����Ʈ �Ŵ������� �����Ѵ�
			GAMEOBJMANGER->removeObj(_gameWords->getId());
			GAMEOBJMANGER->removeObj(_startWords->getId());
			GAMEOBJMANGER->removeObj(_blackBg->getId());
		}
	}

	//���� ���� �����̸�
	if (GAMESTATEMANAGER->getGameOver())
	{
		//Game Over���� �����ֱ�
		if (!_setTime)
		{
			SOUNDMANAGER->stop(static_cast<int>(SoundTypeTag::PlayScene));
			SOUNDMANAGER->play(static_cast<int>(SoundTypeTag::Lose), SoundTypeTag::Lose);
			_setTime = true;
			_startTime = static_cast<int>(TIMEMANAGER->getWorldTime());
			
			//���ӿ����� �׷��� �ִϸ��̼� ��ҵ��� ���� �Ŵ����� ���� ����Ѵ�
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
		if (diff > 6)//8�� �ھ��� �κ�ȭ������ �����Ų��
		{
			if (!_setGameStateInit)
			{
				//GAME STATE �缳��
				GAMESTATEMANAGER->setGameStart(false);
				GAMESTATEMANAGER->setGameOver(false);
				_setGameStateInit = true;
			}
			GAMEOBJMANGER->removeObjAll(); //���ӿ�����Ʈ�� �� ������
			SCENEMANAGER->changeScene(SceneTag::Lobby); //�� �ٲٱ�
		}
	}

	//������ ��ư�� ������
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&_stopGameRect, ptMouse))
		{
			SOUNDMANAGER->stop(static_cast<int>(SoundTypeTag::PlayScene));
			//GAME STATE �缳��
			GAMESTATEMANAGER->setGameStart(false);
			GAMESTATEMANAGER->setGameOver(false);

			GAMEOBJMANGER->removeObjAll(); //���ӿ�����Ʈ�� �� ������
			SCENEMANAGER->changeScene(SceneTag::ModeSelect); //�� �ٲٱ�
		}
	}

}

void PlayScene::render(HDC hdc)
{
    //���
    IMAGEMANAGER->findImage("playBg")->render(hdc);

    //Ÿ��
    for (int i = 0; i < BOARD_ROW; ++i)
    {
        for (int j = 0; j < BOARD_COL; ++j)
        {
            IMAGEMANAGER->findImage(_vvTile[i][j].getStrKey())->render(hdc, BOARD_STARTX + (BOARD_RECTSIZE * j), BOARD_STARTY + (BOARD_RECTSIZE * i));
            /* ���� ����ϴ� �� ����!!!! */
			//drawRect(hdc, BOARD_STARTX + (BOARD_RECTSIZE * j), BOARD_STARTY + (BOARD_RECTSIZE * i), BOARD_RECTSIZE, BOARD_RECTSIZE);
        }
    }

    //ȭ�� ����
    //Text(15, 10, WINSIZEY - 22, "Play Scene")(hdc);
    //if (_mode == ModeTypeTag::TwoPlayer)
    //{
    //    Text(15, 100, WINSIZEY - 22, "(2�� ���)")(hdc);
    //}
    //else if (_mode == ModeTypeTag::Monster)
    //{
    //    Text(15, 100, WINSIZEY - 22, "(���� ���)")(hdc);
    //}

    //�� ����
    if (_mapType == MapTypeTag::Forest)
    {
        Text(18, 62, 10, "������Ʈ", WHITE)(hdc);
    }

	//����׿�
	//debug(hdc);
}

void PlayScene::handleArgs(vector<int> args)
{
    assert(args.size() == 2); //�ݵ�� ���������� ����, ������ó�� #include <cassert>, ����׿� x -> ��������
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
	//������ �� ���� �� �� �ִ��� ���� �� ���� ��Ÿ����
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