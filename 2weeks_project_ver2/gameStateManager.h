#pragma once
#include "singletonBase.h"
class GameStateManager : public SingletonBase<GameStateManager>
{
private:
    bool _isGameStart;
	bool _isGameOver;
public:
    GameStateManager()
        : _isGameStart(false)
		, _isGameOver(false)
    {
    }
    ~GameStateManager()
    {
    }


	/*
		게터세터
	*/
    bool getGameStart() const
    {
        return _isGameStart;
    }
    void setGameStart(bool isStart)
    {
        _isGameStart = isStart;
    }

	bool getGameOver() const
	{
		return _isGameOver;
	}
	void setGameOver(bool isGameOver)
	{
		_isGameOver = isGameOver;
	}
};