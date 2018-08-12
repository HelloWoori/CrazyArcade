#include "stdafx.h"
#include "bomb.h"
#include "playScene.h"

Bomb::Bomb(Player* player, POINT_FLOAT center, int power)
	: GameObject(GameObjectTag::Bomb)
	, _SIZE(BOARD_RECTSIZE)
	, _POWER(power)
	, _previousState(BombStateTag::Not)
	, _currentState(BombStateTag::Set) //ù ���´� ��ġ�Ǿ��ٴ� �ǹ̿��� set
	, _startTime(TIMEMANAGER->getWorldTime())
	, _BOMB_COOLTIME(0.2f)
	, _POP_COOLTIME(0.1f)
	, _POP_TIME(3.f)
	, _DISAPPEAR_TIME(0.5f)
	, _whoUsed(player)
	, _chainExplosion(false)
{
    setAnimationInfo("bomb1", _BOMB_COOLTIME);

    //�� ���ڿ� �� ���� ���
    _center.x = static_cast<float>(static_cast<int>((BOARD_STARTX + center.x) / BOARD_RECTSIZE) * BOARD_RECTSIZE);
    _center.y = static_cast<float>(static_cast<int>(center.y / BOARD_RECTSIZE) * BOARD_RECTSIZE + (BOARD_RECTSIZE / 2) + 1);

    _start.x = _center.x - BOARD_RECTSIZE / 2.f;
    _start.y = _center.y - BOARD_RECTSIZE / 2.f;

    //���� ��� ��ǥ�� �ʿ��� �� �� �� ���� �ִ��� �˾Ƴ�
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
        if (currentTime - _startTime > _POP_TIME) //3�� �Ŀ� ��ǳ�� state�� pop ���°� �ȴ�(���ٱ� ���� ������ ���ؼ� state�� �ϳ� �� ����)
        {
            _previousState = _currentState;
            _currentState = BombStateTag::MakeWaveStartingPoint;
        }
    }
	else if (_currentState == BombStateTag::MakeWaveStartingPoint)
	{
		WaveStartingPoint* waveStartPoint = new WaveStartingPoint(_mapSpace, _POWER); //���ٱ⸦ ����� ����
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
 
        if (getFrame() == IMAGEMANAGER->findImage(getStrKey())->getMaxFrameX())  //������ �ִϸ��̼��� �����ϰ�, �ƽ� �������� �Ǹ� �����Ѵ�
        {
			_whoUsed->changeUsedBombs();//�������Ƿ� �������� ��ǳ�� ���� ����
            GAMEOBJMANGER->removeObj(getId());
            PlayScene::changeMapArr(_mapSpace.row, _mapSpace.col, false);        //isBomb�� false�� �ٲ��ش�
        }
    }
}

void Bomb::render(HDC hdc)
{
    IMAGEMANAGER->findImage(getStrKey())->frameRender(hdc, static_cast<int>(_center.x - 28.f), static_cast<int>(_center.y - 27.f), getFrame(), 0);
	//������
	//debug(hdc);

}

void Bomb::release()
{
}

//�浹�Ǿ��� �� �ؾ��� �ൿ
void Bomb::onCollisionEnter(GameObject* other, RECT area)
{
    int areaWidth = area.right - area.left;
    int areaHeight = area.bottom - area.top;

    switch (other->getTag())
    {
    case GameObjectTag::Player: //ĳ���Ͷ� �浹�� ���
        {
            Player* p = dynamic_cast<Player*>(other);
            //������ŭ ĳ���͸� �аǵ�, �����¿� ��𿡼� �����ϴ��Ŀ� ���� �̴� ������ �޶���
            //x������ �����Ѱ��
            if(areaWidth < areaHeight)
            {
                //if (p->getStartX() < area.left)                 //���ʿ��� ���������� ������ ��, �������� �о��ش�
                //    p->setStartX(p->getStartX() - areaWidth);
                //else                                            //�����ʿ��� �������� ������ ��
                //    p->setStartX(p->getStartX() + areaWidth);
                if (p->getBodyStartX() < area.left)                 //���ʿ��� ���������� ������ ��, �������� �о��ش�
                    p->setStartX(p->getStartX() - areaWidth);
                else                                            //�����ʿ��� �������� ������ ��
                    p->setStartX(p->getStartX() + areaWidth);
            }
            //y������ �����Ѱ��
            else 
            {
                //if (p->getStartY() < area.top)                  //������ �Ʒ��� �����Ѱ�, ���� �о��ش�
                //    p->setStartY(p->getStartY() - areaHeight);
                //else                                            //�Ʒ����� ���� ������ ��, �Ʒ��� �о��ش�
                //    p->setStartY(p->getStartY() + areaHeight);
                if (p->getBodyStartY() < area.top)                  //������ �Ʒ��� �����Ѱ�, ���� �о��ش�
                    p->setStartY(p->getStartY() - areaHeight);
                else                                            //�Ʒ����� ���� ������ ��, �Ʒ��� �о��ش�
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
    //������ �簢��
    PenWithHatchBrush(1, RGB(66, 110, 244), RGB(81, 243, 255), HS_BDIAGONAL)(hdc, [hdc, this]
    {
        drawRect(hdc, _start.x, _start.y, _SIZE, _SIZE);
    });

	Text(15, _start.x, _start.y, showBombStateForDebug(_currentState))(hdc);
}