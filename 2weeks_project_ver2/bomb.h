#pragma once
class Bomb : public GameObject
{
private:
    POINT_FLOAT _center;
    POINT_FLOAT _start;

    MapSpace _mapSpace;
    float _startTime;
	bool _chainExplosion; //연쇄폭발관련

    //애니메이션용
    BombStateTag _previousState;
    BombStateTag _currentState;

    //상수    
	Player* _whoUsed; //누가 사용한 물풍선인지
    const int _SIZE;
    const int _POWER;
    const float _BOMB_COOLTIME;
    const float _POP_COOLTIME;
    const float _POP_TIME;
    const float _DISAPPEAR_TIME;

public:
	Bomb(Player* player, POINT_FLOAT center, int power);
    ~Bomb();

    virtual void init() override;
    virtual void update() override;
    virtual void render(HDC hdc) override;
    virtual void release() override;
    virtual void onCollisionEnter(GameObject* other, RECT area);


	//연쇄폭발 로직 해결관련
	void changeBombState(BombStateTag bombStateTag)
	{
		_previousState = _currentState;
		_currentState = bombStateTag;
	}
	void setChainExplosion(bool chainExplosion)
	{
		_chainExplosion = chainExplosion;
	}
	bool getChainExplosion()
	{
		return _chainExplosion;
	}

	//디버깅용
    string showBombStateForDebug(BombStateTag tag);
    void debug(HDC hdc);

	/*
		게터세터
	*/
	int getSize() const
	{
		return _SIZE;
	}
	float getStartX() const
	{
		return _start.x;
	}
	float getStartY() const
	{
		return _start.y;
	}
	Player* getWhoUser() const
	{
		return _whoUsed;
	}
};