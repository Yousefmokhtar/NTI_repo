#ifndef _ALARM_H
#define _ALARM_H
#include <stdint.h>

typedef enum
{
    NORMAL_ALARM,
    HIGH_ALARM,
    LOW_ALARM
} ALARM_STATES_t;

typedef struct
{
    uint8_t ledHigh;
    uint8_t ledLow;
    uint8_t ledDimmer;
    ALARM_STATES_t state;
} ALARM_t;

void Alarm_Init(void);
void Alarm_main(void);

#endif