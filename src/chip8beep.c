#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#include "config.h"
#include "chip8beep.h"

static void *beepRoutine(void *arg)
{
    struct Beeper *beeper = (struct Beeper *)arg;
    while (beeper->isBeeperRunning)
    {
        pthread_mutex_lock(&beeper->mutex);
        while (beeper->isBeeperWaiting)
        {
            pthread_cond_wait(&beeper->cond, &beeper->mutex);
        }

        if (beeper->soundTimerValue == 0)
        {
            printf("Sound timer value is equal to 0");
        }
        else
        {
            char result[160];
            sprintf(result, "powershell.exe"
                            " -NoProfile -NonInteractive -Command \"[console]::beep(%d, %d)\"",
                    BEEP_FREQUENCY, 100 * beeper->soundTimerValue);
            system(result);
        }

        beeper->isBeeperWaiting = true;
        pthread_mutex_unlock(&beeper->mutex);
    }

    return NULL;
}

void initBeeper(struct Beeper *beeper)
{
    pthread_mutex_init(&beeper->mutex, NULL);
    pthread_cond_init(&beeper->cond, NULL);
    beeper->isBeeperRunning = true;
    beeper->isBeeperWaiting = true;

    if (pthread_create(&beeper->beepThread, NULL, &beepRoutine, (void *)beeper) != 0)
    {
        printf("Unable to create the beep thread.\n");
    }
}

void destroyBeeper(struct Beeper *beeper)
{
    pthread_mutex_lock(&beeper->mutex);
    beeper->isBeeperRunning = false;
    beeper->isBeeperWaiting = false;
    beeper->soundTimerValue = 0;
    pthread_mutex_unlock(&beeper->mutex);
    pthread_cond_signal(&beeper->cond);

    if (pthread_join(beeper->beepThread, NULL) != 0)
    {
        pthread_cond_destroy(&beeper->cond);
        pthread_mutex_destroy(&beeper->mutex);
    }
}

void beep(struct Beeper *beeper, unsigned char soundTimerValue)
{
    pthread_mutex_lock(&beeper->mutex);
    beeper->isBeeperWaiting = false;
    beeper->soundTimerValue = soundTimerValue;
    pthread_mutex_unlock(&beeper->mutex);

    pthread_cond_signal(&beeper->cond);
}