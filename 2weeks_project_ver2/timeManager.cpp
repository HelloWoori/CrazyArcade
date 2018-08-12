#include "stdafx.h"
#include "timeManager.h"
TimeManager::TimeManager()
    : _timer(NULL)
{
}

TimeManager::~TimeManager()
{
}

HRESULT TimeManager::init()
{
    //타이머 생성
    _timer = new Timer;
    _timer->init();
    return S_OK;
}

void TimeManager::release()
{
    if (_timer != NULL)
    {
        SAFE_DELETE(_timer);
    }
}

void TimeManager::update(float lock)
{
    if (_timer != NULL)
        _timer->tick(lock);
}

void TimeManager::render(HDC hdc)
{
    //char str[256];
    string strFrame;

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, WHITE);

#ifdef _DEBUG
    {
        if (_timer != NULL)
        {
            //프레임 찍어주자
            Text(12, 5, 10, TEXT("FramePerSec: ") + to_string(_timer->getFrameRate()))(hdc);
            //wsprintf(str, "frmaePerSec : %d", _timer->getFrameRate());
            //TextOut(hdc, 670, 670, str, strlen(str));

            //월드타임 찍어주자
            Text(12, 670, 690, TEXT("WorldTime: ") + to_string(_timer->getWorldTime()))(hdc);
            //sprintf_s(str, "worldtime : %f", _timer->getWorldTime());
            //TextOut(hdc, 670, 690, str, strlen(str));

            //갱신 Tick 찍어주자
            Text(12, 670, 710, TEXT("ElapsedTime: ") + to_string(_timer->getElapsedTime()))(hdc);
            //sprintf_s(str, "elapsedTime : %f", _timer->getElapsedTime());
            //TextOut(hdc, 670, 710, str, strlen(str));

        }
    }
#else
    {
        if (_timer != NULL)
        {
            //프레임 찍어주자
            Text(12, 920, 10, TEXT("FramePerSec: ") + to_string(_timer->getFrameRate()), WHITE)(hdc);
            //wsprintf(str, "frmaePerSec : %d", _timer->getFrameRate());
            //TextOut(hdc, 5, 5, str, strlen(str));
        }
    }
#endif
}
