#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

Adafruit_BMP085_Unified bmp;

const int buzzerPin = 9; // Buzzer connected to digital pin 9
const float pressureThreshold = 1010.8; // Example threshold in hPa (adjust as needed)

void setup() {
  Serial.begin(9600);
  
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1);
  }

  pinMode(buzzerPin, OUTPUT);
  Serial.println("BMP180 sensor initialized!");
}

void loop() {
  // Read the pressure in hPa
  sensors_event_t event;
  bmp.getEvent(&event); // Get the latest sensor data

  if (event.pressure) {
    float pressure = event.pressure; // hPa
    Serial.print("Pressure: ");
   Serial.print(pressure);
    Serial.println(" hPa");

    // Trigger the buzzer if the pressure exceeds the threshold
    if (pressure > pressureThreshold) {
      digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
    } else {
      digitalWrite(buzzerPin, LOW); // Turn off the buzzer
    }
  }

  delay(1000); // Read every second
}
