#pragma once
class DesignElement : public GameObject
{
private:
    Image * _image;
    bool _isAlpha;
    BYTE _alpha;
public:
    DesignElement(Image* image);
    DesignElement(Image* image, bool isAlpha);
    ~DesignElement();

    virtual void init() override;
    virtual void update() override;
    virtual void render(HDC hdc) override;
    virtual void release() override;

    void setAlpha(const BYTE alpha)
    {
        _alpha = alpha;
    }
    BYTE getAlpha() const
    {
        return _alpha;
    }
    Image* getImage() const
    {
        return _image;
    }
};