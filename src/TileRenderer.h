#ifndef _TILERENDERER_H_
#define _TILERENDERER_H_

#include <SDL2/SDL.h>

#include "IParser.h"

class TileRenderer {
public:
    TileRenderer(SDL_Renderer* renderer, int width, int height);
    ~TileRenderer();

    void Render(IParser* parser);

protected:
    SDL_Renderer* _renderer;
    SDL_Texture*  _crossTile;
    SDL_Texture*  _slashTile;
    SDL_Texture*  _fontMap;

private:
    static const char* CrossTilePath;
    static const char* SlashTilePath;
    static const char* FontMapPath;
    static const int   FontMapSize;
    static const int   FontMapWidth;

    int _scaledTileSize = 0;
    int _scaledOrtX = 0;
    int _scaledOrtY = 0;
    int _width;
    int _height;

    inline void _renderColumns(int width);
    inline void _renderRow(int row);
    inline void _renderGrid(int width, int height);
    inline int _getScaledTileSize() const { return this->_scaledTileSize; };
    inline int _getScaledOrtX() const { return this->_scaledOrtX; };
    inline int _getScaledOrtY() const { return this->_scaledOrtY; };
    inline void _updateScaling(int width, int height);
};

#endif
