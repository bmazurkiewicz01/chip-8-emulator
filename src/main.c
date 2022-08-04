#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "SDL.h"
#include "chip8.h"
#include "chip8keyboard.h"
#include "chip8screen.h"
#include "chip8beep.h"

const char keyboardMap[CHIP8_TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4,
    SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9,
    SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f};

int main(int argc, char **argv)
{
    struct Chip8 chip8;
    bool isRunning = true;

    initChip8(&chip8);
    chip8.registers.ST = 255;

    struct Beeper beeper;
    initBeeper(&beeper);

    drawSprite(&chip8.screen, 0, 0, &chip8.memory.memory[0x05], 5);
    drawSprite(&chip8.screen, 20, 20, &chip8.memory.memory[0x0A], 5);
    drawSprite(&chip8.screen, 40, 20, &chip8.memory.memory[60], 5);
    drawSprite(&chip8.screen, 62, 30, &chip8.memory.memory[65], 5);

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
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

        for (int x = 0; x < CHIP8_WIDTH; x++)
        {
            for (int y = 0; y < CHIP8_HEIGHT; y++)
            {
                if (isScreenSet(&chip8.screen, x, y))
                {
                    SDL_Rect r;

                    r.x = x * EMULATOR_WINDOW_MULTIPLIER;
                    r.y = y * EMULATOR_WINDOW_MULTIPLIER;
                    r.w = EMULATOR_WINDOW_MULTIPLIER;
                    r.h = EMULATOR_WINDOW_MULTIPLIER;

                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }
        SDL_RenderPresent(renderer);

        if (chip8.registers.DT > 0)
        {
            usleep(100 * 1000);
            chip8.registers.DT -= 1;
        }

        if (chip8.registers.ST > 0)
        {
            beep(&beeper, chip8.registers.ST);
            chip8.registers.ST = 0;
        }
        usleep(20 * 1000);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    destroyBeeper(&beeper);
    return 0;
}