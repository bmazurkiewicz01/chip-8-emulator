#include <stdio.h>

#include "SDL.h"
#include "chip8.h"

int main(int argc, char **argv)
{
    struct Chip8 chip8;
    setMemory(&chip8.memory, 0, 'Z');
    printf("%c\n", getMemory(&chip8.memory, 0));

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow(
        EMULATOR_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        CHIP8_WIDTH * EMULATOR_WINDOW_MULTIPLIER,
        CHIP8_HEIGHT * EMULATOR_WINDOW_MULTIPLIER,
        SDL_WINDOW_SHOWN);
    printf("Hello World!\n");

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
    while (1)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                goto out;
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
    }

out:
    SDL_DestroyWindow(window);
    return 0;
}