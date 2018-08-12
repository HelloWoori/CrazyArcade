#pragma once
class Item : public GameObject
{
private:
	ItemTypeTag _itemType;
	MapSpace _mapSpace;

	POINT_FLOAT _start;
	POINT_FLOAT _center;

	const int _SIZE;
	const float _COOLTIME;

public:
	Item(MapSpace mapSpace, ItemTypeTag itemType);
	~Item();

	virtual void init() override;
	virtual void update() override;
	virtual void render(HDC hdc) override;
	virtual void release() override;
	virtual void onCollisionEnter(GameObject* other, RECT area) override;


	float getStartX() const
	{
		return _start.x;
	}
	float getStartY() const
	{
		return _start.y;
	}
	int getSize() const
	{
		return _SIZE;
	}

	ItemTypeTag getItemType() const
	{
		return _itemType;
	}


	void setCenter();
	void debug(HDC hdc);
};