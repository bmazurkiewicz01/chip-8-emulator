#ifndef CHIP8BEEP_H
#define CHIP8BEEP_H

#include <pthread.h>
#include <stdbool.h>

struct Beeper {
    pthread_t beepThread;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    unsigned char soundTimerValue;
    bool isBeeperRunning;
    bool isBeeperWaiting;
};

void initBeeper(struct Beeper *beeper);
void destroyBeeper(struct Beeper *beeper);

void beep(struct Beeper *beeper, unsigned char soundTimerValue);

#endif