#include "stdafx.h"
#include "playScene.h"

GameObjectManager::GameObjectManager()
{
    _instance_id = 0;

    //나중에 푸쉬백 할 수록 나중에 그려짐
    _layerOrders.push_back(GameObjectTag::WaveStartingPoint);
    _layerOrders.push_back(GameObjectTag::Wave);
	_layerOrders.push_back(GameObjectTag::Item);
    _layerOrders.push_back(GameObjectTag::Bomb);
    _layerOrders.push_back(GameObjectTag::Player);
    _layerOrders.push_back(GameObjectTag::DesignElement);
}

GameObjectManager::~GameObjectManager()
{
}

//객체생성
void GameObjectManager::registerObj(GameObject* obj)
{
    //인스턴스별로 개별 ID를 지정해준뒤, vector에 집어넣음
    obj->setId(_instance_id++);
    _gameObj.push_back(obj);
	obj->init(); //초기화를 여기서 해줌
}
//객체삭제
void GameObjectManager::removeObj(int id)
{
    for (size_t i = 0; i < _gameObj.size(); ++i)
    {
        if (_gameObj[i]->getId() == id)
            _gameObj.erase(_gameObj.begin() + i);
    }
}

void GameObjectManager::removeObjAll()
{
	_gameObj.resize(0); //모든 오브젝트들 삭제
}

void GameObjectManager::updateObj()
{
    for (auto g : _gameObj)
    {
        g->update();
    }

    if (GAMESTATEMANAGER->getGameStart())
    {
        //게임을 시작하면, 
        //게임오브젝트간 충돌여부를 계속 체크
        notifyCollisions();
    }//gamestart
}

void GameObjectManager::renderObj(HDC hdc)
{
    //for (size_t i = 0; i < _gameObj.size(); ++i)
    //{
    //    _gameObj[i]->render(hdc);
    //}

    /*
    태그별로 나누고 그리고 싶은 순서부터 그려라
    나중에 그릴수록 위에 그려짐
    */
    map<GameObjectTag, vector<GameObject*>> m;
    for (auto go : _gameObj)
        m[go->getTag()].push_back(go);

    for (auto tag : _layerOrders)
    {
        for (auto go : m[tag])
            go->render(hdc);
    }

	//디버그용
	//debug(hdc);
}

void GameObjectManager::releaseObj()
{
    for (size_t i = 0; i < _gameObj.size(); ++i)
    {
        _gameObj[i]->release();
    }
}

void GameObjectManager::notifyCollisions()
{
    /*
        사용법
        충돌을 위한 이중 for문
        // collision->onCollisionEnter(actor, area); 이런식으로 넘김
        ex) bomb->onCollisionEnter(player, rcTemp);
    */
    map<GameObjectTag, vector<GameObject*>> m;
    RECT rcTemp;

    for (auto go : _gameObj)
        m[go->getTag()].push_back(go);

    /* 플레이어와 물풍선간의 충돌*/
    for (auto player : m[GameObjectTag::Player])
    {
        for (auto bomb : m[GameObjectTag::Bomb])
        {
            Player* p = dynamic_cast<Player*>(player);
            Bomb* b = dynamic_cast<Bomb*>(bomb);
            RECT playerBodyRect = playerBodyRect = makeRect(p->getBodyStartX(), p->getBodyStartY(), p->getBodyWidth(), p->getBodyHeight());
            RECT bombRect = bombRect = makeRect(b->getStartX(), b->getStartY(), b->getSize(), b->getSize());
            MapSpace mapSpace = mapSpace = leftTopToMapSpace(b->getStartX(), b->getStartY());
            if (PlayScene::mapArr[mapSpace.row][mapSpace.col].rightAfter) //폭탄을 놓고난 직후
            {
                if (!IntersectRect(&rcTemp, &playerBodyRect, &bombRect))
                {
                    //폭탄을 놓고난 직후에, 충돌이 아닌상태가 되면 '폭탄을 놓고난 직후가 아닌상태'로 변경
					if (b->getWhoUser() == p) //단, 폭탄을 놓은 그 자신만 해당하게끔 하기 위함
						PlayScene::mapArr[mapSpace.row][mapSpace.col].rightAfter = false;
                }
            }
            else //폭탄을 놓고난 직후가 아닌데 충돌이 일어난 경우에만 캐릭터를 충돌한 만큼 밀어버린다
            {
                if (IntersectRect(&rcTemp, &playerBodyRect, &bombRect))
                {
                    //충돌이 일어났으면 그때 해당함수 호출
                    bomb->onCollisionEnter(player, rcTemp);
                }
            }
        }
    }

    /* 플레이어와 물줄기간의 충돌 */
    for (auto player : m[GameObjectTag::Player])
    {
        for (auto wave : m[GameObjectTag::Wave])
        {
            Player* p = dynamic_cast<Player*>(player);
            Wave* w = dynamic_cast<Wave*>(wave);
            RECT playerCollisionRect = playerCollisionRect = makeRect(p->getCollisionStartX(), p->getCollisionStartY(), p->getCollisionWidth(), p->getCollisionHeight());
            RECT waveRect = makeRect(w->getStartX(), w->getStartY(), w->getSize(), w->getSize());
            if (IntersectRect(&rcTemp, &playerCollisionRect, &waveRect))
            {
                player->onCollisionEnter(wave, rcTemp);
            }

        }
    }
    /* 플레이어와 waveStartPoint간의 충돌*/
    for (auto player : m[GameObjectTag::Player])
    {
        for (auto waveStartingPoint : m[GameObjectTag::WaveStartingPoint])
        {
            Player* p = dynamic_cast<Player*>(player);
            WaveStartingPoint* wsp = dynamic_cast<WaveStartingPoint*>(waveStartingPoint);
            RECT playerCollisionRect = makeRect(p->getCollisionStartX(), p->getCollisionStartY(), p->getCollisionWidth(), p->getCollisionHeight());
            RECT waveStartingPointRect= makeRect(wsp->getStartX(), wsp->getStartY(), wsp->getSize(), wsp->getSize());
            if (IntersectRect(&rcTemp, &playerCollisionRect, &waveStartingPointRect))
            {
                player->onCollisionEnter(wsp, rcTemp);
            }

        }
    }

	/*플레이어와 아이템 간의 충돌*/
	for (auto player : m[GameObjectTag::Player])
	{
		for (auto item : m[GameObjectTag::Item])
		{
			Player* p = dynamic_cast<Player*>(player);
			Item * i = dynamic_cast<Item*>(item);
			RECT playerCollisionRect = makeRect(p->getCollisionStartX(), p->getCollisionStartY(), p->getCollisionWidth(), p->getCollisionHeight());
			RECT itemRect = makeRect(i->getStartX(), i->getStartY(), i->getSize(), i->getSize());
			if (IntersectRect(&rcTemp, &playerCollisionRect, &itemRect))
			{
				//충돌한 아이템을 삭제한다
				GAMEOBJMANGER->removeObj(i->getId());
				player->onCollisionEnter(item, rcTemp);
			}
		}
	}

	/* 아이템과 물줄기 or wave Start Point 간의 충돌*/
	for (auto item : m[GameObjectTag::Item])
	{
		for (auto wave : m[GameObjectTag::Wave])
		{
			Wave* w = dynamic_cast<Wave*>(wave);
			Item * i = dynamic_cast<Item*>(item);
			RECT waveRect = makeRect(w->getStartX(), w->getStartY(), w->getSize(), w->getSize());
			RECT itemRect = makeRect(i->getStartX(), i->getStartY(), i->getSize(), i->getSize());
			if (IntersectRect(&rcTemp, &waveRect, &itemRect))
			{
				//충돌한 아이템을 삭제한다
				GAMEOBJMANGER->removeObj(i->getId());
			}

		}
	}
	for (auto item : m[GameObjectTag::Item])
	{
		for (auto waveStartingPoint : m[GameObjectTag::WaveStartingPoint])
		{
			Item * i = dynamic_cast<Item*>(item);
			WaveStartingPoint* wsp = dynamic_cast<WaveStartingPoint*>(waveStartingPoint);
			RECT itemRect = makeRect(i->getStartX(), i->getStartY(), i->getSize(), i->getSize());
			RECT waveStartingPointRect = makeRect(wsp->getStartX(), wsp->getStartY(), wsp->getSize(), wsp->getSize());
			if (IntersectRect(&rcTemp, &itemRect, &waveStartingPointRect))
			{
				//충돌한 아이템을 삭제한다
				GAMEOBJMANGER->removeObj(i->getId());
			}
		}
	}

	/*풍선이 물줄기에 닿으면 바로 터지게끔!!!*/
	for (auto bomb : m[GameObjectTag::Bomb])
	{
        Bomb* b = dynamic_cast<Bomb*>(bomb);

		for (auto wave : m[GameObjectTag::Wave])
		{
			Wave* w = dynamic_cast<Wave*>(wave);
			RECT bombRect = makeRect(b->getStartX(), b->getStartY(), b->getSize(), b->getSize());
			RECT waveRect = makeRect(w->getStartX(), w->getStartY(), w->getSize(), w->getSize());
			if (IntersectRect(&rcTemp, &bombRect, &waveRect))
			{
				if (!b->getChainExplosion())
				{
					b->setChainExplosion(true);
					b->changeBombState(BombStateTag::MakeWaveStartingPoint);
				}
			}
		}
	}

	
	Player* p1 = nullptr;
	Player* p2 = nullptr;
	RECT player1Rect;
	RECT player2Rect;
	/* 플레이어와 플레이어의 충돌 */
	for (auto player : m[GameObjectTag::Player])
	{
		Player* p = dynamic_cast<Player*>(player);
		if (p->getPlayerType() == PlayerTypeTag::Player1)
		{
			p1 = p;
			player1Rect = makeRect(p->getStartX(), p->getStartY(), p->getWidth(), p->getHeight());
		}
		if (p->getPlayerType() == PlayerTypeTag::Player2)
		{
			p2 = p;
			player2Rect = makeRect(p->getStartX(), p->getStartY(), p->getWidth(), p->getHeight());
		}
	
		if (p1 && p2)
		{
			// 둘다 물풍선이 아닐때, 건들면 죽게끔
			if (IntersectRect(&rcTemp, &player1Rect, &player2Rect))
			{
				if (!(p1->getPlayerState() == PlayerStateTag::Trap && p2->getPlayerState() == PlayerStateTag::Trap))
				{
					if (p1->getPlayerState() == PlayerStateTag::Trap)
					{
						p1->setPlayerState(PlayerStateTag::Die);
					}
					else if (p2->getPlayerState() == PlayerStateTag::Trap)
					{
						p2->setPlayerState(PlayerStateTag::Die);
					}
				}
 			}

		}

	}

	/* 충돌은 아니지만 그냥 여기 넣어야징 */
	// 한쪽 플레이어가 죽으면 한쪽 플레이어는 살린다
	for (auto player : m[GameObjectTag::Player])
	{
		Player* p = dynamic_cast<Player*>(player);
		if (p->getPlayerType() == PlayerTypeTag::Player1)
		{
			p1 = p;
		}
		if (p->getPlayerType() == PlayerTypeTag::Player2)
		{
			p2 = p;
		}

		if (p1 && p2)
		{
			//어느한쪽이 먼저 죽으면 다른 한쪽을 살린다
			if (p1->getPlayerState() == PlayerStateTag::Die)
			{
				if (!p2->getLive())
				{
					if (p2->getPlayerState() == PlayerStateTag::Trap)
						p2->setPlayerState(PlayerStateTag::Live);
					else
						p2->setPlayerState(PlayerStateTag::Jump);
					p2->setLive(true);
				}
			}
			else if (p2->getPlayerState() == PlayerStateTag::Die)
			{
				if (!p1->getLive())
				{
					if (p1->getPlayerState() == PlayerStateTag::Trap)
						p1->setPlayerState(PlayerStateTag::Live);
					else
						p1->setPlayerState(PlayerStateTag::Jump);
					p1->setLive(true);
				}
			}
		}
	}
}

string GameObjectManager::showTagForDebug(GameObjectTag tag)
{
    switch (tag)
    {
    case GameObjectTag::Not:
        return "Not";
    case GameObjectTag::Player:
        return "Player";
    case GameObjectTag::Bomb:
        return "Bomb";
    case GameObjectTag::DesignElement:
        return "Design Element";
    case GameObjectTag::Wave:
        return "Wave";
    case GameObjectTag::WaveStartingPoint:
        return "WaveStartingPoint";
	case GameObjectTag::Item:
		return "Item";
    default:
        return "?";
    }
}

void GameObjectManager::debug(HDC hdc)
{
	Text(15, 5, 80, TEXT("GameObj size: ") + to_string(getGameObjSize()))(hdc);
	//디버깅용    
	for (size_t i = 0; i < _gameObj.size(); ++i)
	{
		Text(15, 5, 100 + (15 * i), TEXT("ID: ") + to_string(_gameObj[i]->getId()) + TEXT(", TAG: ") + showTagForDebug(_gameObj[i]->getTag()), WHITE)(hdc);
	}
}