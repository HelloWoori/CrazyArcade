#include "stdafx.h"
#include "image.h"
#pragma comment(lib, "msimg32.lib")
/*
1. bmp: ������ ������, ���ս� �������̹Ƿ� �ӵ��� ���� ������
2. jpg: �ս� �����̹Ƿ� �������� �̹����� �ս��� �Ͼ��. ��� �˰��� ���� ������ �Ǿ� �뷮�� �۴�
3. png: ��ս� ����, 8��Ʈ ���� ä���� �̿��� �پ��� �������� �����Ѵ�
*/

/*
DC��???
- Device Context
- ��¿� �ʿ��� ��� ������ ������ ������ ����ü �Դϴ�.
- GDI�� ���� ������ �˴ϴ�.
- � ��Ʈ, ����, ����, ��¹�� ���� ��� ��¿� �ʿ��� �����ν� �׷��� ������ �� ��� �ֽ��ϴ�.

HDC��???
- Handle Device Context
- �ᱹ HDC �� DC�� ������ �����ϴ� ������ ����ü�� ��ġ�� �˱�����.
- HDC�� �����Ͱ� �ƴϴ� . �׻� ���� ��ü�� �޸� �ּҸ� ����ų ���̴�.
-> ���������� �޸� �ּҰ� �ٲ� , DC�� ������ġ�� ã�ư�.
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

//�� ��Ʈ�� �̹����� �����
HRESULT Image::init(int width, int height)
{
    // ���ʱ�ȭ ������, �̹��� ������ ���� �ִٸ� ������
    if (m_imageInfo != nullptr) release();
    // DC��������, ���� �����쿡 ���� ȭ�� DC�� �ӽ������� �޾ƿ�
    HDC hdc = GetDC(_hWnd);
    // CreateCompatibleDC -  ��Ʈ���� ����ϱ� ���� �޸�DC�� ����� �ִ� �Լ�
    // CreateCompatibleBitmap - ���� DC�� ȣȯ�Ǵ� ��Ʈ���� �������ִ� �Լ�
    // selectObject - ������ ��Ʈ�� �ڵ��� ��ȯ�ϰ�, ���ο� ��Ʈ���� �޸� DC�� �����ϰ�, �޸� DC�� ��Ʈ���� �׷���

    // �̹��� ������ ��������
    m_imageInfo = new IMAGE_INFO;
    m_imageInfo->loadType = LOAD_EMPTY;
    m_imageInfo->resID = 0;
    m_imageInfo->hMemDC = CreateCompatibleDC(hdc);
    m_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
    m_imageInfo->hOBit = (HBITMAP)SelectObject(m_imageInfo->hMemDC, m_imageInfo->hBit);
    m_imageInfo->width = width;
    m_imageInfo->height = height;

    // ���� �̸�
    m_fileName = nullptr;

    //���� ���� �ɼ� ����
    m_blendFunc.BlendFlags = 0;
    m_blendFunc.AlphaFormat = 0;
    m_blendFunc.BlendOp = AC_SRC_OVER;

    //���� ���� ����� ���� �̹��� ����
    m_blendImage = new IMAGE_INFO;
    m_blendImage->loadType = LOAD_EMPTY;
    m_blendImage->resID = 0;
    m_blendImage->hMemDC = CreateCompatibleDC(hdc);
    m_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
    m_blendImage->hOBit = (HBITMAP)SelectObject(m_blendImage->hMemDC, m_blendImage->hBit);
    m_blendImage->width = WINSIZEX;
    m_blendImage->height = WINSIZEY;

    // ����Ű �÷� ����
    m_isTrans = false;
    m_transColor = BLACK;

    ReleaseDC(_hWnd, hdc); //DC����

                           // ���ҽ��� ������ �� �������� ���
    if (m_imageInfo->hBit == 0)
    {
        release();
        return E_FAIL;
    }
    return S_OK;
}

// ���ҽ��κ��� �о�´�
HRESULT Image::init(const DWORD resID, int width, int height, bool isTrans, COLORREF transColor)
{
    // ���ʱ�ȭ ������, �̹��� ������ ���� �ִٸ� ������
    if (m_imageInfo != nullptr) release();
    // DC��������, ���� �����쿡 ���� ȭ�� DC�� �ӽ������� �޾ƿ�
    HDC hdc = GetDC(_hWnd);
    // CreateCompatibleDC -  ��Ʈ���� ����ϱ� ���� �޸�DC�� ����� �ִ� �Լ�
    // CreateCompatibleBitmap - ���� DC�� ȣȯ�Ǵ� ��Ʈ���� �������ִ� �Լ�
    // selectObject - ������ ��Ʈ�� �ڵ��� ��ȯ�ϰ�, ���ο� ��Ʈ���� �޸� DC�� �����ϰ�, �޸� DC�� ��Ʈ���� �׷���

    // �̹��� ������ ��������
    m_imageInfo = new IMAGE_INFO;
    m_imageInfo->loadType = LOAD_RESOURCE;
    m_imageInfo->resID = resID;
    m_imageInfo->hMemDC = CreateCompatibleDC(hdc);
    m_imageInfo->hBit = (HBITMAP)LoadBitmap(_hInstance, MAKEINTRESOURCE(m_imageInfo->resID)); // ������ ��Ʈ�ʿ� �ø��� 
    m_imageInfo->hOBit = (HBITMAP)SelectObject(m_imageInfo->hMemDC, m_imageInfo->hBit);
    m_imageInfo->width = width;
    m_imageInfo->height = height;

    // ���� �̸�
    m_fileName = nullptr;

    //���� ���� �ɼ� ����
    m_blendFunc.BlendFlags = 0;
    m_blendFunc.AlphaFormat = 0;
    m_blendFunc.BlendOp = AC_SRC_OVER;

    //���� ���� ����� ���� �̹��� ����
    m_blendImage = new IMAGE_INFO;
    m_blendImage->loadType = LOAD_EMPTY;
    m_blendImage->resID = 0;
    m_blendImage->hMemDC = CreateCompatibleDC(hdc);
    m_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
    m_blendImage->hOBit = (HBITMAP)SelectObject(m_blendImage->hMemDC, m_blendImage->hBit);
    m_blendImage->width = WINSIZEX;
    m_blendImage->height = WINSIZEY;

    // ����Ű �÷� ����
    m_isTrans = isTrans;
    m_transColor = transColor;

    ReleaseDC(_hWnd, hdc); // DC����

                           // ���ҽ��� ������ �� �������� ���
    if (m_imageInfo->hBit == 0)
    {
        release();
        return E_FAIL;
    }
    return S_OK;
}

// �̹��� ���� (���Ϸκ��� �о�´�)
HRESULT Image::init(const char * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
    //���ϰ�ΰ� �߸��Ǿ�����
    if (fileName == nullptr) return E_FAIL;

    // ���ʱ�ȭ ������, �̹��� ������ ���� �ִٸ� ������
    if (m_imageInfo != nullptr) release();
    // DC��������, ���� �����쿡 ���� ȭ�� DC�� �ӽ������� �޾ƿ�
    HDC hdc = GetDC(_hWnd);
    // CreateCompatibleDC -  ��Ʈ���� ����ϱ� ���� �޸�DC�� ����� �ִ� �Լ�
    // CreateCompatibleBitmap - ���� DC�� ȣȯ�Ǵ� ��Ʈ���� �������ִ� �Լ�
    // selectObject - ������ ��Ʈ�� �ڵ��� ��ȯ�ϰ�, ���ο� ��Ʈ���� �޸� DC�� �����ϰ�, �޸� DC�� ��Ʈ���� �׷���

    // �̹��� ������ ��������
    m_imageInfo = new IMAGE_INFO;
    m_imageInfo->loadType = LOAD_FILE;
    m_imageInfo->resID = 0;
    m_imageInfo->hMemDC = CreateCompatibleDC(hdc);
    m_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    m_imageInfo->hOBit = (HBITMAP)SelectObject(m_imageInfo->hMemDC, m_imageInfo->hBit);
    m_imageInfo->width = width;
    m_imageInfo->height = height;

    // ���� �̸�
    size_t len = strlen(fileName);
    m_fileName = new char[len + 1];
    strcpy_s(m_fileName, len + 1, fileName);

    //���� ���� �ɼ� ����
    m_blendFunc.BlendFlags = 0;
    m_blendFunc.AlphaFormat = 0;
    m_blendFunc.BlendOp = AC_SRC_OVER;

    //���� ���� ����� ���� �̹��� ����
    m_blendImage = new IMAGE_INFO;
    m_blendImage->loadType = LOAD_EMPTY;
    m_blendImage->resID = 0;
    m_blendImage->hMemDC = CreateCompatibleDC(hdc);
    m_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
    m_blendImage->hOBit = (HBITMAP)SelectObject(m_blendImage->hMemDC, m_blendImage->hBit);
    m_blendImage->width = WINSIZEX;
    m_blendImage->height = WINSIZEY;

    // ����Ű �÷� ����
    m_isTrans = isTrans;
    m_transColor = transColor;

    ReleaseDC(_hWnd, hdc); // DC����

                           // ���ҽ��� ������ �� �������� ���
    if (m_imageInfo->hBit == 0)
    {
        release();
        return E_FAIL;
    }
    return S_OK;
}

// �̹��� + ������ ���� �ʱ�ȭ
HRESULT Image::init(const char * fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
    //���ϰ�ΰ� �߸��Ǿ�����
    if (fileName == nullptr) return E_FAIL;

    // ���ʱ�ȭ ������, �̹��� ������ ���� �ִٸ� ������
    if (m_imageInfo != nullptr) release();
    // DC��������, ���� �����쿡 ���� ȭ�� DC�� �ӽ������� �޾ƿ�
    HDC hdc = GetDC(_hWnd);
    // CreateCompatibleDC -  ��Ʈ���� ����ϱ� ���� �޸�DC�� ����� �ִ� �Լ�
    // CreateCompatibleBitmap - ���� DC�� ȣȯ�Ǵ� ��Ʈ���� �������ִ� �Լ�
    // selectObject - ������ ��Ʈ�� �ڵ��� ��ȯ�ϰ�, ���ο� ��Ʈ���� �޸� DC�� �����ϰ�, �޸� DC�� ��Ʈ���� �׷���

    // �̹��� ������ ��������
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

    // ���� �̸�
    size_t len = strlen(fileName);
    m_fileName = new char[len + 1];
    strcpy_s(m_fileName, len + 1, fileName);

    //���� ���� �ɼ� ����
    m_blendFunc.BlendFlags = 0;
    m_blendFunc.AlphaFormat = 0;
    m_blendFunc.BlendOp = AC_SRC_OVER;

    //���� ���� ����� ���� �̹��� ����
    m_blendImage = new IMAGE_INFO;
    m_blendImage->loadType = LOAD_EMPTY;
    m_blendImage->resID = 0;
    m_blendImage->hMemDC = CreateCompatibleDC(hdc);
    m_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
    m_blendImage->hOBit = (HBITMAP)SelectObject(m_blendImage->hMemDC, m_blendImage->hBit);
    m_blendImage->width = WINSIZEX;
    m_blendImage->height = WINSIZEY;

    // ����Ű �÷� ����
    m_isTrans = isTrans;
    m_transColor = transColor;

    ReleaseDC(_hWnd, hdc); // DC����

                           // ���ҽ��� ������ �� �������� ���
    if (m_imageInfo->hBit == 0)
    {
        release();
        return E_FAIL;
    }
    return S_OK;
}

HRESULT Image::init(const char * fileName, int x, int y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
    //���ϰ�ΰ� �߸��Ǿ�����
    if (fileName == nullptr) return E_FAIL;

    // ���ʱ�ȭ ������, �̹��� ������ ���� �ִٸ� ������
    if (m_imageInfo != nullptr) release();
    // DC��������, ���� �����쿡 ���� ȭ�� DC�� �ӽ������� �޾ƿ�
    HDC hdc = GetDC(_hWnd);
    // CreateCompatibleDC -  ��Ʈ���� ����ϱ� ���� �޸�DC�� ����� �ִ� �Լ�
    // CreateCompatibleBitmap - ���� DC�� ȣȯ�Ǵ� ��Ʈ���� �������ִ� �Լ�
    // selectObject - ������ ��Ʈ�� �ڵ��� ��ȯ�ϰ�, ���ο� ��Ʈ���� �޸� DC�� �����ϰ�, �޸� DC�� ��Ʈ���� �׷���

    // �̹��� ������ ��������
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

    // ���� �̸�
    size_t len = strlen(fileName);
    m_fileName = new char[len + 1];
    strcpy_s(m_fileName, len + 1, fileName);

    //���� ���� �ɼ� ����
    m_blendFunc.BlendFlags = 0;
    m_blendFunc.AlphaFormat = 0;
    m_blendFunc.BlendOp = AC_SRC_OVER;

    //���� ���� ����� ���� �̹��� ����
    m_blendImage = new IMAGE_INFO;
    m_blendImage->loadType = LOAD_EMPTY;
    m_blendImage->resID = 0;
    m_blendImage->hMemDC = CreateCompatibleDC(hdc);
    m_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
    m_blendImage->hOBit = (HBITMAP)SelectObject(m_blendImage->hMemDC, m_blendImage->hBit);
    m_blendImage->width = WINSIZEX;
    m_blendImage->height = WINSIZEY;

    // ����Ű �÷� ����
    m_isTrans = isTrans;
    m_transColor = transColor;

    ReleaseDC(_hWnd, hdc); // DC����

                           // ���ҽ��� ������ �� �������� ���
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
    // �̹��� ������ �����ִٸ�, ����� ����
    if (m_imageInfo)
    {
        //�̹��� ����
        SelectObject(m_imageInfo->hMemDC, m_imageInfo->hBit);
        DeleteObject(m_imageInfo->hOBit);
        DeleteObject(m_imageInfo->hMemDC);

        //������ ����
        SAFE_DELETE(m_imageInfo);
        SAFE_DELETE(m_fileName);

        //����Ű �ʱ�ȭ
        m_isTrans = false;
        m_transColor = BLACK;
    }
}

void Image::render(HDC hdc)
{
    // ���� ���ٰų�
    if (m_isTrans)
    {
        // GdiTransparentBlt - ��Ʈ���� �ҷ��� �� Ư�� ���� �����ϰ� �����ϴ� �Լ�
        // ���ڰ�
        // ����� ����� DC, ����� ��ǥ�� ������ X�� Y, ����� �̹����� ���ο� ���� ũ��
        // ����� ����� DC, �������������  X�� Y, ���翵���� ���ο� ����ũ��,
        // ������ ���� ������ ����
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
    //���� �ȳ����� �����̹��� �״�� �̾Ƴ��ų�
    else
    {
        // SRCCOPY - �������ִ� ��, ���μ��θ� ������ �ؼ� ������
        // DC�� �������� ��� �����ϴ� �༮
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
    // ���� ���ٰų�
    if (m_isTrans)
    {
        // GdiTransparentBlt - ��Ʈ���� �ҷ��� �� Ư�� ���� �����ϰ� �����ϴ� �Լ�
        // ���ڰ�
        // ����� ����� DC, ����� ��ǥ�� ������ X�� Y, ����� �̹����� ���ο� ���� ũ��
        // ����� ����� DC, �������������  X�� Y, ���翵���� ���ο� ����ũ��,
        // ������ ���� ������ ����
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
    //���� �ȳ����� �����̹��� �״�� �̾Ƴ��ų�
    else
    {
        // SRCCOPY - �������ִ� ��, ���μ��θ� ������ �ؼ� ������
        // DC�� �������� ��� �����ϴ� �༮
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
        //����ؾ� �� DC�� �׷��� �ִ� ������ blend�� �׷��ش�
        BitBlt(m_blendImage->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, hdc, 0, 0, SRCCOPY);

        //����ؾ� �� �̹����� blend�� �׷��ش�
        GdiTransparentBlt(m_blendImage->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
            m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_transColor);

        // blendDC�� ����ؾ��� DC�� �׸���
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
        //����ؾ� �� DC�� �׷��� �ִ� ������ blend�� �׷��ش�
        BitBlt(m_blendImage->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, hdc, destX, destY, SRCCOPY);

        //����ؾ� �� �̹����� blend�� �׷��ش�
        GdiTransparentBlt(m_blendImage->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height,
            m_imageInfo->hMemDC, 0, 0, m_imageInfo->width, m_imageInfo->height, m_transColor);

        // blendDC�� ����ؾ��� DC�� �׸���
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
        //����ؾ� �� DC�� �׷��� �ִ� ������ blend�� �׷��ش�
        BitBlt(m_blendImage->hMemDC, destX, destY, m_imageInfo->width, m_imageInfo->height, hdc, sourX, sourY, SRCCOPY);

        //����ؾ� �� �̹����� blend�� �׷��ش�
        GdiTransparentBlt(m_blendImage->hMemDC, destX, destY, sourWidth, sourHeight,
            m_imageInfo->hMemDC, sourX, sourY, sourWidth, sourHeight, m_transColor);

        // blendDC�� ����ؾ��� DC�� �׸���
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
        // GdiTransparentBlt - ��Ʈ���� �ҷ��� �� Ư�� ���� �����ϰ� �����ϴ� �Լ�
        // ���ڰ�
        // ����� ����� DC, ����� ��ǥ�� ������ X�� Y, ����� �̹����� ���ο� ���� ũ��
        // ����� ����� DC, �������������  X�� Y, ���翵���� ���ο� ����ũ��,
        // ������ ���� ������ ����
        GdiTransparentBlt(
            hdc,
            destX,
            destY,
            m_imageInfo->frameWidth,
            m_imageInfo->frameHeight,
            m_imageInfo->hMemDC, // ����� ��� DC
            m_imageInfo->currentFrameX * m_imageInfo->frameWidth, // ���� ���������� x
            m_imageInfo->currentFrameY * m_imageInfo->frameHeight, // ���� ���������� y
            m_imageInfo->frameWidth, // ���� ���� ���� ũ��
            m_imageInfo->frameHeight, // ���� ���� ���� ũ��
            m_transColor);
    }
    //���� �ȳ����� �����̹��� �״�� �̾Ƴ��ų�
    else
    {
        // SRCCOPY - �������ִ� ��, ���μ��θ� ������ �ؼ� ������
        // DC�� �������� ��� �����ϴ� �༮
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

    //���� ���ٰų�
    if (m_isTrans)
    {
        //GdiTransparentBlt : ��Ʈ���� �ҷ��ö� Ư�� ���� �����ϰ� �����ϴ� �Լ�
        //���ڰ� :  ����� ����� DC, ����� ��ǥ�� ������ X�� Y, ����� �̹����� ����ũ��� ����ũ��, ����� ����� DC, ����������� X��Y, ���� ������ ����ũ��, ����ũ��, �����Ҷ��� ������ ����
        GdiTransparentBlt(
            hdc,
            destX,
            destY,
            m_imageInfo->frameWidth,
            m_imageInfo->frameHeight,
            m_imageInfo->hMemDC,	//����� ��� DC
            m_imageInfo->currentFrameX * m_imageInfo->frameWidth, //���� ��������
            m_imageInfo->currentFrameY * m_imageInfo->frameHeight, //���� ��������
            m_imageInfo->frameWidth, //���� ���� ����ũ��
            m_imageInfo->frameHeight, //���� ���� ����ũ��
            m_transColor);
    }
    //���� �ȳ����� �����̹��� �״�� �̾Ƴ��ų�
    else
    {
        // SRCCOPY: �������ִ� ��, ���μ��θ� ������ �ؼ� ������
        // DC�� �������� ��Ӻ��� �ϴ� �༮
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
    // offset���� ������ ��� �������ش�
    if (offsetX < 0) offsetX = m_imageInfo->width + (offsetX % m_imageInfo->width);
    if (offsetY < 0) offsetY = m_imageInfo->height + (offsetY % m_imageInfo->height);

    // �׷����� �ҽ��� ����(�̹���) ������ ����
    RECT rcSour;
    int sourWidth;
    int sourHeight;

    // �׷����� ������ DC
    RECT rcDest;

    // �׷����� ������ ��ü ����
    int drawAreaX = drawArea->left;
    int drawAreaY = drawArea->top;
    int drawAreaW = drawArea->right - drawArea->left;
    int drawAreaH = drawArea->bottom - drawArea->top;

    //���� ���� ����
    for (int y = 0; y < drawAreaH; y += sourHeight)
    {
        //�ҽ�(�̹��� sour)������ ���� ���
        rcSour.top = (y + offsetY) % m_imageInfo->height;
        rcSour.bottom = m_imageInfo->height;
        sourHeight = rcSour.bottom - rcSour.top;

        //�ҽ��� ������ �׸��� ȭ���� �Ѿ�ٸ�(ȭ�� ������ �̹����� ������ ��)
        if (y + sourHeight > drawAreaH)
        {
            //�Ѿ �׸��� ����ŭ ���Ұ��� �÷�����
            rcSour.bottom -= (y + sourHeight) - drawAreaH;
            sourHeight = rcSour.bottom - rcSour.top;
        }

        //�׷����� ����
        rcDest.top = y + drawAreaY;
        rcDest.bottom = rcDest.top + sourHeight;

        //���� ���� ����
        for (int x = 0; x < drawAreaW; x += sourWidth)
        {
            //�ҽ��� ���� ���� ���
            rcSour.left = (x + offsetX % m_imageInfo->width);
            rcSour.right = m_imageInfo->width;
            sourWidth = rcSour.right - rcSour.left;

            if (x + sourWidth > drawAreaW)
            {
                //�Ѿ ��ŭ right���� ��������
                rcSour.right -= (x + sourWidth) - drawAreaW;
                sourWidth = rcSour.right - rcSour.left;
            }

            //�׷����� ����
            rcDest.left = x + drawAreaX;
            rcDest.right = rcDest.left + sourWidth;

            //�׷�����
            render(hdc, rcDest.left, rcDest.top, rcSour.left, rcSour.top, rcSour.right - rcSour.left, rcSour.bottom - rcSour.top);
        }

    }
}