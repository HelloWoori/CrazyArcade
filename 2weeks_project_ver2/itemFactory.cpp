#include "stdafx.h"
#include "itemFactory.h"
#include "playScene.h"
void ItemFactory::createItemAtRandomPosition()
{
	int theNumOfItem = RANDOM->getIntFromTo(35, 40); //35~40 ������ �������� �����Ѵ�
	int count = 0; // ������ ���� Ƚ���� ���� ����
	while (count < theNumOfItem) //theNumOfItem�� ������ŭ ����� ���� while���� ����
	{
		int randomRow = RANDOM->getIntFromTo(0, 12); //13��
		int randomCol = RANDOM->getIntFromTo(0, 14); //15��
		ItemTypeTag randomItemType = static_cast<ItemTypeTag>(RANDOM->getIntFromTo(1, 4)); //0�� NOT�̹Ƿ� ����

		//TODO: ������Ʈ�� �� ����ִ� �濡�� �������� �ʵ��� ���ǹ� �߰��������!!!
		if (PlayScene::getIsItem(randomRow, randomCol) == false) //�װ��� �������� ���� ��쿡��
		{
			MapSpace mapSpace;
			mapSpace.row = randomRow;
			mapSpace.col = randomCol;

			Item* item = new Item(mapSpace, randomItemType);
			GAMEOBJMANGER->registerObj(item);
			PlayScene::setIsItem(randomRow, randomCol, true); //���� �ű� �������� ������ ǥ��
			count++;
		}

	}

}
