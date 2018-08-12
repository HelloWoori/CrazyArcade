#pragma once
class Scene
{
private:

public:
    Scene() {}
    virtual ~Scene() {}

    virtual void init() {}
    virtual void release() {}
    virtual void update() {}
    virtual void render(HDC hdc) {}
    /*
    args[0] = mode
    args[1] = mapType
    */
    virtual void handleArgs(vector<int> args) {}
};