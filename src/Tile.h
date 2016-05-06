#ifndef _TILE_H_
#define _TILE_H_

enum class TileType {
    None,
    Cross,
    Slash
};

enum class TileDirection : unsigned int {
    N = 0,
    E,
    S,
    W
};

struct Tile {
    TileType      Type;
    TileDirection Direction;
};

#endif
