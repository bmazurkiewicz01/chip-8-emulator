#include <stdio.h>
#include <memory.h>
#include <stdbool.h>

#include "chip8.h"

const char chip8DefaultCharacterSet[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    0xF0, 0x90, 0xF0, 0x80, 0x80};

void initChip8(struct Chip8 *chip8)
{
    memset(chip8, 0, sizeof(struct Chip8));
    memcpy(&chip8->memory.memory, chip8DefaultCharacterSet, sizeof(chip8DefaultCharacterSet));
}

void loadChip8(struct Chip8 *chip8, const char *buffer, size_t bufferSize)
{
    if ((bufferSize + CHIP8_LOAD_ADDRESS) >= CHIP8_MEMORY_SIZE)
    {
        printf("Error: Buffer size is not within the bounds.\n");
    }
    else
    {
        memcpy(&chip8->memory.memory[CHIP8_LOAD_ADDRESS], buffer, bufferSize);
        chip8->registers.PC = CHIP8_LOAD_ADDRESS;
    }
}

static void execExtendedChip8(struct Chip8 *chip8, unsigned short opcode)
{
    unsigned short addr = opcode & 0x0FFF;
    unsigned char x = (opcode >> 8) & 0x000F;
    unsigned char y = (opcode >> 4) & 0x000F;
    unsigned short kk = opcode & 0x00FF;
    unsigned char charResult = 0;
    unsigned short shortResult = 0;

    switch (opcode & 0xF000)
    {
    // Jumps to location addr
    case 0x1000:
        chip8->registers.PC = addr;
        break;

    // Calls subroutine at addr
    case 0x2000:
        pushStack(chip8, chip8->registers.PC);
        chip8->registers.PC = addr;
        break;

    // Skips next instruction if register indexed with the number x is EQUAL to kk
    case 0x3000:
        if (chip8->registers.V[x] == kk)
        {
            chip8->registers.PC += 2;
        }
        break;

    // Skips next instruction if register indexed with the number x is NOT EQUAL to kk
    case 0x4000:
        if (chip8->registers.V[x] != kk)
        {
            chip8->registers.PC += 2;
        }
        break;

    // Skips next insctruction if register indexed with the number x is EQUAL to register indexed with the number y
    case 0x5000:
        if (chip8->registers.V[x] == chip8->registers.V[y])
        {
            chip8->registers.PC += 2;
        }
        break;

    // Puts the value kk to the register indexed with the number x
    case 0x6000:
        chip8->registers.V[x] = kk;
        break;

    // Adds the value kk to the value of register indexed with the number x
    case 0x7000:
        chip8->registers.V[x] += kk;
        break;

    // It is more complex command and handles six operations
    case 0x8000:
        switch (opcode & 0x000F)
        {
        // Sets register indexed with the number x to the value of register indexed with the number y
        case 0x00:
            chip8->registers.V[x] = chip8->registers.V[y];
            break;

        // Performs a bitwise OR on the values of register indexed with the number x and register indexed with the number y
        // Then stores the result of the operation in the register indexed with the number x
        case 0x01:
            charResult = chip8->registers.V[x] | chip8->registers.V[y];
            chip8->registers.V[x] = charResult;
            break;

        // Performs a bitwise AND on the values of register indexed with the number x and register indexed with the number y
        // Then stores the result of the operation in the register indexed with the number x
        case 0x02:
            charResult = chip8->registers.V[x] & chip8->registers.V[y];
            chip8->registers.V[x] = charResult;
            break;

        // Performs a bitwise XOR on the values of register indexed with the number x and register indexed with the number y
        // Then stores the result of the operation in the register indexed with the number x
        case 0x03:
            charResult = chip8->registers.V[x] ^ chip8->registers.V[y];
            chip8->registers.V[x] = charResult;
            break;

        // Adds the value of the register indexed with the number y to the register indexed with the number x
        // If the result of the sum operation is greater than 8 bits register indexed with number 0x0F is set to 1, otherwise 0.
        case 0x04:
            shortResult = chip8->registers.V[x] + chip8->registers.V[y];
            chip8->registers.V[0x0F] = shortResult > 0xFF;
            chip8->registers.V[x] = shortResult;
            break;

        // Substracts the value of the register indexed with the number y from the value of the register indexed with the number x
        // If the value of the register indexed with the number x is greater than the register indexed with the number y
        // Then the register indexed with the number 0x0F is set to 1, otherwise 0
        case 0x05:
            chip8->registers.V[0x0F] = chip8->registers.V[x] > chip8->registers.V[y];
            chip8->registers.V[x] -= chip8->registers.V[y];
            break;

        // If the least-significant bit of register indexed with the number x is 1, then the value of the register indexed with the number 0x0F is set to 1, otherwise 0
        // Then the value of the register indexed with the number x is divided by 2
        case 0x06:
            chip8->registers.V[0x0F] = chip8->registers.V[x] & 0x01;
            chip8->registers.V[x] /= 2;
            break;

        // If the value of the register indexed with the number y is greater than the value of the register indexed with the number x
        // Then the register indexed with the number 0x0F is set to 1, otherwise 0
        // Then the value of the register indexed with the number x is subtracted from the value of the register indexed with the number y,
        // and the results stored in the register indexed with the number x
        case 0x07:
            chip8->registers.V[0x0F] = chip8->registers.V[y] > chip8->registers.V[x];
            break;

        default:
            break;
        }
        break;

    default:
        printf("Error: wrong opcode value: %X. Can't execute chip8 operation.\n", opcode);
        break;
    }
}

void execChip8(struct Chip8 *chip8, unsigned short opcode)
{
    switch (opcode)
    {
    // Clears the display
    case 0x00E0:
        clearScreen(&chip8->screen);
        break;

    // Returns from a subroutine
    case 0x00EE:
        chip8->registers.PC = popStack(chip8);
        break;

    // Goes to the extended execution function
    default:
        execExtendedChip8(chip8, opcode);
        break;
    }
}