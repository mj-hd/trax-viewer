#include <iostream>
#include <SDL2/SDL.h>
#include <boost/thread.hpp>

#include "IParser.h"
#include "MjhdParser.h"
#include "TileRenderer.h"

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

SDL_Window* window;
SDL_Renderer* renderer;
TileRenderer* tileRenderer;
IParser* parser;

boost::atomic<bool> isParsing(false);

void thStdInput() {

    parser = new MjhdParser();

    while (true){
        try {
            isParsing = false;
            *parser << std::cin;
            isParsing = true;
        }
        catch (const char* message) {
            std::cerr << message << std::endl;
            return;
        }
    }
}

int main(int argc, char* argv[]) {
    boost::thread threadStdInput(thStdInput);

    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Trax", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    tileRenderer = new TileRenderer(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    SDL_Event ev;
    while(true) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        while(SDL_PollEvent(&ev) || isParsing) {
            if (ev.type == SDL_QUIT)
                goto end;

            switch(ev.type) {
                case SDL_KEYDOWN:
                    switch (ev.key.keysym.sym) {
                        case SDLK_LEFT:

                            break;
                        case SDLK_RIGHT:

                            break;
                    }
                    break;
                case SDL_KEYUP:
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (ev.button.button == SDL_BUTTON_LEFT) {

                    }
                    break;
                case SDL_MOUSEMOTION:
                    // ev.motion.x, y
                    break;
            }
        }

        tileRenderer->Render(parser);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

end:

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    delete parser, tileRenderer;

    SDL_Quit();

    return 0;
}
