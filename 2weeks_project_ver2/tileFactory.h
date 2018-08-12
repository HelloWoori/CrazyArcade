#pragma once
class TileFactory
{
public:
    static vector<vector<Tile>> makeMapTile(MapTypeTag mapType);
};