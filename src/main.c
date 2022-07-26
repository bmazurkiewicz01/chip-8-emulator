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
    const char *fileName = "INVADERS";

    FILE *file = fopen(fileName, "rb");
    if (!file)
    {
        printf("Error: failed to open the file %s\n", fileName);
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char buffer[size];
    int result = fread(buffer, size, 1, file);
    if (result != 1)
    {
        printf("Error: failed to read from file %s\n", fileName);
        return -2;
    }
    buffer[size] = '\0';
    printf("%s\n", buffer);

    struct Chip8 chip8;
    bool isRunning = true;

    initChip8(&chip8);
    loadChip8(&chip8, buffer, size);
    setKeyboardMap(&chip8.keyboard, keyboardMap);

#if CHIP8_BEEPER_ACTIVE == true
    struct Beeper beeper;
    initBeeper(&beeper);
#endif

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
                int vKey = mapKey(&chip8.keyboard, key);
                if (vKey != -1)
                {
                    holdKeyDown(&chip8.keyboard, vKey);
                }

                break;
            }

            case SDL_KEYUP:
                char key = event.key.keysym.sym;
                int vKey = mapKey(&chip8.keyboard, key);
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
            usleep(CHIP8_MAIN_DELAY);
            chip8.registers.DT -= 1;
        }

#if CHIP8_BEEPER_ACTIVE == true
        if (chip8.registers.ST > 0)
        {
            beep(&beeper, chip8.registers.ST);
            chip8.registers.ST = 0;
        }
#endif

        unsigned short opcode = getMemoryAsShort(&chip8.memory, chip8.registers.PC);
        chip8.registers.PC += 2;
        execChip8(&chip8, opcode);
        usleep(CHIP8_LOOP_DELAY);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
#if CHIP8_BEEPER_ACTIVE == true
    destroyBeeper(&beeper);
#endif
    return 0;
}