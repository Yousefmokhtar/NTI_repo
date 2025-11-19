// #include <Arduino.h>
// #include "../APP_cfg.h"
// #include "AlarmSystem.h"
// #include "../HAL/GPIO/gpio.h"
// #include "POT.h"
// #include "PWM.h"
// #include <math.h>

// #if ALARM__DEBUG == STD_ON
// #define DEBUG_PRINTLN(var) Serial.println(var)
// #else
// #define DEBUG_PRINTLN(var)
// #endif

// ALARM_t alarmsyatem = {ALARM_LED_HIGH_PIN, ALARM_LED_LOW_PIN, ALARM_LED_DIMMER_PWM_CHANNEL, FRIST_ALARM_STATE};

// void Alarm_Init(void)
// {
// #if ALARM_ENABLED == STD_ON
//     pinMode(alarmsyatem.ledHigh, GPIO_OUTPUT);
//     pinMode(alarmsyatem.ledLow, GPIO_OUTPUT);
//     DEBUG_PRINTLN("Intialized LED Pins");
//     POT_init();
//     PWM_Init(alarmsyatem.ledDimmer);
//     DEBUG_PRINTLN("Finsh Intialization");
// #endif
// }

// void Alarm_main(void)
// {
// #if ALARM_ENABLED == STD_ON

//     DEBUG_PRINTLN("start Alarm System");
//     float pot_value = POT_value_Geter();

//     int percentage = map(pot_value, MIN_POT_VALUE, MAX_POT_VALUE, MIN_PERCENTAGE, MAX_PERCENTAGE);
//     DEBUG_PRINTLN("The percentage of the Voltage: %d" + String(percentage));
//     if (percentage >= 80)
//     {
//         write_pin_high(alarmsyatem.ledHigh);
//         write_pin_Low(alarmsyatem.ledLow);
//         PWM_SetDuty(alarmsyatem.ledDimmer, (float)ALARM_HIGH_THRESHOLD_PERCENTAGE);
//         DEBUG_PRINTLN(" IN HIGH Alarm");
//     }
//     else if (percentage > 30 && percentage < 80)
//     {
//         write_pin_Low(alarmsyatem.ledHigh);
//         write_pin_Low(alarmsyatem.ledLow);
//         PWM_SetDuty(alarmsyatem.ledDimmer, (float)percentage);
//         DEBUG_PRINTLN(" IN Norma; Alarm");
//     }
//     else if (percentage <= 30)
//     {
//         write_pin_Low(alarmsyatem.ledHigh);
//         write_pin_high(alarmsyatem.ledLow);
//         PWM_SetDuty(alarmsyatem.ledDimmer, (float)ALARM_LOW_THRESHOLD_PERCENTAGE);
//         DEBUG_PRINTLN(" IN LOW Alarm");
//     }
//     else
//     {
//         write_pin_Low(alarmsyatem.ledHigh);
//         write_pin_Low(alarmsyatem.ledLow);
//         write_pin_Low(alarmsyatem.ledDimmer);
//         DEBUG_PRINTLN(" NO Alarm");
//     }
// #endif
// }

#include <Arduino.h>
#include "../APP_cfg.h"
#include "AlarmSystem.h"
#include "../HAL/GPIO/gpio.h"
#include "POT.h"
#include "PWM.h"
#include <math.h>

#if ALARM__DEBUG == STD_ON
#define DEBUG_PRINTLN(var) Serial.println(var)
#else
#define DEBUG_PRINTLN(var)
#endif

ALARM_t alarmsyatem = {ALARM_LED_HIGH_PIN, ALARM_LED_LOW_PIN, ALARM_LED_DIMMER_PWM_CHANNEL, FRIST_ALARM_STATE};

void Alarm_Init(void)
{
#if ALARM_ENABLED == STD_ON
    pinMode(alarmsyatem.ledHigh, OUTPUT);
    pinMode(alarmsyatem.ledLow, OUTPUT);
    DEBUG_PRINTLN("Intialized LED Pins");
    POT_init();
    PWM_Init(alarmsyatem.ledDimmer);
    DEBUG_PRINTLN("Finsh Intialization");
#endif
}

ALARM_STATES_t Alarm_Transition(float prcent_value)
{
    if (prcent_value >= 80)
        return HIGH_ALARM;
    else if (prcent_value <= 30)
        return LOW_ALARM;
    else if (prcent_value > 30 && prcent_value < 80)
        return NORMAL_ALARM;
}

void enterState(ALARM_STATES_t state, float dutyCycle)
{
    alarmsyatem.state = state;

    // reset all LEDs
    write_pin_Low(alarmsyatem.ledHigh);
    write_pin_Low(alarmsyatem.ledLow);
    write_pin_Low(alarmsyatem.ledDimmer);

    switch (state)
    {
    case NORMAL_ALARM:
    {
        write_pin_Low(alarmsyatem.ledHigh);
        write_pin_Low(alarmsyatem.ledLow);
        PWM_SetDuty(alarmsyatem.ledDimmer, (float)dutyCycle);
        DEBUG_PRINTLN(" IN Norma Alarm");
    }
    break;
    case HIGH_ALARM:
    {
        write_pin_high(alarmsyatem.ledHigh);
        write_pin_Low(alarmsyatem.ledLow);
        PWM_SetDuty(alarmsyatem.ledDimmer, (float)ALARM_HIGH_THRESHOLD_PERCENTAGE);
        DEBUG_PRINTLN(" IN HIGH Alarm");
    }
    break;
    case LOW_ALARM:
    {
        write_pin_Low(alarmsyatem.ledHigh);
        write_pin_high(alarmsyatem.ledLow);
        PWM_SetDuty(alarmsyatem.ledDimmer, (float)ALARM_LOW_THRESHOLD_PERCENTAGE);
        DEBUG_PRINTLN(" IN LOW Alarm");
    }
    break;
    }
}
void Alarm_main(void)
{
#if ALARM_ENABLED == STD_ON

    DEBUG_PRINTLN("start Alarm System");
    uint16_t pot_value = POT_value_Getter();

    int percentage = map(pot_value, MIN_POT_VALUE, MAX_POT_VALUE, MIN_PERCENTAGE, MAX_PERCENTAGE);
    DEBUG_PRINTLN("The percentage of the Voltage: %d" + String(percentage));

    enterState(Alarm_Transition((float)percentage), percentage);
#endif
}