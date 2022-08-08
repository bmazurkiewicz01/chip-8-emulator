#include <stdio.h>
#include <memory.h>
#include "chip8screen.h"

static bool arePixelsInBounds(int x, int y)
{
    if (x < 0 || x >= CHIP8_WIDTH || y < 0 || y >= CHIP8_HEIGHT)
    {
        return false;
    }
    return true;
}

void setScreen(struct Chip8Screen *screen, int x, int y)
{
    if (arePixelsInBounds(x, y))
    {
        screen->pixels[y][x] = true;
    }
    else
    {
        printf("Error: pixels are not within bounds");
    }
}

bool isScreenSet(struct Chip8Screen *screen, int x, int y)
{
    if (arePixelsInBounds(x, y))
    {
        return screen->pixels[y][x];
    }
    else
    {
        printf("Error: pixels are not within bounds");
        return false;
    }
}

bool drawSprite(struct Chip8Screen *screen, int x, int y, const char *sprite, int num)
{
    bool isPixelChanged = false;

    for (int lY = 0; lY < num; lY++)
    {
        char c = sprite[lY];
        for (int lX = 0; lX < 8; lX++)
        {
            if ((c & (0b10000000 >> lX)) != 0)
            {
                if (screen->pixels[(lY + y) % CHIP8_HEIGHT][(lX + x) % CHIP8_WIDTH])
                {
                    isPixelChanged = true;
                }
                screen->pixels[(lY + y) % CHIP8_HEIGHT][(lX + x) % CHIP8_WIDTH] ^= true;
            }
        }
    }

    return isPixelChanged;
}

void clearScreen(struct Chip8Screen *screen)
{
    memset(screen->pixels, 0, sizeof(screen->pixels));
}