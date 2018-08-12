#pragma once
/*
사용법 예시
Pen(펜사이즈, 펜색)(hdc, [hdc]()
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
사용법 예시
PenWithBrush(펜사이즈, 펜색, 브러쉬색)(hdc, [hdc, this]() // this는 멤버함수와 멤버변수를 함께 캡쳐
// hdc는 사이에 실행할 함수에 필요하기 때문
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

사용법 예시
PenWithHatchBrush(펜사이즈, 펜색, 브러쉬색, HS_CROSS)(hdc, [hdc, this]()
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
사용법 예시1 (유니코드)
Text(hdc, 80, 100, 100, to_wstring(_fallYaxis), L"배달의민족 주아", 텍스트색)(hdc);

사용법 예시2 (유니코드)
Text(hdc, 40, 90, 50, L"점수: ", L"배달의 민족 주아", 텍스트색)(hdc);

멀티바이트의 경우 "문자열" 혹은 to_string(숫자) 로 사용
*/
//다음_Regular
struct Text
{
    Text(int size, int x, int y, string str, COLORREF textColor = RGB(0, 0, 0), string font = "다음_Regular")
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
        SetBkMode(hdc, TRANSPARENT); //글자 배경모드 (TRANSPARENT : 투명 , QPAQUEL : 불투명)
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