#pragma once
#include "struct.h"
#include "define.h"

template < typename T1, typename T2, typename T3, typename T4>
inline void drawLine(HDC hdc, T1 x1, T2 y1, T3 x2, T4 y2)
{
    MoveToEx(hdc, static_cast<int>(x1), static_cast<int>(y1), NULL);
    LineTo(hdc, static_cast<int>(x2), static_cast<int>(y2));
}

template < typename T1, typename T2, typename T3, typename T4>
inline RECT makeRect(T1 x, T2 y, T3 width, T4 height)
{
    RECT rc = { static_cast<int>(x), static_cast<int>(y),
        static_cast<int>(x + width), static_cast<int>(y + height) };
    return rc;
}

template < typename T1, typename T2, typename T3, typename T4>
inline RECT makeRectCenter(T1 x, T2 y, T3 width, T4 height)
{
    RECT rc = { static_cast<int>(x - (width / 2)), static_cast<int>(y - (height / 2)),
        static_cast<int>(x + (width / 2)), static_cast<int>(y + (height / 2)) };
    return rc;
}

template < typename T1, typename T2, typename T3, typename T4>
inline void drawRect(HDC hdc, T1 x, T2 y, T3 width, T4 height)
{
    Rectangle(hdc, static_cast<int>(x), static_cast<int>(y),
        static_cast<int>(x + width), static_cast<int>(y + height));
}


template < typename T1, typename T2, typename T3, typename T4>
inline void drawRectCenter(HDC hdc, T1 x, T2 y, T3 width, T4 height)
{
    Rectangle(hdc, static_cast<int>(x - (width / 2)), static_cast<int>(y - (height / 2)),
        static_cast<int>(x + (width / 2)), static_cast<int>(y + (height / 2)));
}


template < typename T1, typename T2, typename T3, typename T4>
inline void drawEllipse(HDC hdc, T1 x, T2 y, T3 width, T4 height)
{
    Ellipse(hdc, static_cast<int>(x), static_cast<int>(y), static_cast<int>(x + width), static_cast<int>(y + height));
}

template < typename T1, typename T2, typename T3, typename T4>
inline void drawEllipseCenter(HDC hdc, T1 x, T2 y, T3 width, T4 height)
{
    Ellipse(hdc, static_cast<int>(x - (width / 2)), static_cast<int>(y - (height / 2)),
        static_cast<int>(x + (width / 2)), static_cast<int>(y + (height / 2)));
}


//ÁÂ»ó´Ü ÁÂÇ¥°¡ µé¾î¿À¸é, ¸ÊÀÇ Çà·Ä·Î º¯È¯
inline MapSpace leftTopToMapSpace(float startX, float startY)
{
    //int col = (static_cast<int>(_start.x) - BOARD_STARTX) / BOARD_RECTSIZE;
    //int row = (static_cast<int>(_start.y) - BOARD_STARTY) / BOARD_RECTSIZE;
    MapSpace m;
    m.col = (static_cast<int>(startX) - BOARD_STARTX) / BOARD_RECTSIZE;
    m.row = (static_cast<int>(startY) - BOARD_STARTY) / BOARD_RECTSIZE;
    return m;
}

//Áß°£ ÁÂÇ¥°¡ µé¾î¿À¸é, ¸ÊÀÇ Çà·Ä·Î º¯È¯
inline MapSpace centerToMapSpace(float centerX, float centerY)
{
    MapSpace m;
    float tempCenterX = static_cast<float>(static_cast<int>((BOARD_STARTX + centerX) / BOARD_RECTSIZE) * BOARD_RECTSIZE);
    float tempCenterY = static_cast<float>(static_cast<int>(centerY / BOARD_RECTSIZE) * BOARD_RECTSIZE + (BOARD_RECTSIZE / 2) + 1);

    float tempStartX = tempCenterX - BOARD_RECTSIZE / 2.f;
    float tempStartY = tempCenterY - BOARD_RECTSIZE / 2.f;

    m.col = (static_cast<int>(tempStartX) - BOARD_STARTX) / BOARD_RECTSIZE;
    m.row = (static_cast<int>(tempStartY) - BOARD_STARTY) / BOARD_RECTSIZE;

    return m;
}

//¸ÊÀÇ Çà·ÄÀÌ µé¾î¿À¸é ÁÂ»ó´Ü ÁÂÇ¥·Î º¯È¯
inline POINT_FLOAT mapSpaceToLeftTop(int row, int col)
{
    POINT_FLOAT pt;
    pt.x = static_cast<float>(BOARD_STARTX + col * BOARD_RECTSIZE);
    pt.y = static_cast<float>(BOARD_STARTY + row * BOARD_RECTSIZE);
    return pt;
}