#include "PWM.h"
#include "../APP_cfg.h"
#include <Arduino.h> // لو على ESP32/Arduino، ممكن تغيري حسب platform

void PWM_Init(uint8_t pin)
{
    pinMode(pin, OUTPUT);
}

// duty_percent: 0.0 → 100.0
void PWM_SetDuty(uint8_t pin, float duty_percent)
{
    if (duty_percent < 0.0f)
    {
        duty_percent = 0.0f;
    }
    if (duty_percent > 100.0f)
    {
        duty_percent = 100.0f;
    }

    uint8_t pwm_value = (uint8_t)((duty_percent / 100.0f) * 255.0f);
    analogWrite(pin, pwm_value);
}
