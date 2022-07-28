#ifndef CHIP8KEYBOARD_H
#define CHIP8KEYBOARD_H

#include <stdbool.h>
#include "config.h"

struct Chip8Keyboard
{
    bool keyboard[CHIP8_TOTAL_KEYS];
};

int mapKey(const char *map, char key);
void holdKeyDown(struct Chip8Keyboard *keyboard, int key);
void releaseKey(struct Chip8Keyboard *keyboard, int key);
bool isKeyDown(struct Chip8Keyboard *keyboard, int key);

#endif