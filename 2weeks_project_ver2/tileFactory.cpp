#include "stdafx.h"
#include "tileFactory.h"

vector<vector<Tile>> TileFactory::makeMapTile(MapTypeTag mapType)
{
    vector<vector<Tile>> _vvTile(BOARD_ROW, vector<Tile>(BOARD_COL, Tile()));
    switch (mapType)
    {
    case MapTypeTag::Forest:
    {
        Tile tile1("forestTile1");
        Tile tile2("forestTile2");
        Tile tile3("forestTile3");
        Tile tile4("forestTile4");
        Tile tile5("forestTile5");
        Tile tile6("forestTile6");
        Tile tile7("forestTile7");
        Tile tile8("forestTile8");
        Tile tile9("forestTile9");
        Tile tile10("forestTile10");

        //0행
        _vvTile[0][0] = tile4;
        for (int i = 1; i <= 13; ++i)
        {
            _vvTile[0][i] = tile1;
        }
        _vvTile[0][14] = tile3;

        //2, 6, 10행
        for (int i = 2; i <= 10; i += 4)
        {
            for (int j = 0; j < 15; j++)
            {
                _vvTile[i][j] = tile2;
            }
        }

        //4, 8, 12행
        for (int i = 4; i <= 12; i += 4)
        {
            for (int j = 0; j < 15; j++)
            {
                _vvTile[i][j] = tile1;
            }
        }

        //1, 5, 9행
        for (int i = 1; i <= 9; i += 4)
        {
            _vvTile[i][0] = tile5;
            _vvTile[i][1] = tile6;
            _vvTile[i][2] = tile7;
            _vvTile[i][3] = tile8;
            _vvTile[i][4] = tile9;
            _vvTile[i][5] = tile10;
            _vvTile[i][6] = tile7;
            _vvTile[i][7] = tile8;
            _vvTile[i][8] = tile9;
            _vvTile[i][9] = tile10;
            _vvTile[i][10] = tile5;
            _vvTile[i][11] = tile6;
            _vvTile[i][12] = tile7;
            _vvTile[i][13] = tile8;
            _vvTile[i][14] = tile9;
        }

        //3, 7, 11행
        for (int i = 3; i <= 11; i += 4)
        {
            _vvTile[i][0] = tile9;
            _vvTile[i][1] = tile10;
            _vvTile[i][2] = tile7;
            _vvTile[i][3] = tile8;
            _vvTile[i][4] = tile5;
            _vvTile[i][5] = tile6;
            _vvTile[i][6] = tile8;
            _vvTile[i][7] = tile9;
            _vvTile[i][8] = tile8;
            _vvTile[i][9] = tile9;
            _vvTile[i][10] = tile10;
            _vvTile[i][11] = tile5;
            _vvTile[i][12] = tile8;
            _vvTile[i][13] = tile9;
            _vvTile[i][14] = tile7;
        }
    }
    break;
    //다른 맵 추가할 시 요기에 작성하면 됨


    }
    return _vvTile;
}