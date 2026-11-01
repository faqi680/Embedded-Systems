#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---- PWM setup ----
const int ledPin1 = 18;
const int ledPin2 = 19;
const int freq1 = 1000;        // LED 1: 1 kHz
const int freq2 = 5000;        // LED 2: 5 kHz
const int channel1 = 0;        // channels 0 and 2 -> different LEDC timers
const int channel2 = 2;
const int resolution = 8;      // 8-bit: 0-255
const int maxDuty = 255;

// ---- OLED setup ----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C         // use the address your scanner found
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int duty1 = 0, duty2 = 0;      // values actually written to LEDC

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("PWM MONITOR");
  display.setCursor(0, 20);
  display.printf("LED1: %3d%% F:%dHz\n", duty1 * 100 / maxDuty, freq1);
  display.setCursor(0, 40);
  display.printf("LED2: %3d%% F:%dHz\n", duty2 * 100 / maxDuty, freq2);
  display.display();           // nothing appears without this
}

void setup() {
  Serial.begin(115200);

  ledcSetup(channel1, freq1, resolution);
  ledcSetup(channel2, freq2, resolution);
  ledcAttachPin(ledPin1, channel1);
  ledcAttachPin(ledPin2, channel2);

  Wire.begin(21, 22);          // SDA, SCL
  Wire.setClock(400000);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("SSD1306 init failed - check wiring/address");
    while (true) delay(1000);
  }
  display.clearDisplay();
  display.display();
}

void loop() {
  for (int d = 0; d <= maxDuty; d++) {
    duty1 = d;
    duty2 = maxDuty - d;
    ledcWrite(channel1, duty1);
    ledcWrite(channel2, duty2);
    if (d % 3 == 0) updateDisplay();   // refresh every few steps
    delay(10);
  }
  for (int d = maxDuty; d >= 0; d--) {
    duty1 = d;
    duty2 = maxDuty - d;
    ledcWrite(channel1, duty1);
    ledcWrite(channel2, duty2);
    if (d % 3 == 0) updateDisplay();
    delay(10);
  }
}