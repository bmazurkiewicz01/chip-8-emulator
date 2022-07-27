#ifndef CHIP8MEMORY_H
#define CHIP8MEMORY_H

#include "config.h"

struct Chip8Memory
{
    unsigned char memory[CHIP8_MEMORY_SIZE];
};

unsigned char getMemory(struct Chip8Memory *memory, int index);

void setMemory(struct Chip8Memory *memory, int index, unsigned char val);

#endif