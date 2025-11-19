#include <Arduino.h>
#include "src/App_cfg.h"
#include "src/APP/AlarmSystem.h"

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    Alarm_Init();
}

void loop()
{
    Alarm_main();
    delay(1000);
}
