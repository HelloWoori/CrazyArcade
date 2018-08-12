#pragma once
class Wave : public GameObject
{
private:
    POINT_FLOAT _start;
    MapSpace _mapSpace;

    float _startTime;
    WaveStateTag _state;

    //»ó¼ö
    const int _POWER;
    const int _SIZE;
    const float _WAVE_COOLTIME;

public:
    Wave(int row, int col, WaveStateTag state, int power);
    ~Wave();

    virtual void init() override;
    virtual void update() override;
    virtual void render(HDC hdc) override;
    virtual void release() override;
    virtual void onCollisionEnter(GameObject* other, RECT area) override;

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

    void debug(HDC hdc);
};