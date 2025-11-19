#ifndef PWM_H
#define PWM_H

#include <stdint.h>

// Initialize PWM module
void PWM_Init(uint8_t pin);
// Set PWM duty cycle (0.0 to 100.0)
void PWM_SetDuty(uint8_t channel, float duty_percent);

#endif
