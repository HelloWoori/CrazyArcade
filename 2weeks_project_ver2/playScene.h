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
	bool _check; //�ѹ��� �����ϴ� �� üũ�ϱ� ����

    DesignElement* _gameWords;
    DesignElement* _startWords;
    DesignElement* _blackBg;
	DesignElement* _gameGrayWords;
	DesignElement* _overWords;

	RECT _stopGameRect; //������ ��ư

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

    //��ǳ�� ���� ������
    static CanIGo mapArr[BOARD_ROW][BOARD_COL];
    static void changeMapArr(int row, int col, bool isBomb, bool rightAfter = false);

	//������ ���� ������
	static bool isItemArr[BOARD_ROW][BOARD_COL];
	static void setIsItem(int row, int col, bool isItem);
	static bool getIsItem(int row, int col);
};