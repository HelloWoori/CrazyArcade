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
            if (_currentWaveIndex <= _POWER) // _MAKE_COOLTIME마다 wave 객체 생성(순차적으로 그려지는 느낌 주기 위함)
            {                
                //범위를 넘어선다면 애초에 만들지 않는다
                //TODO: 블럭 등이 있어서 그리지 못한다면 -> 애초에 생성하지 않는다
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
    else if (_state == WaveStartingPointStateTag::Complete) //wave를 power만큼 다 만들었으면 게임오브젝트 매니저에서 삭제
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
