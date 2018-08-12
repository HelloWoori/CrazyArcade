#pragma once
/*
상속을 통해 연관된 일련된 클래스에 대해 공통적인 규약을 정의할 수 있다.
즉, 적용하고픈 공통규약을 묶어서 GameObject클래스를 정의하였고,
이로인해서 GameObject를 상속하는 모든 클래스의 객체를 GameObject의 객체로 바라볼 수 있게 된 것
*/
class GameObject
{
private:
    int _instance_id;
    GameObjectTag _tag;

    //애니메이션용
    string _strKey;
    int _frame;
    float _coolTime;
    float _coolDown;

public:
    GameObject(GameObjectTag tag)
        : _instance_id(-1)
        , _tag(tag)
        , _strKey(string())
        , _frame(0)
        , _coolTime(0.f)
    {
        _coolDown = _coolTime;
    }
    virtual ~GameObject()
    {
    }

    virtual void init() { }
    virtual void update() { }
    virtual void render(HDC hdc) { }
    virtual void release() { }

    virtual void animation(int maxFrame) final;
    //충돌관련 함수
    virtual void onCollisionEnter(GameObject* other, RECT area);

    void setId(const int id)
    {
        _instance_id = id;
    }
    int getId() const
    {
        return _instance_id;
    }

    void setStrKey(const string strKey)
    {
        _strKey = strKey;
    }
    string getStrKey() const
    {
        return _strKey;
    }

    void setFrame(const int frame)
    {
        _frame = frame;
    }
    int getFrame() const
    {
        return _frame;
    }

    void setCoolTime(const float coolTime)
    {
        _coolTime = coolTime;
    }
    float getCoolTime() const
    {
        return _coolTime;
    }

    void setCoolDown(const float coolDown)
    {
        _coolDown = coolDown;
    }
    float getCoolDown() const
    {
        return _coolDown;
    }

    void setAnimationInfo(const string strKey, const float coolTime, const int frame = 0)
    {
        _strKey = strKey;
        _coolTime = coolTime;
        _coolDown = _coolTime;
        _frame = frame;
    }

    GameObjectTag getTag()
    {
        return _tag;
    }
};