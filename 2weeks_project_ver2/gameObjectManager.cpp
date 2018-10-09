#include "stdafx.h"
#include "playScene.h"

GameObjectManager::GameObjectManager()
{
    _instance_id = 0;

    //���߿� Ǫ���� �� ���� ���߿� �׷���
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

//��ü����
void GameObjectManager::registerObj(GameObject* obj)
{
    //�ν��Ͻ����� ���� ID�� �������ص�, vector�� �������
    obj->setId(_instance_id++);
    _gameObj.push_back(obj);
	obj->init(); //�ʱ�ȭ�� ���⼭ ����
}
//��ü����
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
	_gameObj.resize(0); //��� ������Ʈ�� ����
}

void GameObjectManager::updateObj()
{
    for (auto g : _gameObj)
    {
        g->update();
    }

    if (GAMESTATEMANAGER->getGameStart())
    {
        //������ �����ϸ�, 
        //���ӿ�����Ʈ�� �浹���θ� ��� üũ
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
    �±׺��� ������ �׸��� ���� �������� �׷���
    ���߿� �׸����� ���� �׷���
    */
    map<GameObjectTag, vector<GameObject*>> m;
    for (auto go : _gameObj)
        m[go->getTag()].push_back(go);

    for (auto tag : _layerOrders)
    {
        for (auto go : m[tag])
            go->render(hdc);
    }

	//����׿�
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
        ����
        �浹�� ���� ���� for��
        // collision->onCollisionEnter(actor, area); �̷������� �ѱ�
        ex) bomb->onCollisionEnter(player, rcTemp);
    */
    map<GameObjectTag, vector<GameObject*>> m;
    RECT rcTemp;

    for (auto go : _gameObj)
        m[go->getTag()].push_back(go);

    /* �÷��̾�� ��ǳ������ �浹*/
    for (auto player : m[GameObjectTag::Player])
    {
        for (auto bomb : m[GameObjectTag::Bomb])
        {
            Player* p = dynamic_cast<Player*>(player);
            Bomb* b = dynamic_cast<Bomb*>(bomb);
            RECT playerBodyRect = playerBodyRect = makeRect(p->getBodyStartX(), p->getBodyStartY(), p->getBodyWidth(), p->getBodyHeight());
            RECT bombRect = bombRect = makeRect(b->getStartX(), b->getStartY(), b->getSize(), b->getSize());
            MapSpace mapSpace = mapSpace = leftTopToMapSpace(b->getStartX(), b->getStartY());
            if (PlayScene::mapArr[mapSpace.row][mapSpace.col].rightAfter) //��ź�� ���� ����
            {
                if (!IntersectRect(&rcTemp, &playerBodyRect, &bombRect))
                {
                    //��ź�� ���� ���Ŀ�, �浹�� �ƴѻ��°� �Ǹ� '��ź�� ���� ���İ� �ƴѻ���'�� ����
					if (b->getWhoUser() == p) //��, ��ź�� ���� �� �ڽŸ� �ش��ϰԲ� �ϱ� ����
						PlayScene::mapArr[mapSpace.row][mapSpace.col].rightAfter = false;
                }
            }
            else //��ź�� ���� ���İ� �ƴѵ� �浹�� �Ͼ ��쿡�� ĳ���͸� �浹�� ��ŭ �о������
            {
                if (IntersectRect(&rcTemp, &playerBodyRect, &bombRect))
                {
                    //�浹�� �Ͼ���� �׶� �ش��Լ� ȣ��
                    bomb->onCollisionEnter(player, rcTemp);
                }
            }
        }
    }

    /* �÷��̾�� ���ٱⰣ�� �浹 */
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
    /* �÷��̾�� waveStartPoint���� �浹*/
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

	/*�÷��̾�� ������ ���� �浹*/
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
				//�浹�� �������� �����Ѵ�
				GAMEOBJMANGER->removeObj(i->getId());
				player->onCollisionEnter(item, rcTemp);
			}
		}
	}

	/* �����۰� ���ٱ� or wave Start Point ���� �浹*/
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
				//�浹�� �������� �����Ѵ�
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
				//�浹�� �������� �����Ѵ�
				GAMEOBJMANGER->removeObj(i->getId());
			}
		}
	}

	/*ǳ���� ���ٱ⿡ ������ �ٷ� �����Բ�!!!*/
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
	/* �÷��̾�� �÷��̾��� �浹 */
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
			// �Ѵ� ��ǳ���� �ƴҶ�, �ǵ�� �װԲ�
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

	/* �浹�� �ƴ����� �׳� ���� �־��¡ */
	// ���� �÷��̾ ������ ���� �÷��̾�� �츰��
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
			//��������� ���� ������ �ٸ� ������ �츰��
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
	//������    
	for (size_t i = 0; i < _gameObj.size(); ++i)
	{
		Text(15, 5, 100 + (15 * i), TEXT("ID: ") + to_string(_gameObj[i]->getId()) + TEXT(", TAG: ") + showTagForDebug(_gameObj[i]->getTag()), WHITE)(hdc);
	}
}