#pragma once
#pragma comment(lib, "winmm.lib")
#include "gameNode.h"
#include <mmsystem.h>
class MainGame : public GameNode
{
private:

public:
    MainGame();
    virtual ~MainGame();

    virtual HRESULT init() override;
    virtual void release() override;
    virtual void update() override;
    virtual void render() override;
};