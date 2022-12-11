#include <SoftwareSerial.h>
SoftwareSerial nodemcu(2, 3); //rx-3....tx-2
String cdata;

int soundsensor = A0;

#include <SPI.h>
#include <Wire.h>

int measurePin = A2;
int ledPower = 4;

unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBoldOblique24pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define bitmap_height   128
#define bitmap_width    64


void setup() {
  Serial.begin(9600);
  nodemcu.begin(9600);
  pinMode(soundsensor, INPUT);
  pinMode(ledPower, OUTPUT);


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  // Clear the buffer
  display.clearDisplay();

}

void loop() {
  float status_sound = analogRead(soundsensor);
  float pressure = 1;

  digitalWrite(ledPower, LOW);
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin);

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH);
  delayMicroseconds(sleepTime);

  calcVoltage = voMeasured * (5.0 / 1024);
  dustDensity = 0.17 * calcVoltage - 0.1;

  if ( dustDensity < 0)
  {
    dustDensity = 0.00;
  }

  float dust = dustDensity;

  cdata = String(status_sound) + "," + String(pressure) + "," + String(dust);
  nodemcu.println(cdata);

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Sound : ");
  display.println(status_sound);
  display.println("");
  display.print("Pressure : ");
  display.print(pressure);
  display.println(" hPa");
  display.println("");
  display.print("Dust : ");
  display.print(dustDensity);
  display.println(" μg/m3");

  display.display();

  delay(1000);
}
