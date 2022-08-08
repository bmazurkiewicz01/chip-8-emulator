#ifndef CHIP8SCREEN_H
#define CHIP8SCREEN_H

#include <stdbool.h>
#include "config.h"

struct Chip8Screen
{
    bool pixels[CHIP8_HEIGHT][CHIP8_WIDTH];
};

void setScreen(struct Chip8Screen *screen, int x, int y);
bool isScreenSet(struct Chip8Screen *screen, int x, int y);
bool drawSprite(struct Chip8Screen *screen, int x, int y, const char *sprite, int num);
void clearScreen(struct Chip8Screen *screen);

#endif