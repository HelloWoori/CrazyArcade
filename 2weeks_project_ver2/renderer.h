#pragma once
/*
���� ����
Pen(�������, ���)(hdc, [hdc]()
{
drawLine(hdc, .....);
});
*/
struct Pen
{
    Pen(HDC hdc, int size, COLORREF penColor)
        : _size(size)
        , _penColor(penColor)
    {}
    void operator()(HDC hdc, function<void()> functor)
    {
        HPEN myPen, oldPen;
        myPen = CreatePen(PS_SOLID, _size, _penColor);
        oldPen = (HPEN)SelectObject(hdc, myPen);

        functor();

        SelectObject(hdc, oldPen);
        DeleteObject(myPen);
    }

    int _size;
    COLORREF _penColor;
};


/*
���� ����
PenWithBrush(�������, ���, �귯����)(hdc, [hdc, this]() // this�� ����Լ��� ��������� �Բ� ĸ��
// hdc�� ���̿� ������ �Լ��� �ʿ��ϱ� ����
{
drawRect(hdc, ....);
});
*/
struct PenWithBrush
{
    PenWithBrush(int size, COLORREF penColor, COLORREF brushColor)
        : _size(size)
        , _penColor(penColor)
        , _brushColor(brushColor)
    {}
    void operator()(HDC hdc, function<void()> functor)
    {
        HPEN myPen, oldPen;
        HBRUSH myBrush, oldBrush;
        myPen = CreatePen(PS_SOLID, _size, _penColor);
        oldPen = (HPEN)SelectObject(hdc, myPen);
        myBrush = CreateSolidBrush(_brushColor);
        oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

        functor();

        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(myPen);
        DeleteObject(myBrush);

    }

    int _size;
    COLORREF _penColor;
    COLORREF _brushColor;
};

/*
#define HS_HORIZONTAL       0       -----
#define HS_VERTICAL         1       |||||
#define HS_FDIAGONAL        2       \\\\\
#define HS_BDIAGONAL        3       /////
#define HS_CROSS            4       +++++
#define HS_DIAGCROSS        5       xxxxx

���� ����
PenWithHatchBrush(�������, ���, �귯����, HS_CROSS)(hdc, [hdc, this]()
{

drawRect(hdc,.....);

});
*/
struct PenWithHatchBrush
{
    PenWithHatchBrush(int size, COLORREF penColor, COLORREF brushColor, int hatchStyle)
        : _size(size)
        , _penColor(penColor)
        , _brushColor(brushColor)
        , _hatchStyle(hatchStyle)
    {}
    void operator()(HDC hdc, function<void()> functor)
    {
        HPEN myPen, oldPen;
        HBRUSH myBrush, oldBrush;
        myPen = CreatePen(PS_SOLID, _size, _penColor);
        oldPen = (HPEN)SelectObject(hdc, myPen);
        myBrush = CreateHatchBrush(_hatchStyle, _brushColor);
        oldBrush = (HBRUSH)SelectObject(hdc, myBrush);

        functor();

        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(myPen);
        DeleteObject(myBrush);

    }

    int _size;
    COLORREF _penColor;
    COLORREF _brushColor;
    int _hatchStyle;
};

/*
���� ����1 (�����ڵ�)
Text(hdc, 80, 100, 100, to_wstring(_fallYaxis), L"����ǹ��� �־�", �ؽ�Ʈ��)(hdc);

���� ����2 (�����ڵ�)
Text(hdc, 40, 90, 50, L"����: ", L"����� ���� �־�", �ؽ�Ʈ��)(hdc);

��Ƽ����Ʈ�� ��� "���ڿ�" Ȥ�� to_string(����) �� ���
*/
//����_Regular
struct Text
{
    Text(int size, int x, int y, string str, COLORREF textColor = RGB(0, 0, 0), string font = "����_Regular")
        : _size(size)
        , _x(x)
        , _y(y)
        , _str(str)
        , _font(font)
        , _textColor(textColor)
    {}

    void operator()(HDC hdc)
    {
        HFONT font, oldFont;
        font = CreateFont(_size, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _font.c_str());
        oldFont = (HFONT)SelectObject(hdc, font);
        SetBkMode(hdc, TRANSPARENT); //���� ����� (TRANSPARENT : ���� , QPAQUEL : ������)
        SetTextColor(hdc, _textColor);
        TextOut(hdc, _x, _y, _str.c_str(), static_cast<int>(_str.size()));
        SelectObject(hdc, oldFont);
        DeleteObject(font);
    }

    int _size;
    int _x;
    int _y;
    string _str;
    string _font;
    COLORREF _textColor;
};