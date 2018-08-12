#pragma once
class Image
{
    enum IMAGE_LOAD_KIND
    {
        LOAD_RESOURCE, // ���ҽ��κ��� ����
        LOAD_FILE,     // ���Ϸκ��� ����
        LOAD_EMPTY,    // �� ����
        LOAD_END
    };

    typedef struct tagImage
    {
        tagImage()
        {
            resID = 0;
            hMemDC = nullptr;
            hBit = nullptr;
            hOBit = nullptr;
            x = 0;
            y = 0;
            width = 0;
            height = 0;
            currentFrameX = 0;
            currentFrameY = 0;
            maxFrameX = 0;
            maxFrameY = 0;
            frameWidth = 0;
            frameHeight = 0;
            loadType = LOAD_RESOURCE;
        }

        DWORD resID;        // ���ҽ� ���̵�
        HDC hMemDC;         // �޸� DC
        HBITMAP hBit;       // ��Ʈ��(���ο� ��Ʈ�� �ڵ�)
        HBITMAP hOBit;      // �õ� ��Ʈ��(���� ��Ʈ�� �ڵ�)
        float x;            // x��ǥ
        float y;            // y��ǥ
        int width;          // �̹��� ����ũ��
        int height;         // �̹��� ���� ũ��
        int currentFrameX;  // ���� ������ x
        int currentFrameY;  // ���� ������ y
        int maxFrameX;      // �ִ� ������ x
        int maxFrameY;      // �ִ� ������ y 
        int frameWidth;
        int frameHeight;

        BYTE loadType;      // Ÿ��
    } IMAGE_INFO, *LPIMAGE_INFO;

public:
    Image();
    ~Image();

    /* init() ==================================*/
    // �� ��Ʈ�� �ʱ�ȭ
    HRESULT init(int width, int height);
    // �̹��� ���ҽ� �ʱ�ȭ
    HRESULT init(const DWORD resID, int width, int height, bool isTrans = false, COLORREF transColor = MAGENTA);

    //const char* fileName
    // �̹��� ���� �ʱ�ȭ
    HRESULT init(const char* fileName, int width, int height, bool isTrans = false, COLORREF transColor = MAGENTA);
    // ������ �̹��� ���Ϸ� �ʱ�ȭ
    HRESULT init(const char* fileName, int width, int height, int frameX, int frameY, bool isTrans = false, COLORREF transColor = MAGENTA);
    HRESULT init(const char* fileName, int x, int y, int width, int height, int frameX, int frameY, bool isTrans = false, COLORREF transColor = MAGENTA);


    // ����Ű ���� == Ư�� ������ �׸��� �ʴ´�
    void setTransColor(bool isTrans, COLORREF transColor);
    // ������
    void release();

    /* render =======================================================================================================*/
    //�׸��� �༮(�Ϲ� ����)
    void render(HDC hdc);
    void render(HDC hdc, int destX, int destY);
    void render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);
    void renderStretch(HDC hdc, int destX, int destY, int destWidth, int destHeight, int sourX, int sourY, int sourWidth, int sourHeight);

    // �츮 �߰�
    // ���� ����
    void alphaRender(HDC hdc, BYTE alpha);
    void alphaRender(HDC hdc, int destX, int destY, BYTE alpha);
    void alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);

    //�׸��� �༮(������ ����)
    void frameRender(HDC hdc, int destX, int destY);
    void frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);

    //�׸��� �༮(���� ����)
    void loopRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY);

    /* DC�� ���´� ============================================================================================= */
    inline HDC getMemDC() { return m_imageInfo->hMemDC; }

    /* �̹����� ��ǥ �ټ�, �̹����� ���� ��ǥ ����, �̹����� ���μ��� ũ�� ��� ================================== */
    // �̹��� ��ǥ x ���
    inline float getX() { return m_imageInfo->x; }
    inline void setX(float x) { m_imageInfo->x = x; }

    // �̹��� ��ǥ y ���
    inline float getY() { return m_imageInfo->y; }
    inline void setY(float y) { m_imageInfo->y = y; }

    // �̹��� ���� ��ǥ ����
    inline void setCenter(float x, float y)
    {
        m_imageInfo->x = x - (m_imageInfo->width / 2);
        m_imageInfo->y = y - (m_imageInfo->height / 2);
    }
    // ���� X
    inline float getCenterX()
    {
        return m_imageInfo->maxFrameX <= 0 ? m_imageInfo->x + (m_imageInfo->width / 2)
            : m_imageInfo->x + (m_imageInfo->frameWidth / 2);
    }
    // ���� Y
    inline float getCenterY()
    {
        return m_imageInfo->maxFrameY <= 0 ? m_imageInfo->y + (m_imageInfo->height / 2)
            : m_imageInfo->y + (m_imageInfo->frameHeight / 2);
    }

    // ���� ���� ũ�� ���
    inline int getWidth() { return m_imageInfo->width; }
    inline int getHeight() { return m_imageInfo->height; }

    /* �ٿ�� �ڽ� (�浹��) ======================================================================================== */
    inline RECT getBoundingBox()
    {
        RECT rc;
        if (m_imageInfo->frameWidth != 0 && m_imageInfo->frameHeight != 0)
        {
            rc = makeRect(m_imageInfo->x, m_imageInfo->y, m_imageInfo->frameWidth, m_imageInfo->frameHeight);
        }
        else
        {
            rc = makeRect(m_imageInfo->x, m_imageInfo->y, m_imageInfo->width, m_imageInfo->height);
        }
        return rc;
    }

    /* ������ ��ǥ �ټ� ==================================================================================== */
    // ������ x
    inline int getFrameX() { return m_imageInfo->currentFrameX; }
    inline void setFrameX(int frameX)
    {
        m_imageInfo->currentFrameX = frameX;
        if (frameX > m_imageInfo->maxFrameX)
        {
            m_imageInfo->currentFrameX = m_imageInfo->maxFrameX;
        }
    }

    //������ y
    inline int getFrameY() { return m_imageInfo->currentFrameY; }
    inline void setFrameY(int frameY)
    {
        m_imageInfo->currentFrameY = frameY;
        if (frameY > m_imageInfo->maxFrameY)
        {
            m_imageInfo->currentFrameY = m_imageInfo->maxFrameY;
        }
    }

    /* ������ ���μ��� ũ�� ======================================================================================= */
    inline int getFrameWidth() { return m_imageInfo->frameWidth; }
    inline int getFrameHeight() { return m_imageInfo->frameHeight; }
    /* �ƽ� ������ ================================================================================================ */
    inline int getMaxFrameX() { return m_imageInfo->maxFrameX; }
    inline int getMaxFrameY() { return m_imageInfo->maxFrameY; }


private:
    LPIMAGE_INFO m_imageInfo; //�̹��� ����
    char* m_fileName; //�̹��� ��� �̸�
    bool m_isTrans;  //������ ���� ���� ���� (���� ����̳�?)
    COLORREF m_transColor; //������ �ʰ��� Ư�� ����

                           //�츮�߰�
    BLENDFUNCTION m_blendFunc; //���� ���带 ���� ����
    LPIMAGE_INFO m_blendImage; //���� ���带 ����ϱ� ���� �̹��� ����
};