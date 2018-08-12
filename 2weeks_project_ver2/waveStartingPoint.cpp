#include "stdafx.h"
#include "waveStartingPoint.h"

WaveStartingPoint::WaveStartingPoint(MapSpace mapSpace, int power)
    : GameObject(GameObjectTag::WaveStartingPoint)
    , _state(WaveStartingPointStateTag::Making)
    , _mapSpace(mapSpace)
    , _POWER(power)
    , _currentWaveIndex(1)
    , _MAKE_COOLTIME(0.05f)
    , _SIZE(BOARD_RECTSIZE)
{
    _coolDown = _MAKE_COOLTIME;
    POINT_FLOAT pt = mapSpaceToLeftTop(mapSpace.row, mapSpace.col);
    _start.x = pt.x;
    _start.y = pt.y;
}

WaveStartingPoint::~WaveStartingPoint()
{
}

void WaveStartingPoint::init()
{
	SOUNDMANAGER->play(1, SoundTypeTag::Wave);
}

void WaveStartingPoint::update()
{
    if (_state == WaveStartingPointStateTag::Making)
    {
        _coolDown -= TIMEMANAGER->getElapsedTime();
        if (_coolDown <= 0.f)
        {
            _coolDown = _MAKE_COOLTIME;
            if (_currentWaveIndex <= _POWER) // _MAKE_COOLTIME���� wave ��ü ����(���������� �׷����� ���� �ֱ� ����)
            {                
                //������ �Ѿ�ٸ� ���ʿ� ������ �ʴ´�
                //TODO: �� ���� �־ �׸��� ���Ѵٸ� -> ���ʿ� �������� �ʴ´�
                if (_mapSpace.row - _currentWaveIndex >= 0)
                {
                    Wave* up = new Wave(_mapSpace.row - _currentWaveIndex, _mapSpace.col, WaveStateTag::Up, _POWER);
                    GAMEOBJMANGER->registerObj(up);
                }
                if (_mapSpace.row + _currentWaveIndex <= BOARD_ROW - 1)
                {
                    Wave* down = new Wave(_mapSpace.row + _currentWaveIndex, _mapSpace.col, WaveStateTag::Down, _POWER);
                    GAMEOBJMANGER->registerObj(down);
                }
                if (_mapSpace.col - _currentWaveIndex >= 0)
                {
                    Wave* left = new Wave(_mapSpace.row, _mapSpace.col - _currentWaveIndex, WaveStateTag::Left, _POWER);
                    GAMEOBJMANGER->registerObj(left);
                }
                if (_mapSpace.col + _currentWaveIndex <= BOARD_COL - 1)
                {
                    Wave* right = new Wave(_mapSpace.row, _mapSpace.col + _currentWaveIndex, WaveStateTag::Right, _POWER);
                    GAMEOBJMANGER->registerObj(right);
                }
                ++_currentWaveIndex;
            }
            if (_currentWaveIndex - 1 == _POWER)
                _state = WaveStartingPointStateTag::Complete;
        }
    }
    else if (_state == WaveStartingPointStateTag::Complete) //wave�� power��ŭ �� ��������� ���ӿ�����Ʈ �Ŵ������� ����
    {
        GAMEOBJMANGER->removeObj(getId());
    }
    
}

void WaveStartingPoint::render(HDC hdc)
{
}

void WaveStartingPoint::release()
{
}
