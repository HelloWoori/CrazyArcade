#include "stdafx.h"
#include "bomb.h"
#include "playScene.h"

Bomb::Bomb(Player* player, POINT_FLOAT center, int power)
	: GameObject(GameObjectTag::Bomb)
	, _SIZE(BOARD_RECTSIZE)
	, _POWER(power)
	, _previousState(BombStateTag::Not)
	, _currentState(BombStateTag::Set) //첫 상태는 설치되었다는 의미에서 set
	, _startTime(TIMEMANAGER->getWorldTime())
	, _BOMB_COOLTIME(0.2f)
	, _POP_COOLTIME(0.1f)
	, _POP_TIME(3.f)
	, _DISAPPEAR_TIME(0.5f)
	, _whoUsed(player)
	, _chainExplosion(false)
{
    setAnimationInfo("bomb1", _BOMB_COOLTIME);

    //맵 격자에 딱 맞춰 출력
    _center.x = static_cast<float>(static_cast<int>((BOARD_STARTX + center.x) / BOARD_RECTSIZE) * BOARD_RECTSIZE);
    _center.y = static_cast<float>(static_cast<int>(center.y / BOARD_RECTSIZE) * BOARD_RECTSIZE + (BOARD_RECTSIZE / 2) + 1);

    _start.x = _center.x - BOARD_RECTSIZE / 2.f;
    _start.y = _center.y - BOARD_RECTSIZE / 2.f;

    //왼쪽 상단 좌표로 맵에서 몇 행 몇 열에 있는지 알아냄
    MapSpace m = leftTopToMapSpace(_start.x, _start.y);
    _mapSpace.row = m.row;  _mapSpace.col = m.col;
}

Bomb::~Bomb()
{
}

void Bomb::init()
{
	//cout << PlayScene::mapArr[_mapSpace.row][_mapSpace.col].rightAfter << endl;
	PlayScene::changeMapArr(_mapSpace.row, _mapSpace.col, true, true);
	//cout << PlayScene::mapArr[_mapSpace.row][_mapSpace.col].rightAfter << endl;
	//SOUNDMANAGER->play(1, SoundTypeTag::);
}

void Bomb::update()
{
    //cout << "Bomb previousState: " << showBombStateForDebug(_previousState) << endl;
    //cout << "Bomb currentState: " << showBombStateForDebug(_currentState) << endl;
    float currentTime;
    if (_currentState == BombStateTag::Set)
    {
        currentTime = TIMEMANAGER->getWorldTime();
        animation(IMAGEMANAGER->findImage(getStrKey())->getMaxFrameX());
        if (currentTime - _startTime > _POP_TIME) //3초 후에 물풍선 state는 pop 상태가 된다(물줄기 연쇄 폭발을 위해서 state를 하나 더 만듦)
        {
            _previousState = _currentState;
            _currentState = BombStateTag::MakeWaveStartingPoint;
        }
    }
	else if (_currentState == BombStateTag::MakeWaveStartingPoint)
	{
		WaveStartingPoint* waveStartPoint = new WaveStartingPoint(_mapSpace, _POWER); //물줄기를 만들기 위함
		GAMEOBJMANGER->registerObj(waveStartPoint);
		_previousState = _currentState;
		_currentState = BombStateTag::Pop;
	}
    else if(_currentState == BombStateTag::Pop)
    {
        if (_currentState != _previousState)
        {
            setAnimationInfo("bombPop", _POP_COOLTIME);
            _previousState = _currentState;
        }
        animation(IMAGEMANAGER->findImage(getStrKey())->getMaxFrameX());
 
        if (getFrame() == IMAGEMANAGER->findImage(getStrKey())->getMaxFrameX())  //터지는 애니메이션을 실행하고, 맥스 프레임이 되면 삭제한다
        {
			_whoUsed->changeUsedBombs();//터졌으므로 소유자의 물풍선 개수 조정
            GAMEOBJMANGER->removeObj(getId());
            PlayScene::changeMapArr(_mapSpace.row, _mapSpace.col, false);        //isBomb을 false로 바꿔준다
        }
    }
}

void Bomb::render(HDC hdc)
{
    IMAGEMANAGER->findImage(getStrKey())->frameRender(hdc, static_cast<int>(_center.x - 28.f), static_cast<int>(_center.y - 27.f), getFrame(), 0);
	//디버깅용
	//debug(hdc);

}

void Bomb::release()
{
}

//충돌되었을 때 해야할 행동
void Bomb::onCollisionEnter(GameObject* other, RECT area)
{
    int areaWidth = area.right - area.left;
    int areaHeight = area.bottom - area.top;

    switch (other->getTag())
    {
    case GameObjectTag::Player: //캐릭터랑 충돌한 경우
        {
            Player* p = dynamic_cast<Player*>(other);
            //공간만큼 캐릭터를 밀건데, 상하좌우 어디에서 접근하느냐에 따라 미는 방향이 달라짐
            //x축으로 접근한경우
            if(areaWidth < areaHeight)
            {
                //if (p->getStartX() < area.left)                 //왼쪽에서 오른쪽으로 접근한 것, 왼쪽으로 밀어준다
                //    p->setStartX(p->getStartX() - areaWidth);
                //else                                            //오른쪽에서 왼쪽으로 접근한 것
                //    p->setStartX(p->getStartX() + areaWidth);
                if (p->getBodyStartX() < area.left)                 //왼쪽에서 오른쪽으로 접근한 것, 왼쪽으로 밀어준다
                    p->setStartX(p->getStartX() - areaWidth);
                else                                            //오른쪽에서 왼쪽으로 접근한 것
                    p->setStartX(p->getStartX() + areaWidth);
            }
            //y축으로 접근한경우
            else 
            {
                //if (p->getStartY() < area.top)                  //위에서 아래로 접근한것, 위로 밀어준다
                //    p->setStartY(p->getStartY() - areaHeight);
                //else                                            //아래에서 위로 접근한 것, 아래로 밀어준다
                //    p->setStartY(p->getStartY() + areaHeight);
                if (p->getBodyStartY() < area.top)                  //위에서 아래로 접근한것, 위로 밀어준다
                    p->setStartY(p->getStartY() - areaHeight);
                else                                            //아래에서 위로 접근한 것, 아래로 밀어준다
                    p->setStartY(p->getStartY() + areaHeight);
            }
        }
    break;
    }//switch
}

string Bomb::showBombStateForDebug(BombStateTag tag)
{
    switch (tag)
    {
    case BombStateTag::Not:
        return "Not";
    case BombStateTag::Set:
        return "Set";
    case BombStateTag::Pop:
        return "Pop";
	case BombStateTag::MakeWaveStartingPoint:
		return "MakeWaveStartingPoint";
    default:
        return "?";
    }
}

void Bomb::debug(HDC hdc)
{   
    //디버깅용 사각형
    PenWithHatchBrush(1, RGB(66, 110, 244), RGB(81, 243, 255), HS_BDIAGONAL)(hdc, [hdc, this]
    {
        drawRect(hdc, _start.x, _start.y, _SIZE, _SIZE);
    });

	Text(15, _start.x, _start.y, showBombStateForDebug(_currentState))(hdc);
}