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

    //Ű�� �ѹ��� ������
    bool isOnceKeyDown(int key);
    //�ѹ� �����ٰ� �ô���
    bool isOnceKeyUp(int key);
    //Ű�� ��� ������ �ִ���
    bool isStayKeyDown(int key);
    //���Ű�� ĸ���� �ѹ���
    bool isToggleKey(int key);
};