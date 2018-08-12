#include "stdafx.h"
#include "mainGame.h"

HINSTANCE _hInstance; //프로그램 인스턴스(메모리상에 할당되어 실행중인 프로그램)
HWND _hWnd; //윈도우 핸들

MainGame* mg;

POINT ptMouse = { 0, 0 };

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void setWindowSize(int x, int y, int width, int height);

/*
hInstance: 프로그램의 인스턴스 핸들
hPrevInstance: 이전에 실행된 인스턴스 핸들
lpszcmdParam: 명령행으로 입력된 프로그램 인수
cmd == comman Param == Parameter
*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszcmParam, int cmdShow)
{
    MSG message; //윈도우 메세지
    WNDCLASS wndClass; //윈도우 클래스(정보)
                       /*메인 게임 할당*/
    mg = new MainGame;

    _hInstance = hInstance;

    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hInstance = hInstance;
    wndClass.lpfnWndProc = (WNDPROC)WndProc;
    wndClass.lpszClassName = WINNAME;
    wndClass.lpszMenuName = NULL;
    wndClass.style = CS_HREDRAW | CS_VREDRAW;

    //윈도우 정보등록하자
    RegisterClass(&wndClass);
    _hWnd = CreateWindow
    (WINNAME, WINNAME,
        WINSTYLE,
        WINSTARTX,
        WINSTARTY,
        WINSIZEX,
        WINSIZEY,
        NULL,
        (HMENU)NULL,
        hInstance,
        NULL);

    //윈도우 창 크기 설정
    setWindowSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);
    //윈도우 창 띄어라
    ShowWindow(_hWnd, cmdShow);

    /*메인게임 동적할당 실패시*/
    if (FAILED(mg->init()))
    {
        return 0;
    }

    /*
    PeekMessage: 메세지가 들어오든 말든 상시로 돌고있음
    GetMessage: 메세지가 들어와야 전달함
    */
    //while (GetMessage(&message, 0, 0, 0))
    //{
    //    TranslateMessage(&message); //키보드를 입력하는 순간 그 메세지 처리담당
    //    DispatchMessage(&message); //실제로 윈도우에 메세지 전달을 담당
    //}
    /* 고성능 타이머 적용 시킨 부분 */
    while (true)
    {
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            if (message.message == WM_QUIT) break;
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        else
        {
            TIMEMANAGER->update(100.f);
            mg->update();
            mg->render();
        }
    }


    /*메인게임 클래스 해제*/
    mg->release();
    //윈도우 클래스 등록해제
    UnregisterClass(WINNAME, hInstance);
    return message.wParam;
}

//윈도우 프로시저: 메세지를 운영체제에 전달, 강제로 운영체제가 호출한다
//hWnd: 어느 윈도우에서 발생한 메세지인지를 구분해준다
//iMessage: 메세지 구분번호
//wParam: unsigned int 마우스 버튼의 상태, 키보드 조합키의 상태를 전달한다
//lParam: unsigned long 마우스 클릭 좌표를 전달
//CALLBACK: 어떤 이벤트가 발생했을 때 나에게 알려달라고 하는 것(즉, 호출 되는 것이라고 봐야함)
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    //메인게임 클래스 내부의 메인프로시저를 실행시킴
    return mg->MainProc(hWnd, iMessage, wParam, lParam);
}

void setWindowSize(int x, int y, int width, int height)
{
    RECT rc;
    rc.left = 0;
    rc.top = 0;
    rc.right = width;
    rc.bottom = height;
    //실제 윈도우 크기 조정
    AdjustWindowRect(&rc, WINSTYLE, false);
    SetWindowPos(_hWnd, NULL, x, y, (rc.right - rc.left), (rc.bottom - rc.top), SWP_NOZORDER | SWP_NOMOVE);
}