#pragma once
class Player : public GameObject
{
private:
	PlayerTypeTag _playerType;
    POINT_FLOAT _start;
    POINT_FLOAT _center;
    //충돌용 사각형
    POINT_FLOAT _collisionStart;
    //물풍선용 몸통 사각형
    POINT_FLOAT _bodyStart;

    float _speed;     //속도
    int _usableBombs; //사용할 수 있는 물풍선의 개수
    int _usedBombs; //사용한 물풍선의 개수
    int _power; //물줄기 세기

    PlayerStateTag _previousState;
    PlayerStateTag _currentState;

    int _startTime;
    float _trapMoveCoolDown;
    int _count;
	//bool _flag; //그림자 출력 오류 잡을라고
	bool _setLive; //살았다 셋팅은 한번만
	bool _check;//물풍선 두는 소리

    /* 상수 */
    const int _WIDTH;
    const int _HEIGHT;
    //충돌용 사각형
    const int _COLLISION_WIDTH;
    const int _COLLISION_HEIGHT;
    //물풍선용 몸통 사각형
    const int _BODY_WIDTH;
    const int _BODY_HEIGHT;

    const float _TRAP_SPEED;
    const float _MOVE_COOLTIME;
    const float _WAIT_COOLTIME; 
    const float _READY_COOLTIME;
    const float _TRAP_COOLTIME;
    const float _TRAP_MOVE_COOLTIME;
    const float _DIE_COOLTIME;

public:
    Player(PlayerTypeTag playerType, float startX, float startY);
    ~Player();

    virtual void init() override;
    virtual void update() override;
    virtual void render(HDC hdc) override;
    virtual void release() override;
    virtual void onCollisionEnter(GameObject* other, RECT area) override;

    void setCenter();
    void setCollisionStart();
    void setBodyStart();

	//사용한 풍선이 터지면 _usedBombs의 개수를 조정해주기 위함
	void changeUsedBombs();

	//디버그용
	string showPlayerStateForDebug(PlayerStateTag state);
    void debug(HDC hdc);

	/*
	게터세터
	*/
	int getWidth() const
	{
		return _WIDTH;
	}
	int getHeight() const
	{
		return _HEIGHT;
	}

	float getStartX() const
	{
		return _start.x;
	}

	void setStartX(const float startX)
	{
		_start.x = startX;
	}

	float getStartY() const
	{
		return _start.y;
	}
	void setStartY(const float startY)
	{
		_start.y = startY;
	}

	//충돌용 사각형
	int getCollisionWidth() const
	{
		return _COLLISION_WIDTH;
	}
	int getCollisionHeight() const
	{
		return _COLLISION_HEIGHT;
	}

	float getCollisionStartX()
	{
		return _collisionStart.x;
	}
	void setCollisionStartX(float collisionStartX)
	{
		_collisionStart.x = collisionStartX;
	}
	float getCollisionStartY()
	{
		return _collisionStart.y;
	}
	void setCollisionStartY(float collisionStartY)
	{
		_collisionStart.y = collisionStartY;
	}


	//몸통용 사각형
	int getBodyWidth() const
	{
		return _BODY_WIDTH;
	}
	int getBodyHeight() const
	{
		return _BODY_HEIGHT;
	}

	float getBodyStartX()
	{
		return _bodyStart.x;
	}
	void setBodyStartX(float bodyStartX)
	{
		_bodyStart.x = bodyStartX;
	}
	float getBodyStartY()
	{
		return _bodyStart.y;
	}
	void setBodyStartY(float bodyStartY)
	{
		_bodyStart.y = bodyStartY;
	}

	PlayerTypeTag getPlayerType() const
	{
		return _playerType;
	}

	PlayerStateTag getPlayerState()
	{
		return _currentState;
	}
	void setPlayerState(PlayerStateTag state)
	{
		_previousState = _currentState;
		_currentState = state;
	}
	bool getLive()
	{
		return _setLive;
	}
	void setLive(bool live)
	{
		_setLive = live;
	}
};