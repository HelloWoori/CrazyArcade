#pragma once
class PlayScene : public Scene
{
private:
    MapTypeTag _mapType;
    ModeTypeTag _mode;
    vector<vector<Tile>> _vvTile;
    int _startTime;
	bool _setTime;
	bool _setGameStateInit;
	bool _check; //한번만 실행하는 거 체크하기 위함

    DesignElement* _gameWords;
    DesignElement* _startWords;
    DesignElement* _blackBg;
	DesignElement* _gameGrayWords;
	DesignElement* _overWords;

	RECT _stopGameRect; //나가기 버튼

public:
    PlayScene();
    ~PlayScene();

    virtual void init() override;
    virtual void release() override;
    virtual void update() override;
    virtual void render(HDC hdc) override;
    virtual void handleArgs(vector<int> args) override;

    void loadTile();
	void debug(HDC hdc);

    //물풍선 관련 맵정보
    static CanIGo mapArr[BOARD_ROW][BOARD_COL];
    static void changeMapArr(int row, int col, bool isBomb, bool rightAfter = false);

	//아이템 관련 맵정보
	static bool isItemArr[BOARD_ROW][BOARD_COL];
	static void setIsItem(int row, int col, bool isItem);
	static bool getIsItem(int row, int col);
};