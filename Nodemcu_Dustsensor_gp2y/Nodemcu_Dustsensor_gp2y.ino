#define BLYNK_TEMPLATE_ID "TMPL3MuOXuIZB"
#define BLYNK_TEMPLATE_NAME "Dust Eater Robot"
#define BLYNK_AUTH_TOKEN "mN4aKWE3KhbT-vBESN9PvrXR0c2Ttv1j"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>    

BlynkTimer timer;

// Initialize LCD object with I2C address 0x27, 16 columns, and 2 rows
LiquidCrystal_I2C lcd(0x27,16,2);

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "iotlab";
char pass[] = "password";

// Pin definitions
int measurePin = A0;    // Pin connected to dust sensor analog output
int ledPower = D0;      // Pin connected to dust sensor LED power
int fan_control = D8;   // Pin connected to control the fan

// Parameters for dust sensor calculation
unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

// Blynk virtual pin handler for LED control
BLYNK_WRITE(V0)
{
  int value = param.asInt();
  Serial.println(value);
  if(value == 1)
  {
    digitalWrite(D8, HIGH);
    Serial.println("LED ON");
  }
  if(value == 0)
  {
    digitalWrite(D8, LOW);
    Serial.println("LED OFF");
  }
}

// Function to read and send sensor data to Blynk and display on LCD
void sendSensor()
{
  digitalWrite(ledPower, LOW);
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin);
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH);
  delayMicroseconds(sleepTime);
  
  calcVoltage = voMeasured * (5.0 / 1024);
  dustDensity = 170 * calcVoltage - 0.1;

  // Display sensor data on LCD
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("-> Dust Density : ");
  lcd.setCursor(3, 1);
  lcd.print(dustDensity);
  lcd.setCursor(9, 1);
  lcd.print(" ug/m3  ");

  // Send sensor data to Blynk
  Blynk.virtualWrite(V0, dustDensity); 
}

void setup()
{
  pinMode(measurePin, INPUT);
  pinMode(ledPower, OUTPUT);
  pinMode(fan_control, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensor);
  
  Serial.begin(115200);
  
  lcd.begin(); 
  lcd.backlight();
  lcd.clear();
}

void loop()
{
  Blynk.run();
  timer.run();
}
