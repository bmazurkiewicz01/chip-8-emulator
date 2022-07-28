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

int mapKey(const char *map, char key)
{
    for (int i = 0; i < CHIP8_TOTAL_KEYS; ++i)
    {
        if (map[i] == key)
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