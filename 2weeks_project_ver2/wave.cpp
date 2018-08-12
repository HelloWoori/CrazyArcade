#include "stdafx.h"  
#include "wave.h"

Wave::Wave(int row, int col, WaveStateTag state, int power)
    : GameObject(GameObjectTag::Wave)
    , _state(state)
    , _SIZE(BOARD_RECTSIZE)
    , _startTime(TIMEMANAGER->getWorldTime())
    , _POWER(power)
    , _WAVE_COOLTIME(0.05f)
{
    _mapSpace.row = row;
    _mapSpace.col = col;

    //�� ��ǥ�� ������ǥ��
    POINT_FLOAT pt = mapSpaceToLeftTop(_mapSpace.row, _mapSpace.col);
    _start.x = pt.x ;
    _start.y = pt.y;
}

Wave::~Wave()
{
}

void Wave::init()
{
    //���ٱ⿡ ���� �ٸ��� �׷��ش�
    switch (_state)
    {
    case WaveStateTag::Up:
    {
        if (_POWER == 1)
            setAnimationInfo("waveUp1",  _WAVE_COOLTIME);
        else 
            setAnimationInfo("waveUp2", _WAVE_COOLTIME);
    }
    break;
    case WaveStateTag::Down:
    {
        if (_POWER == 1)
            setAnimationInfo("waveDown1", _WAVE_COOLTIME);
        else
            setAnimationInfo("waveUp2", _WAVE_COOLTIME);
    }
    break;
    case WaveStateTag::Left:
    {
        if (_POWER == 1)
            setAnimationInfo("waveLeft1", _WAVE_COOLTIME);
        else
            setAnimationInfo("waveLeft2", _WAVE_COOLTIME);
    }
    break;
    case WaveStateTag::Right:
    {
        if (_POWER == 1)
            setAnimationInfo("waveRight1", _WAVE_COOLTIME);
        else
            setAnimationInfo("waveRight2", _WAVE_COOLTIME);
    }
    break;
    }// switch ��
}

void Wave::update()
{ 
	//������ �� ���� ���ӿ�����Ʈ�Ŵ������� ����
 	int maxFrame = IMAGEMANAGER->findImage(getStrKey())->getMaxFrameX();
	animation(maxFrame);

    if (getFrame() >= maxFrame)
        GAMEOBJMANGER->removeObj(getId());
}

void Wave::render(HDC hdc)
{

    IMAGEMANAGER->findImage(getStrKey())->frameRender(hdc, static_cast<int>(_start.x), static_cast<int>(_start.y), getFrame(), 0);
    //����׿�
    //debug(hdc);
}

void Wave::release()
{
}

void Wave::onCollisionEnter(GameObject * other, RECT area)
{
}

void Wave::debug(HDC hdc)
{
    PenWithHatchBrush(1, RGB(66, 110, 244), RGB(81, 243, 255), HS_CROSS)(hdc, [hdc, this]
    {
        drawRect(hdc, _start.x, _start.y, _SIZE, _SIZE);
    });
}
