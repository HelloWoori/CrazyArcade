#pragma once
#include "enum.h"
//float�� ������
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

    bool isBomb; //��ź�� �ִ��� ������
    bool rightAfter; //��ź ��ġ ���� ����
};

struct MapSpace
{
    int row;
    int col;
};