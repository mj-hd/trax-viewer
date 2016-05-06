#include "TileRenderer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "IParser.h"

const char* TileRenderer::CrossTilePath = "./resources/cross.png";
const char* TileRenderer::SlashTilePath = "./resources/slash.png";

TileRenderer::TileRenderer(SDL_Renderer* renderer) {
    this->_renderer = renderer;

    auto crossTileSurface = IMG_Load(TileRenderer::CrossTilePath);
    auto slashTileSurface = IMG_Load(TileRenderer::SlashTilePath);
    this->_crossTile = SDL_CreateTextureFromSurface(renderer, crossTileSurface);
    this->_slashTile = SDL_CreateTextureFromSurface(renderer, slashTileSurface);

    if (this->_crossTile == nullptr) throw "Could not create texture of CrossTile";
    if (this->_slashTile == nullptr) throw "Could not create texture of SlashTile";

    SDL_FreeSurface(crossTileSurface);
    SDL_FreeSurface(slashTileSurface);
    crossTileSurface = NULL;
    slashTileSurface = NULL;
}

TileRenderer::~TileRenderer() {
    SDL_DestroyTexture(this->_crossTile);
    SDL_DestroyTexture(this->_slashTile);
}

void TileRenderer::Render(IParser* parser) {
    auto y = 0;
    for (auto row : *parser) {
        auto x = 0;
        for (auto tile : row) {
            SDL_Texture* texture;

            SDL_Rect rect;

            rect.w = 50;
            rect.h = 50;
            rect.x = x * rect.w;
            rect.y = y * rect.h;

            x++;

            switch (tile.Type) {
                case TileType::None:
                    continue;
                case TileType::Cross:
                    texture = this->_crossTile;
                    break;
                case TileType::Slash:
                    texture = this->_slashTile;
                    break;
            }

            SDL_RenderCopyEx(this->_renderer, texture, nullptr, &rect, (int)tile.Direction * 90.0, nullptr, SDL_FLIP_NONE);

        }

        y++;
    }
}
