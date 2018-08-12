#pragma once
#include "singletonBase.h"
#include <bitset>

#define KEYMAX 256
class KeyManager : public SingletonBase<KeyManager>
{
private:
    bitset<KEYMAX> m_keyUp;
    bitset<KEYMAX> m_keyDown;
public:
    KeyManager();
    ~KeyManager();

    HRESULT init();
    void release();

    //키가 한번만 눌렀냐
    bool isOnceKeyDown(int key);
    //한번 눌렀다가 뗐는지
    bool isOnceKeyUp(int key);
    //키가 계속 눌려져 있는지
    bool isStayKeyDown(int key);
    //토글키냐 캡스락 넘버락
    bool isToggleKey(int key);
};