#include "TileRenderer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <sstream>
#include <unistd.h>

#include "IParser.h"

const char* TileRenderer::CrossTilePath = "./resources/cross.png";
const char* TileRenderer::SlashTilePath = "./resources/slash.png";
const char* TileRenderer::FontMapPath   = "./resources/font.png";

const int TileRenderer::TileBaseSize = 50;
const int TileRenderer::FontMapSize  = 16;
const int TileRenderer::FontMapWidth = 32;

TileRenderer::TileRenderer(SDL_Renderer* renderer) {
    this->_renderer = renderer;

    auto crossTileSurface = IMG_Load(TileRenderer::CrossTilePath);
    auto slashTileSurface = IMG_Load(TileRenderer::SlashTilePath);
    auto fontMapSurface   = IMG_Load(TileRenderer::FontMapPath);
    this->_crossTile = SDL_CreateTextureFromSurface(renderer, crossTileSurface);
    this->_slashTile = SDL_CreateTextureFromSurface(renderer, slashTileSurface);
    this->_fontMap   = SDL_CreateTextureFromSurface(renderer, fontMapSurface);

    if (this->_crossTile == nullptr) throw "Could not create texture of CrossTile";
    if (this->_slashTile == nullptr) throw "Could not create texture of SlashTile";
    if (this->_fontMap   == nullptr) throw "Could not create texture of FontMap";

    SDL_FreeSurface(crossTileSurface);
    SDL_FreeSurface(slashTileSurface);
    SDL_FreeSurface(fontMapSurface);
    crossTileSurface = NULL;
    slashTileSurface = NULL;
    fontMapSurface   = NULL;
}

TileRenderer::~TileRenderer() {
    SDL_DestroyTexture(this->_crossTile);
    SDL_DestroyTexture(this->_slashTile);
    SDL_DestroyTexture(this->_fontMap);
}

inline void TileRenderer::_renderColumns(int width) {
    for (auto x = 0; x < width; x++) {
        SDL_Rect dstRect;
        dstRect.w = TileRenderer::TileBaseSize;
        dstRect.h = TileRenderer::TileBaseSize;
        dstRect.x = (x + 1) * dstRect.w;
        dstRect.y = 0;

        SDL_Rect srcRect;
        srcRect.w = TileRenderer::FontMapSize;
        srcRect.h = TileRenderer::FontMapSize;
        srcRect.x = (('@' + x) % TileRenderer::FontMapWidth)     * TileRenderer::FontMapSize;
        srcRect.y = (('@' + x) / TileRenderer::FontMapWidth - 1) * TileRenderer::FontMapSize;

        SDL_RenderCopy(this->_renderer, this->_fontMap, &srcRect, &dstRect);
    }
}

inline void TileRenderer::_renderRow(int row) {
    SDL_Rect dstRect;
    dstRect.w = TileRenderer::TileBaseSize;
    dstRect.h = TileRenderer::TileBaseSize;
    dstRect.x = 0;
    dstRect.y = dstRect.h * row;

    SDL_Rect srcRect;
    srcRect.w = TileRenderer::FontMapSize;
    srcRect.h = TileRenderer::FontMapSize;
    srcRect.x = (('0' + row - 1) % TileRenderer::FontMapWidth)     * TileRenderer::FontMapSize;
    srcRect.y = (('0' + row - 1) / TileRenderer::FontMapWidth - 1) * TileRenderer::FontMapSize;

    SDL_RenderCopy(this->_renderer, this->_fontMap, &srcRect, &dstRect);
}

inline void TileRenderer::_renderGrid(int width, int height) {

    SDL_SetRenderDrawColor(this->_renderer, 0x00, 0x00, 0x00, 0xaa);

    for (auto x = 0; x < width; x++) {
        SDL_RenderDrawLine(this->_renderer,
                (x + 1) * TileRenderer::TileBaseSize,
                0,
                (x + 1) * TileRenderer::TileBaseSize,
                height  * TileRenderer::TileBaseSize
            );
    }

    for (auto y = 0; y < height; y++) {
         SDL_RenderDrawLine(this->_renderer,
                 0,
                 (y + 1) * TileRenderer::TileBaseSize,
                 width   * TileRenderer::TileBaseSize,
                 (y + 1) * TileRenderer::TileBaseSize
            );
    }
}

void TileRenderer::Render(IParser* parser) {

    _renderColumns(parser->GetWidth());

    auto x = 0;
    auto y = 0;
    for (auto row : *parser) {

        _renderRow(y);

        x = 0;
        for (auto tile : row) {
            SDL_Texture* texture;

            SDL_Rect rect;

            rect.w = TileRenderer::TileBaseSize;
            rect.h = TileRenderer::TileBaseSize;
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
                default:
                    continue;
            }

            SDL_RenderCopyEx(this->_renderer, texture, nullptr, &rect, (int)tile.Direction * 90.0, nullptr, SDL_FLIP_NONE);
        }

        y++;
    }

    _renderGrid(x, y);
}
