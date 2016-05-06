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

private:
    static const char* CrossTilePath;
    static const char* SlashTilePath;
};

#endif
