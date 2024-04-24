#include <Wire.h>  // This library is already built into the Arduino IDE
#include <LiquidCrystal_I2C.h> // This library can be added via Include Library > Manage Library
LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize LCD object with I2C address 0x27, 16 columns, and 2 rows

int muestreo = 0; // Analog pin used for sampling
int IRED = 12; // Digital pin connected to the Infrared LED

// Timing parameters for dust sensor sampling
int samplingTime = 280; 
int deltaTime = 40; 
int sleepTime = 9680;

int value = 0; // Variable to store analog reading
float r1 = 0; // Timing variable for delay
float r2 = 0; // Timing variable for delay
float r3 = 0; // Timing variable for delay
int i = 0; // Counter variable

float ppm = 0; // Variable to accumulate analog readings
float voltage = 0; // Calculated voltage from analog readings
float density_polvo = 0; // Dust density variable
float ppm_real = 0; // Real ppm value variable

void setup() {
  lcd.begin(16, 2); // Initialize the LCD display with 16 columns and 2 rows
  pinMode(IRED, OUTPUT); // Set IRED pin as output
  
  lcd.clear(); // Clear the LCD display
  lcd.setCursor(0, 0); // Set cursor to the beginning of the first line
  lcd.print("Dust Particles: "); // Print message on the first line
  lcd.setCursor(0, 1); // Set cursor to the beginning of the second line
  lcd.print("    METER"); // Print message on the second line
  delay(3000); // Delay for 3 seconds
}

void loop() {
  i = i + 1; // Increment counter

  digitalWrite(IRED, LOW); // Turn off Infrared LED
  delayMicroseconds(r1); 
  value = analogRead(muestreo); // Read analog value from sensor
  ppm = ppm + value; // Accumulate analog readings
  delayMicroseconds(r2); 
  digitalWrite(IRED, HIGH); // Turn on Infrared LED
  delayMicroseconds(r3); 
  
  voltage = ppm / i * 0.0049; // Convert analog readings to voltage
  density_polvo = 0.17 * voltage - 0.1; // Calculate dust density
  ppm_real = (voltage - 0.0356) * 120000; // Calculate real ppm value
  
  // Ensure ppm_real and density_polvo values are within valid ranges
  if(ppm_real < 0)
    ppm_real = 0;
  if(density_polvo < 0)
    density_polvo = 0;
  if(density_polvo > 0.5)
    density_polvo = 0.5;

  // Display dust density on the LCD
  lcd.clear(); // Clear the LCD display
  lcd.setCursor(0, 0); // Set cursor to the beginning of the first line
  lcd.print("-> Density "); // Print message on the first line
  lcd.setCursor(0, 1); // Set cursor to the beginning of the second line
  lcd.print(density_polvo, 3); // Print dust density value with 3 decimal places
  lcd.print(" mg x m3"); // Print unit
  delay(2000); // Delay for 2 seconds
  delay(1000); // Delay for 1 second
}
