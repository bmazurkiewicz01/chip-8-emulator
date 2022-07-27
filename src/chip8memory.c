#include <stdbool.h>
#include <stdio.h>

#include "chip8memory.h"

static bool isMemoryInBounds(int index)
{
    if (index < 0 || index >= CHIP8_MEMORY_SIZE)
    {
        return false;
    }
    return true;
}

unsigned char getMemory(struct Chip8Memory *memory, int index)
{
    if (isMemoryInBounds(index))
    {
        return memory->memory[index];
    }
    else
    {
        printf("Error: index is not within bounds");
        return 0;
    }
}

void setMemory(struct Chip8Memory *memory, int index, unsigned char val)
{
    if (isMemoryInBounds(index))
    {
        memory->memory[index] = val;
    }
    else
    {
        printf("Error: index is not within bounds");
    }
}