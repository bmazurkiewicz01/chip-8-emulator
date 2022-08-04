#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#include "config.h"
#include "chip8beep.h"

static void *beepRoutine(void *arg)
{
    unsigned char *soundTimerValue = (unsigned char *)arg;
    printf("STV = %d\n", *soundTimerValue);

    char result[160];
    sprintf(result, "powershell.exe"
                    " -NoProfile -NonInteractive -Command \"[console]::beep(%d, %d)\"",
            BEEP_FREQUENCY, 100 * *soundTimerValue);
    system(result);

    return NULL;
}

void beep(unsigned char soundTimerValue)
{
    pthread_t beepThread;
    printf("beep st = %d\n", soundTimerValue);

    if (pthread_create(&beepThread, NULL, &beepRoutine, (void *)&soundTimerValue) != 0)
    {
        printf("Unable to create the beep thread.\n");
    }
    usleep(10);
}