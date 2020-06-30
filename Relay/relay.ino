int tempheatbed_pin = 6;
int Vo_bed;
float logR2_bed, T_bed, Tc_bed;
int relay_pin = 7;
volatile byte relayState = LOW;

float R1 = 100000;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
void setup() {
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, LOW);
  Serial.begin(9600);
}

void loop() {
  Vo_bed = analogRead(tempheatbed_pin);
  logR2_bed = log(R1 * ((1024.0 / (float)Vo_bed) - 1.0));
  T_bed = 1.0 / (c1 + (c2*logR2_bed) + (c3*logR2_bed*logR2_bed*logR2_bed));
  Tc_bed = T_bed - 273.15;
  Serial.print("Temperature: "); 
  Serial.print(Tc_bed);
  Serial.println(" C");   
  delay(500);

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
}
