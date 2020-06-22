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

#define DHTPIN1 2     // Digital pin connected to the DHT sensor
#define DHTPIN2 7     // Digital pin connected to the DHT sensor
#define DHTPIN3 4     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

DHT dht[] = {
  {DHTPIN1, DHTTYPE},
  {DHTPIN2, DHTTYPE},
  {DHTPIN3, DHTTYPE},
};

float h[3];
float t[3];

void setup() {
  Serial.begin(9600);
  for (auto& sensor : dht) {
    sensor.begin();
  }
}

void loop() {
  // Wait at least 2 seconds for DHT11
  delay(2000);

  for (int i = 0; i < 3; i++) {
    t[i] = dht[i].readTemperature();
    h[i] = dht[i].readHumidity();
  }

  for (int i = 0; i < 3; i++) {
  Serial.print(h[i]);
  Serial.print(F("\t"));
  Serial.print(t[i]);
  Serial.print(F("\t"));
  }
  Serial.print("\n");
}
