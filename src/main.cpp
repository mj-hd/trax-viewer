#include <iostream>
#include <SDL2/SDL.h>
#include <boost/thread.hpp>

#include "IParser.h"
#include "MjhdParser.h"
#include "TileRenderer.h"

SDL_Window* window;
SDL_Renderer* renderer;
TileRenderer* tileRenderer;
IParser* parser;

boost::atomic<bool> isParsing(false);

void thEventLoop() {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Trax", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);

    renderer = SDL_CreateRenderer(window, -1, 0);

    tileRenderer = new TileRenderer(renderer);

    SDL_Event ev;
    while(true) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        while(SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT)
                return;

            switch(ev.type) {
                case SDL_KEYDOWN:
                    // TODO: implement here
                    break;
                case SDL_KEYUP:
                    // TODO: implement here
                    break;
            }
        }

        while (isParsing) {}

        tileRenderer->Render(parser);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }
}

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
    boost::thread threadEventLoop(thEventLoop);
    boost::thread threadStdInput(thStdInput);

    threadEventLoop.join();
    threadStdInput.join();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    delete parser, tileRenderer;

    SDL_Quit();

    return 0;
}
