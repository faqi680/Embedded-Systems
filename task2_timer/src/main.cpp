 //Week3-Lecture2
// Timer Interrupt (Internal)
// Embedded IoT System Fall-2026

// Name: Muhammad Faqi Ul Hassnain
// Reg#: 24-NTU-CS-FL-1061

#include <Arduino.h>

#define LED 2

hw_timer_t *My_timer = NULL;

// ISR - Interrupt Service Routine
void IRAM_ATTR onTimer()
{
    digitalWrite(LED, !digitalRead(LED));
}

void setup()
{
    pinMode(LED, OUTPUT);

    // Timer 0
    // ESP32 clock = 80 MHz
    // Divider = 80
    // 80 MHz / 80 = 1 MHz
    // Therefore, 1 tick = 1 microsecond

    My_timer = timerBegin(0, 80, true);

    // Attach ISR to timer
    timerAttachInterrupt(My_timer, &onTimer, true);

    // Trigger interrupt every 1,000,000 microseconds
    // = 1 second
    // true = repeat automatically

    timerAlarmWrite(My_timer, 1000000, true);

    // Enable timer alarm
    timerAlarmEnable(My_timer);
}

void loop()
{
    // Nothing needed
}