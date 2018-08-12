#pragma once
#include "image.h"

static Image* _backbuffer = IMAGEMANAGER->addImage("backBuffer", WINSIZEX, WINSIZEY);
class GameNode
{
private:
    HDC _hdc;
    bool _managerInit;

    //protected:
    //    HBITMAP g_backBitmap; //������۸���

public:
    GameNode();
    virtual ~GameNode();

    virtual HRESULT init();
    virtual HRESULT init(bool managerInit);
    virtual void release();
    virtual void update();
    virtual void render();

    Image* getBackBuffer() { return _backbuffer; } //����� ������

    HDC getMemDC() { return _backbuffer->getMemDC(); }
    HDC getHDC() { return _hdc; }

    LRESULT MainProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};