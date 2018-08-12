#include "stdafx.h"
#include "itemFactory.h"
#include "playScene.h"
void ItemFactory::createItemAtRandomPosition()
{
	int theNumOfItem = RANDOM->getIntFromTo(35, 40); //35~40 사이의 아이템을 생성한다
	int count = 0; // 아이템 생성 횟수를 세기 위함
	while (count < theNumOfItem) //theNumOfItem의 개수만큼 만들기 위해 while문을 돈다
	{
		int randomRow = RANDOM->getIntFromTo(0, 12); //13행
		int randomCol = RANDOM->getIntFromTo(0, 14); //15열
		ItemTypeTag randomItemType = static_cast<ItemTypeTag>(RANDOM->getIntFromTo(1, 4)); //0은 NOT이므로 제외

		//TODO: 오브젝트나 텅 비어있는 길에는 생성되지 않도록 조건문 추가해줘야함!!!
		if (PlayScene::getIsItem(randomRow, randomCol) == false) //그곳에 아이템이 없는 경우에만
		{
			MapSpace mapSpace;
			mapSpace.row = randomRow;
			mapSpace.col = randomCol;

			Item* item = new Item(mapSpace, randomItemType);
			GAMEOBJMANGER->registerObj(item);
			PlayScene::setIsItem(randomRow, randomCol, true); //이제 거기 아이템이 있음을 표시
			count++;
		}

	}

}
