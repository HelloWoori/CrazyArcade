#pragma once
class LoginScene : public Scene
{
private:
    RECT _rc;
	bool _check;
public:
    LoginScene();
    ~LoginScene();

    virtual void init() override;
    virtual void release() override;
    virtual void update() override;
    virtual void render(HDC hdc) override;
};