#include <stdbool.h>
#include <stdio.h>

#include "chip8stack.h"
#include "chip8.h"

static bool isStackInBounds(struct Chip8 *chip8)
{
    if (chip8->registers.SP >= CHIP8_STACK_SIZE)
    {
        return false;
    }
    return true;
}

void pushStack(struct Chip8 *chip8, unsigned short val)
{
    if (isStackInBounds(chip8))
    {
        chip8->stack.stack[chip8->registers.SP] = val;
        chip8->registers.SP += 1;
    }
    else
    {
        printf("Error: stack is not within bounds");
    }
}

unsigned short popStack(struct Chip8 *chip8)
{
    if (isStackInBounds(chip8))
    {
        chip8->registers.SP -= 1;
        return chip8->stack.stack[chip8->registers.SP];
    }
    else
    {
        printf("Error: stack is not within bounds");
        return 0;
    }
}