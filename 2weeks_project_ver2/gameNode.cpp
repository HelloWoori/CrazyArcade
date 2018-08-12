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
    //SetTimer(_hWnd, 1, 5, NULL); //타이머 초기화
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
    //KillTimer(_hWnd, 1); //타이머 죽이기
    /* 더블 버퍼링용 - backBitmap에 무언가 있다면 지워라! */
    //if (g_backBitmap)
    //{
    //    DeleteObject(g_backBitmap);
    //}
    if (_managerInit)
    {
        /* 싱글톤 */
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

    //DC 해제해주자
    ReleaseDC(_hWnd, _hdc);
}

void GameNode::update()
{
    //나중에 고성능 타이머를 만든 후에는 사용 ㄴㄴ
    //더블버퍼 이후 사용하지 않음 true -> false;
    //InvalidateRect(_hWnd, NULL, false);
}

void GameNode::render()
{
}

LRESULT GameNode::MainProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    //HDC hdc, hMemDC;
    //PAINTSTRUCT ps;

    //hMemDC와 아래 변수는 더블버퍼링용
    //HBITMAP oldBitmap;
    //RECT crt;

    switch (iMessage)
    {
        //case WM_TIMER:
        //    this->update();
        //    break;
        //case WM_PAINT:
        //    hdc = BeginPaint(hwnd, &ps); //그리기 시작
        //    /* 더블 버퍼링 시작 */
        //    GetClientRect(hwnd, &crt);
        //    if (g_backBitmap == nullptr)
        //    {
        //        g_backBitmap = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
        //    }
        //    hMemDC = CreateCompatibleDC(hdc); //hMemDC는 메모리에 그려줄 DC
        //    oldBitmap = (HBITMAP)SelectObject(hMemDC, g_backBitmap);
        //    FillRect(hMemDC, &crt, GetSysColorBrush(COLOR_WINDOW));
        //    SetBkMode(hMemDC, TRANSPARENT);

        //    this->render(hMemDC); //hMemDC로 그림이나 텍스트 등 필요한 걸 출력해준다

        //    BitBlt(hdc, 0, 0, crt.right, crt.bottom, hMemDC, 0, 0, SRCCOPY);
        //    SelectObject(hMemDC, oldBitmap);
        //    DeleteDC(hMemDC);
        //    /* 더블 버퍼링 끝 */
        //    EndPaint(hwnd, &ps); //그리기 종료
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

    //윈도우 프로시져에서 처리되지 않은 나머지 메세지를 처리해준다
    return (DefWindowProc(hwnd, iMessage, wParam, lParam));
}
