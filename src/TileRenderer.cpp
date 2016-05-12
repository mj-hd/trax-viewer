#include "TileRenderer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <sstream>
#include <unistd.h>

#include <algorithm>

#include "IParser.h"

const char* TileRenderer::CrossTilePath = "./resources/cross.png";
const char* TileRenderer::SlashTilePath = "./resources/slash.png";
const char* TileRenderer::FontMapPath   = "./resources/font.png";

const int TileRenderer::FontMapSize  = 16;
const int TileRenderer::FontMapWidth = 32;

TileRenderer::TileRenderer(SDL_Renderer* renderer, int width, int height) {
    this->_renderer = renderer;
    this->_width    = width;
    this->_height   = height;

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
        dstRect.w = _getScaledTileSize();
        dstRect.h = _getScaledTileSize();
        dstRect.x = _getScaledOrtX() + (x + 1) * dstRect.w;
        dstRect.y = _getScaledOrtY();

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
    dstRect.w = _getScaledTileSize();
    dstRect.h = _getScaledTileSize();
    dstRect.x = _getScaledOrtX();
    dstRect.y = _getScaledOrtY() + dstRect.h * row;

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
                _getScaledOrtX() + (x + 1) * _getScaledTileSize(),
                _getScaledOrtY(),
                _getScaledOrtX() + (x + 1) * _getScaledTileSize(),
                _getScaledOrtY() + height  * _getScaledTileSize()
            );
    }

    for (auto y = 0; y < height; y++) {
         SDL_RenderDrawLine(this->_renderer,
                 _getScaledOrtX(),
                 _getScaledOrtY() + (y + 1) * _getScaledTileSize(),
                 _getScaledOrtX() + width   * _getScaledTileSize(),
                 _getScaledOrtY() + (y + 1) * _getScaledTileSize()
            );
    }
}

inline void TileRenderer::_updateScaling(int width, int height) {
    if ((double)width  / (double)this->_width > (double)height / (double)this->_height)
        this->_scaledTileSize = this->_width / width;
    else
        this->_scaledTileSize = this->_height / height;

    this->_scaledOrtX = (this->_width  - this->_scaledTileSize * width)  / 2;
    this->_scaledOrtY = (this->_height - this->_scaledTileSize * height) / 2;
}

void TileRenderer::Render(IParser* parser) {

    auto width = parser->GetWidth();
    auto height = parser->GetHeight();

    if (width == 0 || height == 0) return;

    _updateScaling(width + 1, height + 1);

    _renderColumns(width);

    auto x = 0;
    auto y = 0;
    for (auto row : *parser) {

        _renderRow(y);

        x = 0;
        for (auto tile : row) {
            SDL_Texture* texture;

            SDL_Rect rect;

            rect.w = _getScaledTileSize();
            rect.h = _getScaledTileSize();
            rect.x = _getScaledOrtX() + x * rect.w;
            rect.y = _getScaledOrtY() + y * rect.h;

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
