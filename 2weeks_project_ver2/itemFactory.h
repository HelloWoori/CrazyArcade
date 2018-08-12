#pragma once
//playScene에서 접근하면 좋을 것 같음!
/*
랜덤한 개수로
랜덤한 행렬에
랜덤한 아이템을 생성한다
playScene에서는 호출하면 댐
*/
class ItemFactory
{
public:
	static void createItemAtRandomPosition();
};