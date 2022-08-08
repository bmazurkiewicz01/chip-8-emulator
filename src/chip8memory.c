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
        printf("Error: memory index is not within bounds");
        return 0;
    }
}

unsigned short getMemoryAsShort(struct Chip8Memory *memory, int index)
{
    unsigned char byte1 = getMemory(memory, index);
    unsigned char byte2 = getMemory(memory, index + 1);

    return byte1 << 8 | byte2;
}

void setMemory(struct Chip8Memory *memory, int index, unsigned char val)
{
    if (isMemoryInBounds(index))
    {
        memory->memory[index] = val;
    }
    else
    {
        printf("Error: memory index is not within bounds");
    }
}