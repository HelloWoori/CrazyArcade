#pragma once
class ModeSelectScene : public Scene
{
private:
    RECT _rc1;
    //RECT _rc2; //���͸���
	bool _check;
public:
    ModeSelectScene();
    ~ModeSelectScene();

    virtual void init() override;
    virtual void release() override;
    virtual void update() override;
    virtual void render(HDC hdc) override;
};