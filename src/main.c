#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "SDL.h"
#include "chip8.h"
#include "chip8keyboard.h"

const char keyboardMap[CHIP8_TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4,
    SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9,
    SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f};

int main(int argc, char **argv)
{
    struct Chip8 chip8;
    bool isRunning = true;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow(
        EMULATOR_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        CHIP8_WIDTH * EMULATOR_WINDOW_MULTIPLIER,
        CHIP8_HEIGHT * EMULATOR_WINDOW_MULTIPLIER,
        SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    while (isRunning)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                isRunning = false;
                break;

            case SDL_KEYDOWN:
            {
                char key = event.key.keysym.sym;
                int vKey = mapKey(keyboardMap, key);
                printf("vKey = %x is down\n", vKey);
                if (vKey != -1)
                {
                    holdKeyDown(&chip8.keyboard, vKey);
                }

                break;
            }

            case SDL_KEYUP:
                char key = event.key.keysym.sym;
                int vKey = mapKey(keyboardMap, key);
                printf("vKey = %x is up\n", vKey);
                if (vKey != -1)
                {
                    releaseKey(&chip8.keyboard, vKey);
                }
                break;

            default:
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        SDL_Rect r;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        r.x = 320;
        r.y = 160;
        r.w = 100;
        r.h = 100;

        SDL_RenderFillRect(renderer, &r);
        SDL_RenderPresent(renderer);
        usleep(10000);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}