#include "stdafx.h"
#include "gameNode.h"

GameNode::GameNode()
{
}

GameNode::~GameNode()
{
}

HRESULT GameNode::init()
{
    _hdc = GetDC(_hWnd);
    _managerInit = false;
    //SetTimer(_hWnd, 1, 5, NULL); //Ÿ�̸� �ʱ�ȭ
    //KEYMANAGER->init();
    return S_OK;
}

HRESULT GameNode::init(bool managerInit)
{
    _hdc = GetDC(_hWnd);
    _managerInit = managerInit;
    if (_managerInit)
    {
        KEYMANAGER->init();
        IMAGEMANAGER->init();
		SOUNDMANAGER->init();
        TIMEMANAGER->init();

        SCENEMANAGER->init();
    }
    return S_OK;
}

void GameNode::release()
{
    //KillTimer(_hWnd, 1); //Ÿ�̸� ���̱�
    /* ���� ���۸��� - backBitmap�� ���� �ִٸ� ������! */
    //if (g_backBitmap)
    //{
    //    DeleteObject(g_backBitmap);
    //}
    if (_managerInit)
    {
        /* �̱��� */
        KEYMANAGER->release();
        KEYMANAGER->releaseSingleton();

        IMAGEMANAGER->release();
        IMAGEMANAGER->releaseSingleton();

        TIMEMANAGER->release();
        TIMEMANAGER->releaseSingleton();

        SCENEMANAGER->release();
        SCENEMANAGER->releaseSingleton();

        GAMESTATEMANAGER->releaseSingleton();
        RANDOM->releaseSingleton();
        GAMEOBJMANGER->releaseSingleton();

		SOUNDMANAGER->release();
		SOUNDMANAGER->releaseSingleton();
    }

    //DC ����������
    ReleaseDC(_hWnd, _hdc);
}

void GameNode::update()
{
    //���߿� ���� Ÿ�̸Ӹ� ���� �Ŀ��� ��� ����
    //������� ���� ������� ���� true -> false;
    //InvalidateRect(_hWnd, NULL, false);
}

void GameNode::render()
{
}

LRESULT GameNode::MainProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    //HDC hdc, hMemDC;
    //PAINTSTRUCT ps;

    //hMemDC�� �Ʒ� ������ ������۸���
    //HBITMAP oldBitmap;
    //RECT crt;

    switch (iMessage)
    {
        //case WM_TIMER:
        //    this->update();
        //    break;
        //case WM_PAINT:
        //    hdc = BeginPaint(hwnd, &ps); //�׸��� ����
        //    /* ���� ���۸� ���� */
        //    GetClientRect(hwnd, &crt);
        //    if (g_backBitmap == nullptr)
        //    {
        //        g_backBitmap = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
        //    }
        //    hMemDC = CreateCompatibleDC(hdc); //hMemDC�� �޸𸮿� �׷��� DC
        //    oldBitmap = (HBITMAP)SelectObject(hMemDC, g_backBitmap);
        //    FillRect(hMemDC, &crt, GetSysColorBrush(COLOR_WINDOW));
        //    SetBkMode(hMemDC, TRANSPARENT);

        //    this->render(hMemDC); //hMemDC�� �׸��̳� �ؽ�Ʈ �� �ʿ��� �� ������ش�

        //    BitBlt(hdc, 0, 0, crt.right, crt.bottom, hMemDC, 0, 0, SRCCOPY);
        //    SelectObject(hMemDC, oldBitmap);
        //    DeleteDC(hMemDC);
        //    /* ���� ���۸� �� */
        //    EndPaint(hwnd, &ps); //�׸��� ����
        //    break;
        //   
        //break;

    case WM_MOUSEMOVE:
        ptMouse.x = LOWORD(lParam);
        ptMouse.y = HIWORD(lParam);
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    //������ ���ν������� ó������ ���� ������ �޼����� ó�����ش�
    return (DefWindowProc(hwnd, iMessage, wParam, lParam));
}
