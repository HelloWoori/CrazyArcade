#pragma once
class Tile
{
private:
    string _strKey;
public:
    Tile();
    Tile(string strKey);

    string getStrKey()
    {
        return _strKey;
    }
};