#ifndef CHIP8KEYBOARD_H
#define CHIP8KEYBOARD_H

#include <stdbool.h>
#include "config.h"

struct Chip8Keyboard
{
    bool keyboard[CHIP8_TOTAL_KEYS];
    const char *keyboardMap;
};

void setKeyboardMap(struct Chip8Keyboard *keyboard, const char* map);
int mapKey(struct Chip8Keyboard *keyboard, char key);
void holdKeyDown(struct Chip8Keyboard *keyboard, int key);
void releaseKey(struct Chip8Keyboard *keyboard, int key);
bool isKeyDown(struct Chip8Keyboard *keyboard, int key);

#endif