/**
 * 
 * Description:
 *  Connect the DHT-11 to the Arduino as per the
 *  hardware connections below. Run the sketch and open a serial
 *  monitor. ...
 * 
 * Hardware Connections:
 *  Arduino | DHT11
 *  -------------------
 *    5V    |   Middle pin    
 *    2     |   S        
 *    GND   |   -
 *  
 * REQUIRES the following Arduino libraries:
 * - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
 * - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
 * 
 */

#include "DHT.h"

#define DHTPIN 0     // Digital pin connected to the DHT sensor
#define DHTPIN2 5     // Digital pin connected to the DHT sensor
#define DHTPIN3 4     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE); // multiple DHT-sensors possible
DHT dht2(DHTPIN2, DHTTYPE); // multiple DHT-sensors possible
DHT dht3(DHTPIN3, DHTTYPE); // multiple DHT-sensors possible

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // Wait at least 2 seconds for DHT11
  delay(2000);

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default, insert 'true' between brackets for Fahrenheit)
  float t = dht.readTemperature();

  float h2 = dht2.readHumidity();
  // Read temperature as Celsius (the default, insert 'true' between brackets for Fahrenheit)
  float t2 = dht2.readTemperature();

  float h3 = dht3.readHumidity();
  // Read temperature as Celsius (the default, insert 'true' between brackets for Fahrenheit)
  float t3 = dht3.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(h);
  Serial.print(F("\t"));
  Serial.print(t);
  Serial.print(F("\t"));
  Serial.print(h2);
  Serial.print(F("\t"));
  Serial.print(t2);  
  Serial.print(F("\t"));
  Serial.print(h3);
  Serial.print(F("\t"));
  Serial.println(t3);
}
