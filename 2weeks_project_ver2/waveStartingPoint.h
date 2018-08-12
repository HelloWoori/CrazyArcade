#pragma once
class WaveStartingPoint : public GameObject
{
private:
    WaveStartingPointStateTag _state;
    MapSpace _mapSpace;
    POINT_FLOAT _start;
    int _currentWaveIndex;
    float _coolDown;

    //»ó¼ö
    const int _SIZE;
    const int _POWER;
    const float _MAKE_COOLTIME;
public:
    WaveStartingPoint(MapSpace mapSpace, int power);
    ~WaveStartingPoint();

    virtual void init() override;
    virtual void update() override;
    virtual void render(HDC hdc) override;
    virtual void release() override;

    float getStartX() const
    {
        return _start.x;
    }
    float getStartY() const
    {
        return _start.y;
    }
    int getSize() const
    {
        return _SIZE;
    }
};