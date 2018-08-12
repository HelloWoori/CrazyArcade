#pragma once
class Player : public GameObject
{
private:
	PlayerTypeTag _playerType;
    POINT_FLOAT _start;
    POINT_FLOAT _center;
    //�浹�� �簢��
    POINT_FLOAT _collisionStart;
    //��ǳ���� ���� �簢��
    POINT_FLOAT _bodyStart;

    float _speed;     //�ӵ�
    int _usableBombs; //����� �� �ִ� ��ǳ���� ����
    int _usedBombs; //����� ��ǳ���� ����
    int _power; //���ٱ� ����

    PlayerStateTag _previousState;
    PlayerStateTag _currentState;

    int _startTime;
    float _trapMoveCoolDown;
    int _count;
	//bool _flag; //�׸��� ��� ���� �������
	bool _setLive; //��Ҵ� ������ �ѹ���
	bool _check;//��ǳ�� �δ� �Ҹ�

    /* ��� */
    const int _WIDTH;
    const int _HEIGHT;
    //�浹�� �簢��
    const int _COLLISION_WIDTH;
    const int _COLLISION_HEIGHT;
    //��ǳ���� ���� �簢��
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

	//����� ǳ���� ������ _usedBombs�� ������ �������ֱ� ����
	void changeUsedBombs();

	//����׿�
	string showPlayerStateForDebug(PlayerStateTag state);
    void debug(HDC hdc);

	/*
	���ͼ���
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

	//�浹�� �簢��
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


	//����� �簢��
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