#pragma once
#include "singletonBase.h"
#include "time.h"

class RandomFunction : public SingletonBase<RandomFunction>
{
public:
    RandomFunction()
    {
        srand(GetTickCount());
    }
    ~RandomFunction()
    {
    }
    inline int getInt(int num)
    {
        return rand() % num;
    }
    inline int getIntFromTo(int fromNum, int toNum)
    {
        return rand() % (toNum - fromNum + 1) + fromNum;
    }
};