#ifndef CHIP8STACK_H
#define CHIP8STACK_H

#include "config.h"

struct Chip8;

struct Chip8Stack 
{
    unsigned short stack[CHIP8_STACK_SIZE];
};

void pushStack(struct Chip8 *chip8, unsigned short val);
unsigned short popStack(struct Chip8 *chip8);

#endif