/* Pin-out wires:
 *      Red: 5V
 *    Black: GNV
 *     Blue: TMP
 *    Green: RV
 * Not used: OUT
 */


/* Modern Device Wind Sensor Sketch for Rev C Wind Sensor
 This sketch is only valid if the wind sensor if powered from 
 a regulated 5 volt supply. An Arduino or Modern Device BBB, RBBB
 powered from an external power supply should work fine. Powering from
 USB will also work but will be slightly less accurate in our experience.
 
When using an Arduino to power the sensor, an external power supply is better. Most Arduinos have a 
 polyfuse which protects the USB line. This fuse has enough resistance to reduce the voltage
 available to around 4.7 to 4.85 volts, depending on the current draw. 
 
 The sketch uses the on-chip temperature sensing thermistor to compensate the sensor
 for changes in ambient temperature. Because the thermistor is just configured as a
 voltage divider, the voltage will change with supply voltage. This is why the 
 sketch depends upon a regulated five volt supply.
 
 Other calibrations could be developed for different sensor supply voltages, but would require
 gathering data for those alternate voltages, or compensating the ratio.
 
 Hardware Setup: 
 Wind Sensor Signals    Arduino
 GND                    GND   // Black wire
 +V                     5V    // Red wire
 RV                     A1    // Green wire;  modify the definitions below to use other pins
 TMP                    A0    // Blue wire;   modify the definitions below to use other pins
 OUT                    N/A   // Yellow wire
 

 Paul Badger 2014
 
 Hardware setup:
 Wind Sensor is powered from a regulated five volt source.
 RV pin and TMP pin are connected to analog innputs.
 
 */

//DHT-sensors

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

//Windsensor Rev C (Modern Device)

#define analogPinForRV    1   // change to pins you the analog pins are using
#define analogPinForTMP   0

// to calibrate your sensor, put a glass over it, but the sensor should not be
// touching the desktop surface however.
// adjust the zeroWindAdjustment until your sensor reads about zero with the glass over it. 

const float zeroWindAdjustment =  0.31; // negative numbers yield smaller wind speeds and vice versa.
const float board_voltage = 4.94; // measure the board voltage and write here

float TMP_Therm_ADunits;  //temp termistor value from wind sensor
float RV_Wind_ADunits;    //RV output from wind sensor 
float RV_Wind_Volts;
unsigned long lastMillis;
int TempCtimes100;
float zeroWind_ADunits;
float zeroWind_volts;
float WindSpeed_mps;

// Touch temperature sensor on the towel
int ThermistorPin = 2;
int Vo;
float R1 = 100000;
float logR2, T, Tc;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

// Variables of the heat bed temperature sensor
int tempheatbed_pin = 6;
int Vo_bed;
float logR2_bed, T_bed, Tc_bed;

// Variables to control the relay
int relay_pin = 7;
volatile byte relayState = LOW;

// Soil moisture sensor
const int AirValue = 510;   //you need to replace this value with Value_1
const int WaterValue = 251;  //you need to replace this value with Value_2
int intervals = (AirValue - WaterValue)/3;
int soilMoistureValue = 0;
int soilmoisturepercent=0;


void setup() {
  // setup pin to control the heat bed
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, LOW);
  
  Serial.begin(9600);
  
  for (auto& sensor : dht) {
    sensor.begin();
  }

}

void loop() {
  //Wind sensor
  
  if (millis() - lastMillis > 2000){ // read every 2000 ms
    
    TMP_Therm_ADunits = analogRead(analogPinForTMP);
    RV_Wind_ADunits = analogRead(analogPinForRV);
    RV_Wind_Volts = (RV_Wind_ADunits * board_voltage / 1023.0);
    TempCtimes100 = (0.005 *(TMP_Therm_ADunits * TMP_Therm_ADunits)) - (16.862 * TMP_Therm_ADunits) + 9075.4;
    zeroWind_ADunits = -0.0006*(TMP_Therm_ADunits * TMP_Therm_ADunits) + 1.0727 * TMP_Therm_ADunits + 47.172;  //  13.0C  553  482.39
    zeroWind_volts = (zeroWind_ADunits * board_voltage / 1023.0) - zeroWindAdjustment;  
    WindSpeed_mps = (1.609344 * pow(((RV_Wind_Volts - zeroWind_volts) /.2300) , 2.7265))/3.6;   

    // DHT-sensors
    for (int i = 0; i < 3; i++) {
      t[i] = dht[i].readTemperature();
      h[i] = dht[i].readHumidity();
    }

    //Touch temp towel
    Vo = analogRead(ThermistorPin);
    logR2 = log(R1 * ((1024.0 / (float)Vo) - 1.0));
    T = 1.0 / (c1 + (c2*logR2) + (c3*logR2*logR2*logR2));
    Tc = T - 273.15;
    //Tf = (Tc * 9.0)/ 5.0 + 32.0; 

    // Heat bed temperature sensor
    Vo_bed = analogRead(tempheatbed_pin);
    logR2_bed = log(R1 * ((1024.0 / (float)Vo_bed) - 1.0));
    T_bed = 1.0 / (c1 + (c2*logR2_bed) + (c3*logR2_bed*logR2_bed*logR2_bed));
    Tc_bed = T_bed - 273.15;
    
    // Soil moisture sensor
    soilMoistureValue = analogRead(A3);  //put Sensor insert into soil
    soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);

    // Output to Serial Monitor
    for (int i = 0; i < 3; i++) {
      Serial.print(h[i]);
      Serial.print(F("\t"));
      Serial.print(t[i]);
      Serial.print(F("\t"));
    }
    
    Serial.print(WindSpeed_mps);
    Serial.print(F("\t"));
    
    Serial.print(Tc);
    Serial.print(F("\t"));

    Serial.print(Tc_bed);
    Serial.print(F("\t"));

    Serial.print(soilmoisturepercent);

    Serial.print("\n");

    // Code to control the temperature in the range
    if (Tc_bed <= 80) {          // check if the sensor is HIGH   
      if(relayState == LOW){
        digitalWrite(relay_pin, HIGH);
        }
      relayState = HIGH;  
      Serial.println("ON");
    } 
    if (Tc_bed >= 90) {  
      digitalWrite(relay_pin, LOW);
      relayState = LOW;
      Serial.println("OFF");
    }
    lastMillis = millis();

    } 
}
