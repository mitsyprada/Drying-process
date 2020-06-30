int ThermistorPin = 0;
int Vo;
float R1 = 100000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
int relay = 2;
volatile byte relayState = LOW;
int state = LOW;   
void setup() {
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  Serial.begin(9600);
}

void loop() {
  Vo = analogRead(ThermistorPin);
  logR2 = log(R1 * ((1024.0 / (float)Vo) - 1.0));
  T = 1.0 / (c1 + (c2*logR2) + (c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  Serial.print("Temperature: "); 
  Serial.print(Tc);
  Serial.println(" C");   
  delay(500);

  if (Tc <= 80) {          // check if the sensor is HIGH   
    if(relayState == LOW){
      digitalWrite(relay, HIGH);
    }
    relayState = HIGH;  
    Serial.println("ON");
  } 
  if (Tc >= 90) {  
    digitalWrite(relay, LOW);
    relayState = LOW;
    Serial.println("OFF");
    }
}
