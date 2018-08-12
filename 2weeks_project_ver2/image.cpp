#include "stdafx.h"
#include "image.h"
#pragma comment(lib, "msimg32.lib")
/*
1. bmp: 제약이 많지만, 무손실 무압축이므로 속도가 가장 빠르다
2. jpg: 손실 압축이므로 원래가진 이미지의 손실이 일어난다. 어떠한 알고리즘에 의해 압축이 되어 용량이 작다
3. png: 비손실 압축, 8비트 알파 채널을 이용한 다양한 투명층을 지원한다
*/

/*
DC란???
- Device Context
- 출력에 필요한 모든 정보를 가지는 데이터 구조체 입니다.
- GDI에 의해 관리가 됩니다.
- 어떤 폰트, 색상, 굵기, 출력방법 등이 모두 출력에 필요한 정보로써 그러한 정보를 다 담고 있습니다.

HDC란???
- Handle Device Context
- 결국 HDC 란 DC의 정보를 저장하는 데이터 구조체의 위치를 알기위함.
- HDC는 포인터가 아니다 . 항상 실제 객체의 메모리 주소를 가르킬 뿐이다.
-> 물리적으로 메모리 주소가 바뀌어도 , DC의 실제위치를 찾아감.
*/

Image::Image()
    : m_imageInfo(nullptr)
    , m_fileName(nullptr)
    , m_isTrans(NULL)
    , m_transColor(BLACK)
    , m_blendImage(nullptr)
{
}

Image::~Image()
{
}

//빈 비트맵 이미지를 만든다
HRESULT Image::init(int width, int height)
{
    // 재초기화 방지용, 이미지 정보에 값이 있다면 릴리즈
    if (m_imageInfo != nullptr) release();
    // DC가져오기, 현재 윈도우에 대한 화면 DC를 임시적으로 받아옴
    HDC hdc = GetDC(_hWnd);
    // CreateCompatibleDC -  비트맵을 출력하기 위해 메모리DC를 만들어 주는 함수
    // CreateCompatibleBitmap - 원본 DC와 호환되는 비트맵을 생성해주는 함수
    // selectObject - 기존의 비트맵 핸들을 반환하고, 새로운 비트맵을 메모리 DC가 선택하고, 메모리 DC에 비트맵이 그려짐

    // 이미지 정보를 생성하자
    m_imageInfo = new IMAGE_INFO;
    m_imageInfo->loadType = LOAD_EMPTY;
    m_imageInfo->resID = 0;
    m_imageInfo->hMemDC = CreateCompatibleDC(hdc);
    m_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
    m_imageInfo->hOBit = (HBITMAP)SelectObject(m_imageInfo->hMemDC, m_imageInfo->hBit);
    m_imageInfo->width = width;
    m_imageInfo->height = height;

    // 파일 이름
    m_fileName = nullptr;

    //알파 블렌드 옵션 설정
    m_blendFunc.BlendFlags = 0;
    m_blendFunc.AlphaFormat = 0;
    m_blendFunc.BlendOp = AC_SRC_OVER;

    //알파 블렌드 사용을 위한 이미지 설정
    m_blendImage = new IMAGE_INFO;
    m_blendImage->loadType = LOAD_EMPTY;
    m_blendImage->resID = 0;
    m_blendImage->hMemDC = CreateCompatibleDC(hdc);
    m_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
    m_blendImage->hOBit = (HBITMAP)SelectObject(m_blendImage->hMemDC, m_blendImage->hBit);
    m_blendImage->width = WINSIZEX;
    m_blendImage->height = WINSIZEY;

    // 투명키 컬러 세팅
    m_isTrans = false;
    m_transColor = BLACK;

    ReleaseDC(_hWnd, hdc); //DC해제

                           // 리소스를 얻어오는 데 실패했을 경우
    if (m_imageInfo->hBit == 0)
    {
        release();
        return E_FAIL;
    }
    return S_OK;
}

// 리소스로부터 읽어온다
HRESULT Image::init(const DWORD resID, int width, int height, bool isTrans, COLORREF transColor)
{
    // 재초기화 방지용, 이미지 정보에 값이 있다면 릴리즈
    if (m_imageInfo != nullptr) release();
    // DC가져오기, 현재 윈도우에 대한 화면 DC를 임시적으로 받아옴
    HDC hdc = GetDC(_hWnd);
    // CreateCompatibleDC -  비트맵을 출력하기 위해 메모리DC를 만들어 주는 함수
    // CreateCompatibleBitmap - 원본 DC와 호환되는 비트맵을 생성해주는 함수
    // selectObject - 기존의 비트맵 핸들을 반환하고, 새로운 비트맵을 메모리 DC가 선택하고, 메모리 DC에 비트맵이 그려짐

    // 이미지 정보를 생성하자
    m_imageInfo = new IMAGE_INFO;
    m_imageInfo->loadType = LOAD_RESOURCE;
    m_imageInfo->resID = resID;
    m_imageInfo->hMemDC = CreateCompatibleDC(hdc);
    m_imageInfo->hBit = (HBITMAP)LoadBitmap(_hInstance, MAKEINTRESOURCE(m_imageInfo->resID)); // 파일을 비트맵에 올린다 
    m_imageInfo->hOBit = (HBITMAP)SelectObject(m_imageInfo->hMemDC, m_imageInfo->hBit);
    m_imageInfo->width = width;
    m_imageInfo->height = height;

    // 파일 이름
    m_fileName = nullptr;

    //알파 블렌드 옵션 설정
    m_blendFunc.BlendFlags = 0;
    m_blendFunc.AlphaFormat = 0;
    m_blendFunc.BlendOp = AC_SRC_OVER;

    //알파 블렌드 사용을 위한 이미지 설정
    m_blendImage = new IMAGE_INFO;
    m_blendImage->loadType = LOAD_EMPTY;
    m_blendImage->resID = 0;
    m_blendImage->hMemDC = CreateCompatibleDC(hdc);
    m_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
    m_blendImage->hOBit = (HBITMAP)SelectObject(m_blendImage->hMemDC, m_blendImage->hBit);
    m_blendImage->width = WINSIZEX;
    m_blendImage->height = WINSIZEY;

    // 투명키 컬러 세팅
    m_isTrans = isTrans;
    m_transColor = transColor;

    ReleaseDC(_hWnd, hdc); // DC해제

                           // 리소스를 얻어오는 데 실패했을 경우
    if (m_imageInfo->hBit == 0)
    {
        release();
        return E_FAIL;
    }
    return S_OK;
}

// 이미지 설정 (파일로부터 읽어온다)
HRESULT Image::init(const char * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
    //파일경로가 잘못되었으면
    if (fileName == nullptr) return E_FAIL;

    // 재초기화 방지용, 이미지 정보에 값이 있다면 릴리즈
    if (m_imageInfo != nullptr) release();
    // DC가져오기, 현재 윈도우에 대한 화면 DC를 임시적으로 받아옴
    HDC hdc = GetDC(_hWnd);
    // CreateCompatibleDC -  비트맵을 출력하기 위해 메모리DC를 만들어 주는 함수
    // CreateCompatibleBitmap - 원본 DC와 호환되는 비트맵을 생성해주는 함수
    // selectObject - 기존의 비트맵 핸들을 반환하고, 새로운 비트맵을 메모리 DC가 선택하고, 메모리 DC에 비트맵이 그려짐

    // 이미지 정보를 생성하자
    m_imageInfo = new IMAGE_INFO;
    m_imageInfo->loadType = LOAD_FILE;
    m_imageInfo->resID = 0;
    m_imageInfo->hMemDC = CreateCompatibleDC(hdc);
    m_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    m_imageInfo->hOBit = (HBITMAP)SelectObject(m_imageInfo->hMemDC, m_imageInfo->hBit);
    m_imageInfo->width = width;
    m_imageInfo->height = height;

    // 파일 이름
    size_t len = strlen(fileName);
    m_fileName = new char[len + 1];
    strcpy_s(m_fileName, len + 1, fileName);

    //알파 블렌드 옵션 설정
    m_blendFunc.BlendFlags = 0;
    m_blendFunc.AlphaFormat = 0;
    m_blendFunc.BlendOp = AC_SRC_OVER;

    //알파 블렌드 사용을 위한 이미지 설정
    m_blendImage = new IMAGE_INFO;
    m_blendImage->loadType = LOAD_EMPTY;
    m_blendImage->resID = 0;
    m_blendImage->hMemDC = CreateCompatibleDC(hdc);
    m_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
    m_blendImage->hOBit = (HBITMAP)SelectObject(m_blendImage->hMemDC, m_blendImage->hBit);
    m_blendImage->width = WINSIZEX;
    m_blendImage->height = WINSIZEY;

    // 투명키 컬러 세팅
    m_isTrans = isTrans;
    m_transColor = transColor;

    ReleaseDC(_hWnd, hdc); // DC해제

                           // 리소스를 얻어오는 데 실패했을 경우
    if (m_imageInfo->hBit == 0)
    {
        release();
        return E_FAIL;
    }
    return S_OK;
}

// 이미지 + 프레임 파일 초기화
HRESULT Image::init(const char * fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
    //파일경로가 잘못되었으면
    if (fileName == nullptr) return E_FAIL;

    // 재초기화 방지용, 이미지 정보에 값이 있다면 릴리즈
    if (m_imageInfo != nullptr) release();
    // DC가져오기, 현재 윈도우에 대한 화면 DC를 임시적으로 받아옴
    HDC hdc = GetDC(_hWnd);
    // CreateCompatibleDC -  비트맵을 출력하기 위해 메모리DC를 만들어 주는 함수
    // CreateCompatibleBitmap - 원본 DC와 호환되는 비트맵을 생성해주는 함수
    // selectObject - 기존의 비트맵 핸들을 반환하고, 새로운 비트맵을 메모리 DC가 선택하고, 메모리 DC에 비트맵이 그려짐

    // 이미지 정보를 생성하자
    m_imageInfo = new IMAGE_INFO;
    m_imageInfo->loadType = LOAD_FILE;
    m_imageInfo->resID = 0;
    m_imageInfo->hMemDC = CreateCompatibleDC(hdc);
    m_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    m_imageInfo->hOBit = (HBITMAP)SelectObject(m_imageInfo->hMemDC, m_imageInfo->hBit);
    m_imageInfo->width = width;
    m_imageInfo->height = height;
    m_imageInfo->maxFrameX = frameX - 1;
    m_imageInfo->maxFrameY = frameY - 1;
    m_imageInfo->frameWidth = width / frameX;
    m_imageInfo->frameHeight = height / frameY;

    // 파일 이름
    size_t len = strlen(fileName);
    m_fileName = new char[len + 1];
    strcpy_s(m_fileName, len + 1, fileName);

    //알파 블렌드 옵션 설정
    m_blendFunc.BlendFlags = 0;
    m_blendFunc.AlphaFormat = 0;
    m_blendFunc.BlendOp = AC_SRC_OVER;

    //알파 블렌드 사용을 위한 이미지 설정
    m_blendImage = new IMAGE_INFO;
    m_blendImage->loadType = LOAD_EMPTY;
    m_blendImage->resID = 0;
    m_blendImage->hMemDC = CreateCompatibleDC(hdc);
    m_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
    m_blendImage->hOBit = (HBITMAP)SelectObject(m_blendImage->hMemDC, m_blendImage->hBit);
    m_blendImage->width = WINSIZEX;
    m_blendImage->height = WINSIZEY;

    // 투명키 컬러 세팅
    m_isTrans = isTrans;
    m_transColor = transColor;

    ReleaseDC(_hWnd, hdc); // DC해제

                           // 리소스를 얻어오는 데 실패했을 경우
    if (m_imageInfo->hBit == 0)
    {
        release();
        return E_FAIL;
    }
    return S_OK;
}

HRESULT Image::init(const char * fileName, int x, int y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
    //파일경로가 잘못되었으면
    if (fileName == nullptr) return E_FAIL;

    // 재초기화 방지용, 이미지 정보에 값이 있다면 릴리즈
    if (m_imageInfo != nullptr) release();
    // DC가져오기, 현재 윈도우에 대한 화면 DC를 임시적으로 받아옴
    HDC hdc = GetDC(_hWnd);
    // CreateCompatibleDC -  비트맵을 출력하기 위해 메모리DC를 만들어 주는 함수
    // CreateCompatibleBitmap - 원본 DC와 호환되는 비트맵을 생성해주는 함수
    // selectObject - 기존의 비트맵 핸들을 반환하고, 새로운 비트맵을 메모리 DC가 선택하고, 메모리 DC에 비트맵이 그려짐

    // 이미지 정보를 생성하자
    m_imageInfo = new IMAGE_INFO;
    m_imageInfo->loadType = LOAD_FILE;
    m_imageInfo->resID = 0;
    m_imageInfo->hMemDC = CreateCompatibleDC(hdc);
    m_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    m_imageInfo->hOBit = (HBITMAP)SelectObject(m_imageInfo->hMemDC, m_imageInfo->hBit);
    m_imageInfo->x = static_cast<float>(x) - (width / frameX / 2.f);
    m_imageInfo->y = static_cast<float>(y) - (height / frameY / 2.f);
    m_imageInfo->width = width;
    m_imageInfo->height = height;
    m_imageInfo->maxFrameX = frameX - 1;
    m_imageInfo->maxFrameY = frameY - 1;
    m_imageInfo->frameWidth = width / frameX;
    m_imageInfo->frameHeight = height / frameY;

    // 파일 이름
    size_t len = strlen(fileName);
    m_fileName = new char[len + 1];
    strcpy_s(m_fileName, len + 1, fileName);

    //알파 블렌드 옵션 설정
    m_blendFunc.BlendFlags = 0;
    m_blendFunc.AlphaFormat = 0;
    m_blendFunc.BlendOp = AC_SRC_OVER;

    //알파 블렌드 사용을 위한 이미지 설정
    m_blendImage = new IMAGE_INFO;
    m_blendImage->loadType = LOAD_EMPTY;
    m_blendImage->resID = 0;
    m_blendImage->hMemDC = CreateCompatibleDC(hdc);
    m_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
    m_blendImage->hOBit = (HBITMAP)SelectObject(m_blendImage->hMemDC, m_blendImage->hBit);
    m_blendImage->width = WINSIZEX;
    m_blendImage->height = WINSIZEY;

    // 투명키 컬러 세팅
    m_isTrans = isTrans;
    m_transColor = transColor;

    ReleaseDC(_hWnd, hdc); // DC해제

                           // 리소스를 얻어오는 데 실패했을 경우
    if (m_imageInfo->hBit == 0)
    {
        release();
        return E_FAIL;
    }
    return S_OK;
}

void Image::setTransColor(bool isTrans, COLORREF transColor)
{
    m_isTrans = isTrans;
    m_transColor = transColor;
}

void Image::release()
{
    // 이미지 정보가 남아있다면, 릴리즈를 하자
    if (m_imageInfo)
    {
        //이미지 삭제
        SelectObject(m_imageInfo->hMemDC, m_imageInfo->hBit);
        DeleteObject(m_imageInfo->hOBit);
        DeleteObject(m_imageInfo->hMemDC);

        //포인터 삭제
        SAFE_DELETE(m_imageInfo);
        SAFE_DELETE(m_fileName);

        //투명키 초기화
        m_isTrans = false;
        m_transColor = BLACK;
    }
}

void Image::render(HDC hdc)
{
    // 배경색 없앨거냐
    if (m_isTrans)
    {
        // GdiTransparentBlt - 비트맵을 불러올 때 특정 색을 제외하고 복사하는 함수
        // 인자값
        // 복사될 장소의 DC, 복사될 좌표의 시작점 X와 Y, 복사될 이미지의 가로와 세로 크기
        // 복사될 대상의 DC, 복사시작지점의  X와 Y, 복사영역의 가로와 세로크기,
        // 복사할 때의 제외할 색상
        GdiTransparentBlt(
            hdc,
            0,
            0,
            m_imageInfo->width,
            m_imageInfo->height,
            m_imageInfo->hMemDC,
            0, 0, m_imageInfo->width,
            m_imageInfo->height,
            m_transColor);
    }
    //배경색 안날리고 원본이미지 그대로 뽑아낼거냐
    else
    {
        // SRCCOPY - 복사해주는 놈, 가로세로를 재정의 해서 복사함
        // DC간 영역끼리 고속 복사하는 녀석
        BitBlt(
            hdc,
            0,
            0,
            m_imageInfo->width,
            m_imageInfo->height,
            m_imageInfo->hMemDC,
            0,
            0,
            SRCCOPY);
    }
}

void Image::render(HDC hdc, int destX, int destY)
{
    // 배경색 없앨거냐
    if (m_isTrans)
    {
        // GdiTransparentBlt - 비트맵을 불러올 때 특정 색을 제외하고 복사하는 함수
        // 인자값
        // 복사될 장소의 DC, 복사될 좌표의 시작점 X와 Y, 복사될 이미지의 가로와 세로 크기
        // 복사될 대상의 DC, 복사시작지점의  X와 Y, 복사영역의 가로와 세로크기,
        // 복사할 때의 제외할 색상
        GdiTransparentBlt(
            hdc,
            destX,
            destY,
            m_imageInfo->width,
            m_imageInfo->height,
            m_imageInfo->hMemDC,
            0,
            0,
            m_imageInfo->width,
            m_imageInfo->height,
            m_transColor);
    }
    //배경색 안날리고 원본이미지 그대로 뽑아낼거냐
    else
    {
        // SRCCOPY - 복사해주는 놈, 가로세로를 재정의 해서 복사함
        // DC간 영역끼리 고속 복사하는 녀석
        BitBlt(
            hdc,
            destX,
            destY,
            m_imageInfo->width,
            m_imageInfo->height,
            m_imageInfo->hMemDC,
            0,
            0,
            SRCCOPY);
    }
}

void Image::render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
    if (m_isTrans)
    {
        GdiTransparentBlt(hdc, destX, destY, sourWidth, sourHeight, m_imageInfo->hMemDC, sourX, sourY, sourWidth, sourHeight, m_transColor);
    }
}

void Image::renderStretch(HDC hdc, int destX, int destY, int destWidth, int destHeight, int sourX, int sourY, int sourWidth, int sourHeight)
{
    StretchBlt(hdc, destX, destY, destWidth, destHeight, m_imageInfo->hMemDC, sourX, sourY, sourWidth, sourHeight, SRCCOPY);
}


void Image::alphaRender(HDC hdc, BYTE alpha)
{
    m_blendFunc.SourceConstantAlpha = alpha;

    if (m_isTrans)
    {
        //출력해야 될 DC에 그려져 있는 내용을 blend에 그려준다
        BitBlt(m_blendImage->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, hdc, 0, 0, SRCCOPY);

        //출력해야 될 이미지를 blend에 그려준다
        GdiTransparentBlt(m_blendImage->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
            m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_transColor);

        // blendDC를 출력해야할 DC에 그린다
        AlphaBlend(hdc, 0, 0, m_imageInfo->width, m_imageInfo->height,
            m_blendImage->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_blendFunc);
    }
    else
    {
        AlphaBlend(hdc, 0, 0, m_imageInfo->width, m_imageInfo->height,
            m_blendImage->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_blendFunc);
    }
}

void Image::alphaRender(HDC hdc, int destX, int destY, BYTE alpha)
{
    m_blendFunc.SourceConstantAlpha = alpha;
    if (m_isTrans)
    {
        //출력해야 될 DC에 그려져 있는 내용을 blend에 그려준다
        BitBlt(m_blendImage->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, hdc, destX, destY, SRCCOPY);

        //출력해야 될 이미지를 blend에 그려준다
        GdiTransparentBlt(m_blendImage->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
            m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_transColor);

        // blendDC를 출력해야할 DC에 그린다
        AlphaBlend(hdc, destX, destY, m_imageInfo->width, m_imageInfo->height,
            m_blendImage->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_blendFunc);
    }
    else
    {
        AlphaBlend(hdc, destX, destY, m_imageInfo->width, m_imageInfo->height,
            m_blendImage->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_blendFunc);
    }
}

void Image::alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha)
{
    m_blendFunc.SourceConstantAlpha = alpha;
    if (m_isTrans)
    {
        //출력해야 될 DC에 그려져 있는 내용을 blend에 그려준다
        BitBlt(m_blendImage->hMemDC, destX, destY, m_imageInfo->width, m_imageInfo->height, hdc, sourX, sourY, SRCCOPY);

        //출력해야 될 이미지를 blend에 그려준다
        GdiTransparentBlt(m_blendImage->hMemDC, destX, destY, sourWidth, sourHeight,
            m_imageInfo->hMemDC, sourX, sourY, sourWidth, sourHeight, m_transColor);

        // blendDC를 출력해야할 DC에 그린다
        AlphaBlend(hdc, destX, destY, sourWidth, sourHeight,
            m_blendImage->hMemDC, sourX, sourY, sourWidth, sourHeight, m_blendFunc);
    }
    else
    {
        AlphaBlend(hdc, destX, destY, sourWidth, sourHeight,
            m_blendImage->hMemDC, sourX, sourY, sourWidth, sourHeight, m_blendFunc);
    }

}

void Image::frameRender(HDC hdc, int destX, int destY)
{
    if (m_isTrans)
    {
        // GdiTransparentBlt - 비트맵을 불러올 때 특정 색을 제외하고 복사하는 함수
        // 인자값
        // 복사될 장소의 DC, 복사될 좌표의 시작점 X와 Y, 복사될 이미지의 가로와 세로 크기
        // 복사될 대상의 DC, 복사시작지점의  X와 Y, 복사영역의 가로와 세로크기,
        // 복사할 때의 제외할 색상
        GdiTransparentBlt(
            hdc,
            destX,
            destY,
            m_imageInfo->frameWidth,
            m_imageInfo->frameHeight,
            m_imageInfo->hMemDC, // 복사될 대상 DC
            m_imageInfo->currentFrameX * m_imageInfo->frameWidth, // 복사 시작지점의 x
            m_imageInfo->currentFrameY * m_imageInfo->frameHeight, // 복사 시작지점의 y
            m_imageInfo->frameWidth, // 복사 영역 가로 크기
            m_imageInfo->frameHeight, // 복사 영역 세로 크기
            m_transColor);
    }
    //배경색 안날리고 원본이미지 그대로 뽑아낼거냐
    else
    {
        // SRCCOPY - 복사해주는 놈, 가로세로를 재정의 해서 복사함
        // DC간 영역끼리 고속 복사하는 녀석
        BitBlt(
            hdc,
            destX,
            destY,
            m_imageInfo->frameWidth,
            m_imageInfo->frameHeight,
            m_imageInfo->hMemDC,
            m_imageInfo->currentFrameX * m_imageInfo->frameWidth,
            m_imageInfo->currentFrameY * m_imageInfo->frameHeight,
            SRCCOPY);
    }
}

void Image::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
    m_imageInfo->currentFrameX = currentFrameX;
    m_imageInfo->currentFrameY = currentFrameY;

    if (currentFrameX > m_imageInfo->maxFrameX)
    {
        m_imageInfo->currentFrameX = m_imageInfo->maxFrameX;
    }
    if (currentFrameY > m_imageInfo->maxFrameY)
    {
        m_imageInfo->currentFrameY = m_imageInfo->maxFrameY;
    }

    //배경색 없앨거냐
    if (m_isTrans)
    {
        //GdiTransparentBlt : 비트맵을 불러올때 특정 색을 제외하고 복사하는 함수
        //인자값 :  복사될 장소의 DC, 복사될 좌표의 시작점 X와 Y, 복사될 이미지의 가로크기와 세로크기, 복사될 대상의 DC, 복사시작지점 X와Y, 복사 영역의 가로크기, 세로크기, 복사할때의 제외할 색상
        GdiTransparentBlt(
            hdc,
            destX,
            destY,
            m_imageInfo->frameWidth,
            m_imageInfo->frameHeight,
            m_imageInfo->hMemDC,	//복사될 대상 DC
            m_imageInfo->currentFrameX * m_imageInfo->frameWidth, //복사 시작지점
            m_imageInfo->currentFrameY * m_imageInfo->frameHeight, //복사 시작지점
            m_imageInfo->frameWidth, //복사 영역 가로크기
            m_imageInfo->frameHeight, //복사 영역 세로크기
            m_transColor);
    }
    //배경색 안날리고 원본이미지 그대로 뽑아낼거냐
    else
    {
        // SRCCOPY: 복사해주는 놈, 가로세로를 재정의 해서 복사함
        // DC간 영역끼리 고속복사 하는 녀석
        BitBlt(
            hdc,
            destX,
            destY,
            m_imageInfo->frameWidth,
            m_imageInfo->frameHeight,
            m_imageInfo->hMemDC,
            m_imageInfo->currentFrameX * m_imageInfo->frameWidth,
            m_imageInfo->currentFrameY * m_imageInfo->frameHeight,
            SRCCOPY);
    }
}

void Image::loopRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY)
{
    // offset값이 음수인 경우 보정해준다
    if (offsetX < 0) offsetX = m_imageInfo->width + (offsetX % m_imageInfo->width);
    if (offsetY < 0) offsetY = m_imageInfo->height + (offsetY % m_imageInfo->height);

    // 그려지는 소스의 영역(이미지) 세팅할 변수
    RECT rcSour;
    int sourWidth;
    int sourHeight;

    // 그려지는 영역의 DC
    RECT rcDest;

    // 그려야할 영역의 전체 영역
    int drawAreaX = drawArea->left;
    int drawAreaY = drawArea->top;
    int drawAreaW = drawArea->right - drawArea->left;
    int drawAreaH = drawArea->bottom - drawArea->top;

    //세로 루프 세팅
    for (int y = 0; y < drawAreaH; y += sourHeight)
    {
        //소스(이미지 sour)영역의 높이 계산
        rcSour.top = (y + offsetY) % m_imageInfo->height;
        rcSour.bottom = m_imageInfo->height;
        sourHeight = rcSour.bottom - rcSour.top;

        //소스의 영역이 그리기 화면을 넘어갔다면(화면 밖으로 이미지가 나갔을 때)
        if (y + sourHeight > drawAreaH)
        {
            //넘어간 그림의 값만큼 바텀값을 올려주자
            rcSour.bottom -= (y + sourHeight) - drawAreaH;
            sourHeight = rcSour.bottom - rcSour.top;
        }

        //그려지는 영역
        rcDest.top = y + drawAreaY;
        rcDest.bottom = rcDest.top + sourHeight;

        //가로 루프 돌자
        for (int x = 0; x < drawAreaW; x += sourWidth)
        {
            //소스의 영역 가로 계산
            rcSour.left = (x + offsetX % m_imageInfo->width);
            rcSour.right = m_imageInfo->width;
            sourWidth = rcSour.right - rcSour.left;

            if (x + sourWidth > drawAreaW)
            {
                //넘어간 만큼 right값을 왼쪽으로
                rcSour.right -= (x + sourWidth) - drawAreaW;
                sourWidth = rcSour.right - rcSour.left;
            }

            //그려지는 영역
            rcDest.left = x + drawAreaX;
            rcDest.right = rcDest.left + sourWidth;

            //그려주자
            render(hdc, rcDest.left, rcDest.top, rcSour.left, rcSour.top, rcSour.right - rcSour.left, rcSour.bottom - rcSour.top);
        }

    }
}