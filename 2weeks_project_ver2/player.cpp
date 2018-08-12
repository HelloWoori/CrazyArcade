#include "stdafx.h"
#include "player.h"
#include "playScene.h"

Player::Player(PlayerTypeTag playerType, float startX, float startY)
    : GameObject(GameObjectTag::Player)
	, _playerType(playerType)
    , _WIDTH(BOARD_RECTSIZE)
    , _HEIGHT(BOARD_RECTSIZE)
    , _speed(2.f)
    , _usedBombs(0)
    , _usableBombs(1)        // TODO: 지금은 100개 사용가능한데, 최대 6개로 제한할 것
    , _power(1)                 //power의 최대는 7
    , _previousState(PlayerStateTag::Not)
    , _currentState(PlayerStateTag::Ready)  
    , _startTime(static_cast<int>(TIMEMANAGER->getWorldTime()))
    , _COLLISION_WIDTH(20)
    , _COLLISION_HEIGHT(26)
    , _BODY_WIDTH(42)
    , _BODY_HEIGHT(24)
    , _TRAP_SPEED(0.2f)
	, _setLive(false)
    //쿨타임 ==================
    , _MOVE_COOLTIME(0.1f)
    , _WAIT_COOLTIME(4.0f)
    , _READY_COOLTIME(0.1f)
    , _TRAP_COOLTIME(0.2f) 
    , _TRAP_MOVE_COOLTIME(0.3f) //물풍선 갇힌 후 자동으로 아래로 내려오는 용
    , _DIE_COOLTIME(0.2f)
    , _count(0)                 //프레임 반복횟수 제한 두기위함
	, _check(false)
{
    _start.x = startX;
    _start.y = startY;

    setCenter();                //중심좌표 업데이트
    setCollisionStart();        //충돌사각형 좌상단 업데이트
    setBodyStart();             //몸통 사각형 좌상단 업데이트

    setAnimationInfo("playerBazziReady", _READY_COOLTIME);
}

Player::~Player()
{

}

void Player::init()
{
}

void Player::update()
{
    //캐릭터 상태 확인용
    //cout << "_previousState: " << showPlayerStateForDebug(_previousState) << endl;
    //cout << "_currentState: " << showPlayerStateForDebug(_currentState) << endl;
    if (!GAMESTATEMANAGER->getGameStart())                             //게임이 시작하기 전에는 레디 애니메이션을 보여준다
    {
        int maxFrame = IMAGEMANAGER->findImage(getStrKey())->getMaxFrameX();
        animation(maxFrame);
        if (getFrame() >= maxFrame)                                    //번쩍번쩍 프레임 위함
            setFrame(15);
    }
    else
    {
        if (_currentState == PlayerStateTag::Ready)                    //여전히 ready상태라면 강제로 wait상태로 변경한다
        {
            setAnimationInfo("playerBazziWait", _WAIT_COOLTIME);
            _previousState = _currentState;
            _currentState = PlayerStateTag::Wait;
        }
        else if (_currentState == PlayerStateTag::Trap)
        {
			if (_currentState != _previousState)
			{
				_startTime = static_cast<int>(TIMEMANAGER->getWorldTime());
				setAnimationInfo("playerBazziTrap", _TRAP_COOLTIME);
				_speed = _TRAP_SPEED; //스피드 느려짐
				_previousState = _currentState;
			}
            int maxFrame = IMAGEMANAGER->findImage(getStrKey())->getMaxFrameX();
            animation(maxFrame);
            if (getFrame() >= maxFrame)
                setFrame(8);

            //조금씩 아래로 내려온다
            _trapMoveCoolDown -= TIMEMANAGER->getElapsedTime();
            if (_trapMoveCoolDown <= 0.f)
            {
                _trapMoveCoolDown = _TRAP_MOVE_COOLTIME;
				if (_start.y + 2.f <= BOARD_STARTY + BOARD_HEIGHT - _HEIGHT) //빠져나가지 않게 처리
					_start.y += 2.f;
            }

			int currentTime = static_cast<int>(TIMEMANAGER->getWorldTime());
			//cout << currentTime - _startTime << endl;
            if (currentTime - _startTime > 7) //7초가 지나면 죽은 상태로!!
            {
				_startTime = static_cast<int>(TIMEMANAGER->getWorldTime()); //죽고나서 몇초 시간 재기위함
				_previousState = _currentState;
                _currentState = PlayerStateTag::Die;
            }

            //갇혔을 때의 키보드 처리
            if ( (_playerType == PlayerTypeTag::SoloPlayer && KEYMANAGER->isStayKeyDown(VK_UP)) ||
				 (_playerType == PlayerTypeTag::Player2 && KEYMANAGER->isStayKeyDown(VK_UP)) ||
			     (_playerType == PlayerTypeTag::Player1 && KEYMANAGER->isStayKeyDown('R')))
            {
                if (_start.y - _speed >= BOARD_STARTY)
                    _start.y -= _speed;
            }
            else if ((_playerType == PlayerTypeTag::SoloPlayer && KEYMANAGER->isStayKeyDown(VK_DOWN)) ||
				     (_playerType == PlayerTypeTag::Player2 && KEYMANAGER->isStayKeyDown(VK_DOWN)) ||
				     (_playerType == PlayerTypeTag::Player1 && KEYMANAGER->isStayKeyDown('F')))
            {
                if (_start.y + _speed <= BOARD_STARTY + BOARD_HEIGHT - _HEIGHT)
                    _start.y += _speed;
            }
            else if ((_playerType == PlayerTypeTag::SoloPlayer && KEYMANAGER->isStayKeyDown(VK_LEFT)) ||
				     (_playerType == PlayerTypeTag::Player2 && KEYMANAGER->isStayKeyDown(VK_LEFT)) ||
				     (_playerType == PlayerTypeTag::Player1 && KEYMANAGER->isStayKeyDown('D')))
            {
                if (_start.x - _speed >= BOARD_STARTX)
                    _start.x -= _speed;
            }
            else if ((_playerType == PlayerTypeTag::SoloPlayer && KEYMANAGER->isStayKeyDown(VK_RIGHT)) ||
				     (_playerType == PlayerTypeTag::Player2 && KEYMANAGER->isStayKeyDown(VK_RIGHT)) ||
				     (_playerType == PlayerTypeTag::Player1 && KEYMANAGER->isStayKeyDown('G')))
            {
                if (_start.x + _speed <= BOARD_STARTX + BOARD_WIDTH - _WIDTH)
                    _start.x += _speed;
            }    
        }
		else if (_currentState == PlayerStateTag::Die)
		{
			GAMESTATEMANAGER->setGameOver(true);//게임오버 상태로 만든다
			if (_currentState != _previousState)
			{
				SOUNDMANAGER->play(static_cast<int>(SoundTypeTag::PlayerDie), SoundTypeTag::PlayerDie);
				SOUNDMANAGER->play(static_cast<int>(SoundTypeTag::GameOver), SoundTypeTag::GameOver);
				setAnimationInfo("playerBazziDie", _DIE_COOLTIME);
				_previousState = _currentState;
				//_flag = true;
			}
			int maxFrame = IMAGEMANAGER->findImage(getStrKey())->getMaxFrameX();
			animation(maxFrame);
			if (_count <= 3)
			{
				if (getFrame() >= maxFrame)
				{
					++_count;
					setFrame(7);
				}
			}
			else
				setFrame(maxFrame);

			//죽은지 5초 뒤에 게임오브젝트 매니저에서 삭제
			int currentTime = static_cast<int>(TIMEMANAGER->getWorldTime());
			if (currentTime - _startTime > 5)
				GAMEOBJMANGER->removeObj(getId());

		} //죽은 경우 끝
		else if (_currentState == PlayerStateTag::Live)
		{
			if (_currentState != _previousState)
			{
				SOUNDMANAGER->play(static_cast<int>(SoundTypeTag::BombPop), SoundTypeTag::BombPop);
				setAnimationInfo("playerBazziLive", 0.3f);
				_previousState = _currentState;
			}
			int maxFrame = IMAGEMANAGER->findImage(getStrKey())->getMaxFrameX();
			animation(maxFrame);
			if (getFrame() >= maxFrame) //프레임 다 돌면 Jump상태로
			{
				_previousState = _currentState;
				_currentState = PlayerStateTag::Jump;
			}
		}
		else if (_currentState == PlayerStateTag::Jump)
		{
			if (_currentState != _previousState)
			{
				setAnimationInfo("playerBazziJump", 0.3f);
				_previousState = _currentState;
			}
			animation(IMAGEMANAGER->findImage(getStrKey())->getMaxFrameX());
		}
		else 
        {	
			if (!GAMESTATEMANAGER->getGameOver())
			{
				if ((_playerType == PlayerTypeTag::SoloPlayer && KEYMANAGER->isStayKeyDown(VK_SPACE)) ||
					(_playerType == PlayerTypeTag::Player2 && KEYMANAGER->isStayKeyDown(VK_RSHIFT)) ||
					(_playerType == PlayerTypeTag::Player1 && KEYMANAGER->isStayKeyDown(VK_LSHIFT)))
				{
					if (!_check)
					{
						_check = true;
						SOUNDMANAGER->play(static_cast<int>(SoundTypeTag::BombSet), SoundTypeTag::BombSet);
					}
					MapSpace m = centerToMapSpace(_center.x, _center.y);
					if (!PlayScene::mapArr[m.row][m.col].isBomb)             //물풍선이 있는 곳엔 다시 물풍선을 두지 못한다
					{
						if (_usedBombs < _usableBombs)                      //사용한 물풍선은 사용할 수 있는 물풍선의 개수를 넘지 않아야한다
						{
							Bomb* bomb = new Bomb(this, _center, _power);          //물풍선생성하기
							GAMEOBJMANGER->registerObj(bomb);
							_usedBombs++;
						}
					}
				}

				if ((_playerType == PlayerTypeTag::SoloPlayer && KEYMANAGER->isStayKeyDown(VK_UP)) ||
					(_playerType == PlayerTypeTag::Player2 && KEYMANAGER->isStayKeyDown(VK_UP)) ||
					(_playerType == PlayerTypeTag::Player1 && KEYMANAGER->isStayKeyDown('R')))
				{
					_startTime = static_cast<int>(TIMEMANAGER->getWorldTime());

					_previousState = _currentState;
					_currentState = PlayerStateTag::Up;
					if (_previousState != _currentState)
					{
						setAnimationInfo("playerBazziUp", _MOVE_COOLTIME);
					}
					animation(IMAGEMANAGER->findImage(getStrKey())->getMaxFrameX());

					if (_start.y - _speed >= BOARD_STARTY)
					{
						_start.y -= _speed;
					}
				}
				else if ((_playerType == PlayerTypeTag::SoloPlayer && KEYMANAGER->isStayKeyDown(VK_DOWN)) ||
					(_playerType == PlayerTypeTag::Player2 && KEYMANAGER->isStayKeyDown(VK_DOWN)) ||
					(_playerType == PlayerTypeTag::Player1 && KEYMANAGER->isStayKeyDown('F')))
				{
					_startTime = static_cast<int>(TIMEMANAGER->getWorldTime());

					_previousState = _currentState;
					_currentState = PlayerStateTag::Down;
					if (_previousState != _currentState)
					{
						setAnimationInfo("playerBazziDown", _MOVE_COOLTIME);
					}
					animation(IMAGEMANAGER->findImage(getStrKey())->getMaxFrameX());

					if (_start.y + _speed <= BOARD_STARTY + BOARD_HEIGHT - _HEIGHT)
					{
						_start.y += _speed;
					}
				}
				else if ((_playerType == PlayerTypeTag::SoloPlayer && KEYMANAGER->isStayKeyDown(VK_LEFT)) ||
					(_playerType == PlayerTypeTag::Player2 && KEYMANAGER->isStayKeyDown(VK_LEFT)) ||
					(_playerType == PlayerTypeTag::Player1 && KEYMANAGER->isStayKeyDown('D')))
				{
					_startTime = static_cast<int>(TIMEMANAGER->getWorldTime());

					_previousState = _currentState;
					_currentState = PlayerStateTag::Left;
					if (_previousState != _currentState)
					{
						setAnimationInfo("playerBazziLeft", _MOVE_COOLTIME);
					}
					animation(IMAGEMANAGER->findImage(getStrKey())->getMaxFrameX());

					if (_start.x - _speed >= BOARD_STARTX)
					{
						_start.x -= _speed;
					}
				}
				else if ((_playerType == PlayerTypeTag::SoloPlayer && KEYMANAGER->isStayKeyDown(VK_RIGHT)) ||
					(_playerType == PlayerTypeTag::Player2 && KEYMANAGER->isStayKeyDown(VK_RIGHT)) ||
					(_playerType == PlayerTypeTag::Player1 && KEYMANAGER->isStayKeyDown('G')))
				{
					_startTime = static_cast<int>(TIMEMANAGER->getWorldTime());

					_previousState = _currentState;
					_currentState = PlayerStateTag::Right;
					if (_previousState != _currentState)
					{
						setAnimationInfo("playerBazziRight", _MOVE_COOLTIME);
					}
					animation(IMAGEMANAGER->findImage(getStrKey())->getMaxFrameX());

					if (_start.x + _speed <= BOARD_STARTX + BOARD_WIDTH - _WIDTH)
					{
						_start.x += _speed;
					}
				}
				else // 키 입력이 없는 경우
				{
					if (_currentState == PlayerStateTag::Left || _currentState == PlayerStateTag::Right ||
						_currentState == PlayerStateTag::Up || _currentState == PlayerStateTag::Down)
					{
						//캐릭터는 항상 0이나 4프레임으로 끝나야함
						int frameX = IMAGEMANAGER->findImage(getStrKey())->getFrameX();
						if (frameX != 0 || frameX != 4)
							setFrame(0);

						//키입력이 없고, 일정시간이 지나면 정면을 바라본다
						int currentTime = static_cast<int>(TIMEMANAGER->getWorldTime());
						if (currentTime - _startTime > 3)
						{
							setAnimationInfo("playerBazziWait", _WAIT_COOLTIME);
							_previousState = _currentState;
							_currentState = PlayerStateTag::Wait;
						}

					}
				}//키 입력이 없는 경우 
			}//게임오버상태가 아닌경우 끝
        } //레디도 트랩도 죽은상태도 아닌경우

        setCenter();         //중심좌표 업데이트
        setCollisionStart(); //충돌사각형 좌상단 업데이트
        setBodyStart();      //몸통 좌상단 업데이트
    }//gameStart 끝
}

void Player::render(HDC hdc)
{
    //TODO: 그림자 - 이거 물줄기보다 아래 그려져야해서 나중에 게임오브젝트로 빼야할듯
    //if (_currentState == PlayerStateTag::Trap)
    //    IMAGEMANAGER->findImage("playerShadow")->alphaRender(hdc, static_cast<int>(_center.x - 21.f), static_cast<int>(_center.y + 16.f), 200);
    //else if (_currentState == PlayerStateTag::Die && _flag)
    //{
    //    if(getFrame() > 3)
    //        IMAGEMANAGER->findImage("playerShadow")->alphaRender(hdc, static_cast<int>(_center.x - 21.f), static_cast<int>(_center.y + 40.f), 200);
    //}
    //else
    //    IMAGEMANAGER->findImage("playerShadow")->alphaRender(hdc, static_cast<int>(_center.x - 21.f), static_cast<int>(_center.y + 3.f), 200);
    //

    if (_currentState == PlayerStateTag::Ready) //10픽셀 y축을 올려그려주기 위함
        IMAGEMANAGER->findImage(getStrKey())->frameRender(hdc, static_cast<int>(_center.x - 32.f), static_cast<int>(_center.y - 65.f), getFrame(), 0);
    else if (_currentState == PlayerStateTag::Trap || _currentState == PlayerStateTag::Die) //얘도 좌표조정위함
        IMAGEMANAGER->findImage(getStrKey())->frameRender(hdc, static_cast<int>(_center.x - 44.f), static_cast<int>(_center.y - 76.f), getFrame(), 0);
    else //기본
        IMAGEMANAGER->findImage(getStrKey())->frameRender(hdc, static_cast<int>(_center.x - 32.f), static_cast<int>(_center.y - 55.f), getFrame(), 0);


	//위에 1p인지 2p인지 알려줌
	if (_playerType == PlayerTypeTag::Player1)
		IMAGEMANAGER->findImage("player1")->render(hdc, _center.x - 12.f, _center.y - 90.f);
	else if(_playerType == PlayerTypeTag::Player2)
		IMAGEMANAGER->findImage("player2")->render(hdc, _center.x - 12.f, _center.y - 90.f);
	else if (_playerType == PlayerTypeTag::SoloPlayer)
		IMAGEMANAGER->findImage("soloPlayer")->render(hdc, _center.x - 12.f, _center.y - 90.f);

    //디버깅용
    //debug(hdc);
}

void Player::release()
{
}


void Player::onCollisionEnter(GameObject* other, RECT area)
{
    switch (other->getTag())
    {
    case GameObjectTag::Wave: //물줄기에 맞았으면
    case GameObjectTag::WaveStartingPoint:
    {
        _previousState = _currentState;
        _currentState = PlayerStateTag::Trap;
    }
        break;
	case GameObjectTag::Item:
	{
		SOUNDMANAGER->play(static_cast<int>(SoundTypeTag::EatItem), SoundTypeTag::EatItem);
		Item* item = dynamic_cast<Item*>(other);
		//item의 타입에 따라 플레이어의 속성이 달라진다
		//현재 item은 물풍선, 물약, 맥스 물약, 스피드가 있고 제한이 있으므로 조건문 처리하기
		switch (item->getItemType())
		{
		case ItemTypeTag::Ballon:
			if(_usableBombs < 6)
				_usableBombs++;
			break;
		case ItemTypeTag::Potion:
			if (_power < 7)
				_power++;
			break;
		case ItemTypeTag::PotionMakePowerMax:
			if (_power < 7)
				_power = 7;
			break;
		case ItemTypeTag::Skate:
			if (_speed < 5.f)
				_speed += 1.f;
			break;
		}
	
	}
		break;
    }//switch 끝
}

void Player::setCenter()
{
    _center.x = _start.x + (_WIDTH / 2) + 1;
    _center.y = _start.y + (_HEIGHT / 2);
}

void Player::setCollisionStart()
{
    _collisionStart.x = _center.x - 10;
    _collisionStart.y = _center.y - 8;
}

void Player::setBodyStart()
{
    _bodyStart.x = _center.x - 21;
    _bodyStart.y = _center.y - 8;
}

void Player::changeUsedBombs()
{
	_usedBombs--;
}

string Player::showPlayerStateForDebug(PlayerStateTag state)
{
    switch (state)
    {
    case PlayerStateTag::Not:
        return "Not";
    case PlayerStateTag::Ready:
        return "Ready";
    case PlayerStateTag::Wait:
        return "Wait";
    case PlayerStateTag::Up:
        return "Up";
    case PlayerStateTag::Down:
        return "Down";
    case PlayerStateTag::Left:
        return "Left";
    case PlayerStateTag::Right:
        return "Right";
    case PlayerStateTag::Trap:
        return "Trap";
    case PlayerStateTag::Die:
        return "Die";
	case PlayerStateTag::Live:
		return "Live";
    default:
        return "?";
    }
}

void Player::debug(HDC hdc)
{
    //디버깅용 사각형
    if (_currentState == PlayerStateTag::Trap)
    {
        PenWithHatchBrush(1, RGB(244, 66, 66), RGB(244, 66, 66), HS_BDIAGONAL)(hdc, [hdc, this]
        {
            drawRectCenter(hdc, _center.x, _center.y, _WIDTH, _HEIGHT);
        });
    }
    else
    {
        PenWithHatchBrush(1, BLACK, RGB(74, 76, 76), HS_BDIAGONAL)(hdc, [hdc, this]
        {
            drawRectCenter(hdc, _center.x, _center.y, _WIDTH, _HEIGHT);
        });
    }
    ////디버깅용 몸통 사각형
    PenWithBrush(1, RGB(0, 0, 255), RGB(0, 0, 255))(hdc, [hdc, this]
    {
        drawRect(hdc, _bodyStart.x, _bodyStart.y, _BODY_WIDTH, _BODY_HEIGHT);
    });

    ////디버깅용 충돌 사각형
    PenWithBrush(1, RGB(0, 255, 0), RGB(0, 255, 0))(hdc, [hdc, this]
    {
        drawRect(hdc, _collisionStart.x, _collisionStart.y, _COLLISION_WIDTH, _COLLISION_HEIGHT);
    });

    //디버깅용 캐릭터 좌상단 위치
	if (_playerType == PlayerTypeTag::Player1 || _playerType == PlayerTypeTag::SoloPlayer)
	{
		Text(15, 150, 120, TEXT("Player1=========="), WHITE)(hdc);
		Text(15, 150, 140, to_string(_start.x), WHITE)(hdc);
		Text(15, 150, 160, to_string(_start.y), WHITE)(hdc);
		Text(15, 150, 180, TEXT("Player previousState: ") + showPlayerStateForDebug(_previousState), WHITE)(hdc);
		Text(15, 150, 200, TEXT("Player currentState: ") + showPlayerStateForDebug(_currentState), WHITE)(hdc);
		Text(15, 150, 220, TEXT("Player speed: ") + to_string(_speed))(hdc);
		Text(15, 150, 240, TEXT("Player power: ") + to_string(_power))(hdc);
		Text(15, 150, 260, TEXT("Player usable Bombs: ") + to_string(_usableBombs))(hdc);
		Text(15, 150, 280, TEXT("Player used Bombs: ") + to_string(_usedBombs))(hdc);
	}
	else if (_playerType == PlayerTypeTag::Player2)
	{
		Text(15, 150, 300, TEXT("Player2=========="), WHITE)(hdc);
		Text(15, 150, 320, to_string(_start.x), WHITE)(hdc);
		Text(15, 150, 340, to_string(_start.y), WHITE)(hdc);
		Text(15, 150, 360, TEXT("Player previousState: ") + showPlayerStateForDebug(_previousState), WHITE)(hdc);
		Text(15, 150, 380, TEXT("Player currentState: ") + showPlayerStateForDebug(_currentState), WHITE)(hdc);
		Text(15, 150, 400, TEXT("Player speed: ") + to_string(_speed))(hdc);
		Text(15, 150, 420, TEXT("Player power: ") + to_string(_power))(hdc);
		Text(15, 150, 440, TEXT("Player usable Bombs: ") + to_string(_usableBombs))(hdc);
		Text(15, 150, 460, TEXT("Player used Bombs: ") + to_string(_usedBombs))(hdc);
	}
}

