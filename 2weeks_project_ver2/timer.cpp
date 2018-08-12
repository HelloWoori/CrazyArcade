#include "stdafx.h"
#include "timer.h"
#include <mmsystem.h>

//TimeGetTime �Լ��� ����ϱ� ���� ���̺귯��
//�����찡 �������� �ð��� �˷��ִ� API
#pragma comment(lib, "winmm.lib")

Timer::Timer()
{
}


Timer::~Timer()
{
}

HRESULT Timer::init()
{
    //���� Ÿ�̸� �������� ����� ����
    //���ÿ� ���� Ÿ�̸� ������ �ȴٸ� _periodFrequency ���� �ʴ� �ľ��� �� �ִ� �ð��� ���� ���´�.
    //���� ��� �ʴ� �и���������� ����� �����ϴٸ� 1000 ���� ���´�.
    if (QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency))
    {
        _isHardware = true;
        QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);

        //�ʴ� �ð��� ����� �� �ִ� ���� //�ʴ� �ð� ��� ����
        _timeScale = 1.0f / _periodFrequency;
    }
    else
    {
        _isHardware = false;

        //�и������� ������ �ʸ� ������ ������
        _lastTime = timeGetTime();
        _timeScale = 0.001f;
    }

    _frameRate = 0;
    _FPSFrameCount = 0;
    _FPSTimeElapsed = 0;
    _worldTime = 0.f;

    return S_OK;
}

void Timer::tick(float lockFPS)
{
    //���� Ÿ�̸� ���� �ϴ�?
    if (_isHardware)
    {
        //������ ����ũ���� ������ ���
        //QueryPerformanceCounter �� ��Ƽ �ھ�� ����ȭ ������ �ִ�.
        QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
    }
    else
    {
        //�������� ������ time �Լ� �̿� ����
        _curTime = timeGetTime();
    }

    //������ �ð��� ���� �ð��� �����
    _timeElapsed = (_curTime - _lastTime) * _timeScale;

    //���� ������ ó�� (������ ��Ű��)
    if (lockFPS > 0.0f)
    {
        //���� �������� �ð��� ������ ������ ������ ����
        while (_timeElapsed < (1.0f / lockFPS))
        {
            if (_isHardware) QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
            else _curTime = timeGetTime();

            //������ �ð��� ����ð��� �����
            _timeElapsed = (_curTime - _lastTime) * _timeScale;
        }
    }

    _lastTime = _curTime;
    _FPSFrameCount++;
    _FPSTimeElapsed += _timeElapsed;
    _worldTime += _timeElapsed;

    //������ �ʱ�ȭ 1�ʸ��� ��������
    if (_FPSTimeElapsed > 1.0f)
    {
        _frameRate = _FPSFrameCount;
        _FPSFrameCount = 0;
        _FPSTimeElapsed = 0.0f;
    }
}

unsigned long Timer::getFrameRate(char * str) const
{
    if (str != NULL)
    {
        wsprintf(str, "FPS : %d", _frameRate);
    }

    return _frameRate;
}
