#include <stdio.h>
#include "chip8keyboard.h"

static bool isKeyInBounds(int key)
{
    if (key < 0 || key >= CHIP8_TOTAL_KEYS)
    {
        return false;
    }
    return true;
}

void setKeyboardMap(struct Chip8Keyboard *keyboard, const char* map)
{
    keyboard->keyboardMap = map;
}

int mapKey(struct Chip8Keyboard *keyboard, char key)
{
    for (int i = 0; i < CHIP8_TOTAL_KEYS; ++i)
    {
        if (keyboard->keyboardMap[i] == key)
        {
            return i;
        }
    }
    return -1;
}

void holdKeyDown(struct Chip8Keyboard *keyboard, int key)
{
    if (isKeyInBounds(key))
    {
        keyboard->keyboard[key] = true;
    }
    else
    {
        printf("Error: key is not within bounds");
    }
}

void releaseKey(struct Chip8Keyboard *keyboard, int key)
{
    if (isKeyInBounds(key))
    {
        keyboard->keyboard[key] = false;
    }
    else
    {
        printf("Error: key is not within bounds");
    }
}
bool isKeyDown(struct Chip8Keyboard *keyboard, int key)
{
    if (isKeyInBounds(key))
    {
        return keyboard->keyboard[key];
    }
    else
    {
        printf("Error: key is not within bounds");
        return false;
    }
}