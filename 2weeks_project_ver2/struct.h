#pragma once
#include "enum.h"
//float형 포인터
struct POINT_FLOAT
{
    POINT_FLOAT()
        : x(0.f)
        , y(0.f)
    {
    }
    POINT_FLOAT(float px, float py)
    {
        x = px;
        y = py;
    }
    float x;
    float y;
};

struct CanIGo
{
    CanIGo()
        : isBomb(false)
        , rightAfter(false)
    {
    }

    bool isBomb; //폭탄이 있는지 없는지
    bool rightAfter; //폭탄 설치 직후 인지
};

struct MapSpace
{
    int row;
    int col;
};