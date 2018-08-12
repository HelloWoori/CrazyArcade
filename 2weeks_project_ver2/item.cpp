#include "stdafx.h"
#include "item.h"

Item::Item(MapSpace mapSpace, ItemTypeTag itemType)
	: GameObject(GameObjectTag::Item)
	, _mapSpace(mapSpace)
	, _itemType(itemType)
	, _SIZE(BOARD_RECTSIZE)
	, _COOLTIME(1.f)
{
	POINT_FLOAT pt;
	pt = mapSpaceToLeftTop(_mapSpace.row, _mapSpace.col);
	_start = pt;

	setCenter();
}

Item::~Item()
{
}

void Item::init()
{
	//태그에 따라 이미지 다르게 세팅
	switch (_itemType)
	{
	case ItemTypeTag::Ballon:
		setAnimationInfo("itemBallon", _COOLTIME);
		break;
	case ItemTypeTag::Potion:
		setAnimationInfo("itemPotion", _COOLTIME);
		break;
	case ItemTypeTag::PotionMakePowerMax:
		setAnimationInfo("itemPotionMakePowerMax", _COOLTIME);
		break;
	case ItemTypeTag::Skate:
		setAnimationInfo("itemSkate", _COOLTIME);
		break;
	}
}

void Item::update()
{

	animation(IMAGEMANAGER->findImage(getStrKey())->getMaxFrameX());
}

void Item::render(HDC hdc)
{
	//디버깅용
	//debug(hdc);
	IMAGEMANAGER->findImage(getStrKey())->frameRender(hdc, static_cast<int>(_start.x - 2.f), static_cast<int>(_start.y - 18.f), getFrame(), 0);

}

void Item::release()
{
}

void Item::onCollisionEnter(GameObject * other, RECT area)
{
}

void Item::setCenter()
{
	_center.x = _start.x + _SIZE / 2.f;
	_center.y = _start.y + _SIZE / 2.f;
}

void Item::debug(HDC hdc)
{

	//확인 차 아이템 별로 다른 렉트 그려보기
	switch (_itemType)
	{
	case ItemTypeTag::Ballon: //파랑
		PenWithHatchBrush(1, RGB(66, 134, 244), RGB(66, 134, 244), HS_CROSS)(hdc, [hdc, this]
		{
			drawEllipseCenter(hdc, _center.x, _center.y, _SIZE, _SIZE);
		});
		break;
	case ItemTypeTag::Potion: //하늘
		PenWithHatchBrush(1, RGB(158, 195, 255), RGB(158, 195, 255), HS_CROSS)(hdc, [hdc, this]
		{
			drawEllipseCenter(hdc, _center.x, _center.y, _SIZE, _SIZE);
		});
		break;
	case ItemTypeTag::PotionMakePowerMax: //보라
		PenWithHatchBrush(1, RGB(115, 56, 255), RGB(115, 56, 255), HS_CROSS)(hdc, [hdc, this]
		{
			drawEllipseCenter(hdc, _center.x, _center.y, _SIZE, _SIZE);
		});
		break;
	case ItemTypeTag::Skate: //빨강
		PenWithHatchBrush(1, RGB(255, 56, 82), RGB(255, 56, 82), HS_CROSS)(hdc, [hdc, this]
		{
			drawEllipseCenter(hdc, _center.x, _center.y, _SIZE, _SIZE);
		});
		break;
	}
}