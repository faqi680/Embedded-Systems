#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin & PWM configurations
const int ledPin1 = 18;       
const int ledPin2 = 19;       

const int freq1 = 1000;       // 1 kHz
const int freq2 = 5000;       // 5 kHz

const int channel1 = 0;       
const int channel2 = 1;       
const int resolution = 8;     

void setup() {
  Serial.begin(115200);

  // Initialize I2C with pins D21 (SDA) and D22 (SCL)
  Wire.begin(21, 22);

  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    while(1);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Setup PWM channels
  ledcSetup(channel1, freq1, resolution);
  ledcSetup(channel2, freq2, resolution);
  ledcAttachPin(ledPin1, channel1);
  ledcAttachPin(ledPin2, channel2);
}

void loop() {
  // Fade up loop
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    ledcWrite(channel1, dutyCycle);
    ledcWrite(channel2, 255 - dutyCycle);
    
    updateOLED(dutyCycle, 255 - dutyCycle);
    delay(15);
  }
  
  // Fade down loop
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    ledcWrite(channel1, dutyCycle);
    ledcWrite(channel2, 255 - dutyCycle);
    
    updateOLED(dutyCycle, 255 - dutyCycle);
    delay(15);
  }
}

void int updateOLED(int val1, int val2) {
  int percent1 = (val1 * 100) / 255;
  int percent2 = (val2 * 100) / 255;

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("   PWM MONITOR"));
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  display.setCursor(0, 20);
  display.print(F("LED1: "));
  display.print(percent1);
  display.print(F("% "));
  display.setCursor(0, 30);
  display.print(F("F:")); display.print(freq1); display.print(F("Hz"));

  display.setCursor(0, 45);
  display.print(F("LED2: "));
  display.print(percent2);
  display.print(F("% "));
  display.setCursor(0, 55);
  display.print(F("F:")); display.print(freq2); display.print(F("Hz"));

  display.display();
}