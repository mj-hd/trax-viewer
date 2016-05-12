#ifndef _TILERENDERER_H_
#define _TILERENDERER_H_

#include <SDL2/SDL.h>

#include "IParser.h"

class TileRenderer {
public:
    TileRenderer(SDL_Renderer* renderer);
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
    static const int   TileBaseSize;
    static const int   FontMapSize;
    static const int   FontMapWidth;

    inline void _renderColumns(int width);
    inline void _renderRow(int row);
    inline void _renderGrid(int width, int height);
};

#endif
