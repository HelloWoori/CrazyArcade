#include "stdafx.h"
#include "mainGame.h"

HINSTANCE _hInstance; //���α׷� �ν��Ͻ�(�޸𸮻� �Ҵ�Ǿ� �������� ���α׷�)
HWND _hWnd; //������ �ڵ�

MainGame* mg;

POINT ptMouse = { 0, 0 };

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void setWindowSize(int x, int y, int width, int height);

/*
hInstance: ���α׷��� �ν��Ͻ� �ڵ�
hPrevInstance: ������ ����� �ν��Ͻ� �ڵ�
lpszcmdParam: ��������� �Էµ� ���α׷� �μ�
cmd == comman Param == Parameter
*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszcmParam, int cmdShow)
{
    MSG message; //������ �޼���
    WNDCLASS wndClass; //������ Ŭ����(����)
                       /*���� ���� �Ҵ�*/
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

    //������ �����������
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

    //������ â ũ�� ����
    setWindowSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);
    //������ â ����
    ShowWindow(_hWnd, cmdShow);

    /*���ΰ��� �����Ҵ� ���н�*/
    if (FAILED(mg->init()))
    {
        return 0;
    }

    /*
    PeekMessage: �޼����� ������ ���� ��÷� ��������
    GetMessage: �޼����� ���;� ������
    */
    //while (GetMessage(&message, 0, 0, 0))
    //{
    //    TranslateMessage(&message); //Ű���带 �Է��ϴ� ���� �� �޼��� ó�����
    //    DispatchMessage(&message); //������ �����쿡 �޼��� ������ ���
    //}
    /* ���� Ÿ�̸� ���� ��Ų �κ� */
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


    /*���ΰ��� Ŭ���� ����*/
    mg->release();
    //������ Ŭ���� �������
    UnregisterClass(WINNAME, hInstance);
    return message.wParam;
}

//������ ���ν���: �޼����� �ü���� ����, ������ �ü���� ȣ���Ѵ�
//hWnd: ��� �����쿡�� �߻��� �޼��������� �������ش�
//iMessage: �޼��� ���й�ȣ
//wParam: unsigned int ���콺 ��ư�� ����, Ű���� ����Ű�� ���¸� �����Ѵ�
//lParam: unsigned long ���콺 Ŭ�� ��ǥ�� ����
//CALLBACK: � �̺�Ʈ�� �߻����� �� ������ �˷��޶�� �ϴ� ��(��, ȣ�� �Ǵ� ���̶�� ������)
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    //���ΰ��� Ŭ���� ������ �������ν����� �����Ŵ
    return mg->MainProc(hWnd, iMessage, wParam, lParam);
}

void setWindowSize(int x, int y, int width, int height)
{
    RECT rc;
    rc.left = 0;
    rc.top = 0;
    rc.right = width;
    rc.bottom = height;
    //���� ������ ũ�� ����
    AdjustWindowRect(&rc, WINSTYLE, false);
    SetWindowPos(_hWnd, NULL, x, y, (rc.right - rc.left), (rc.bottom - rc.top), SWP_NOZORDER | SWP_NOMOVE);
}