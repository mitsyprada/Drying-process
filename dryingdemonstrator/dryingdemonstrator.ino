//DHT-sensors
#include "DHT.h"
#define DHTPIN1 2     // Digital pin connected to the DHT incoming air
#define DHTPIN2 3     // Digital pin connected to the DHT outcoming air
#define DHTPIN3 4     // Digital pin connected to the DHT inside box air
#define DHTTYPE DHT11   // DHT 11
DHT dht[] = {
  {DHTPIN1, DHTTYPE},
  {DHTPIN2, DHTTYPE},
  {DHTPIN3, DHTTYPE},
};
float h[3];
float t[3];

//Windsensor Rev C (Modern Device)
/* Pin-out wires wind sensor:
 *      Red: 5V
 *    Black: GNV
 *     Blue: TMP
 *    Green: RV
 * Not used: OUT
 */
#define analogPinForRV    3   // change to pins you the analog pins are using
#define analogPinForTMP   4
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
int ThermistorPin = A1;
int Vo;
float R1 = 100000;
float logR2, T, Tc;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

// Variables of the heat bed temperature sensor
int tempheatbed_pin = A2;
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
int soilmoisturepercent = 0;
const float a = -0.171;
const float b = 183.44;
float soilMoistureWeight = 0;

void setup() {
  // setup pin to control the heat bed
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, LOW);
  relayState = LOW;
  
  Serial.begin(9600);
  
  for (auto& sensor : dht) {
    sensor.begin();
  }

}

void loop() {
  
  if (millis() - lastMillis > 2000){ // read every 2000 ms
    
    //Wind sensor
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
    soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
    soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
    soilMoistureWeight = a * soilMoistureValue + b;

    // Output to Serial Monitor
    // Sequence:
    // f1-1,t3-2, h4-3, af-4, t1-5, t2-6, h1-7, f2-8, t4-9 , wm-10,  t5-11,  h3-12,  h2-13 

        //f1-1
    Serial.print("f1:1.00/n");  //ON or OFF

        //f2-8
    Serial.print("f2:1.00"); //ON or OFF
    Serial.print("/n");

        //t1-5
    Serial.print("t1:");
    Serial.print(t[0], 2); //t1 Celsius
    Serial.print("/n");

        //h1-7
    Serial.print("h1:");
    Serial.print(h[0], 2); //h1 relative
    Serial.print("/n");

        //t2-6
    Serial.print("t2:");
    Serial.print(t[1], 2); //t2 Celsius
    Serial.print("/n");

        //h2-13
    Serial.print("h2:");
    Serial.print(h[1], 2); //humidity outcoming air relative
    Serial.print("/n");
    
        //t3-2
    Serial.print("t3:");
    Serial.print(t[2], 2); //t3 Celsius
    Serial.print("/n");

        //h4-3
    Serial.print("h4:");
    Serial.print(h[2], 2); //h4 relative humidity
    Serial.print("/n");
    
        //af-4
    Serial.print("af:");
    Serial.print(WindSpeed_mps, 2); // meters per second
    Serial.print("/n");
    
    //t4-9
    Serial.print("t4:");
    Serial.print(Tc_bed, 2); //Temperature of the heat bed Celsius
    Serial.print("/n");
    
    //wm-10
    Serial.print("wm:"); //watts
    if (relayState == LOW) {
      Serial.print("0.00");
    } else {
      Serial.print("120.00");
    }
    Serial.print("/n");

    //t5-11
    Serial.print("t5:");
    Serial.print(Tc, 2); //Temperature of the towel Celsius
    Serial.print("/n");

    //h3-12
    Serial.print("h3:");
    Serial.print(soilMoistureWeight, 2); //Water content of the towel in grams
    Serial.print("/n");



    /*
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
    Serial.print(soilMoistureValue);
    Serial.print("\n");
    */
    
    // Code to control the temperature in the range
    if (Tc_bed <= 80) {          // check if the sensor is HIGH   
      if(relayState == LOW){
        digitalWrite(relay_pin, HIGH);
        }
      relayState = HIGH;  
      //Serial.println("ON");
    } 
    if (Tc_bed >= 90) {  
      digitalWrite(relay_pin, LOW);
      relayState = LOW;
      //Serial.println("OFF");
    }
    lastMillis = millis();
  } 
}
