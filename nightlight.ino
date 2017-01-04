
int R1input = A0;
int R2input = A1;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  int sensorValue = analogRead(R1input);
  Serial.println(sensorValue);
  sensorValue = analogRead(R2input);
  Serial.println(sensorValue);
  
  delay(250);        // delay in between reads for stability
}
