
 
#define REDPIN 10
#define GREENPIN 11
#define BLUEPIN 9
 
#define FADESPEED 5     // make this higher to slow down
 
float r, g, b;
 
float rtarget, gtarget, btarget;

float rdelta, gdelta, bdelta;

int colormax = 255;
 
void setup() {
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  
  Serial.begin(9600);
  
  randomSeed(analogRead(0));
  

//  for (int c=0; c<=100; c++) {
//    Serial.write("Random number: ");
//    Serial.print(random(256));
//    Serial.write("\n");
//  }
  
  r=0;
  g=0;
  b=0;
  
  analogWrite(REDPIN, (int)r);
  analogWrite(GREENPIN, (int)g);
  analogWrite(BLUEPIN, (int)b);
  
}



void loop() {
  

   rtarget=random(colormax);
   gtarget=random(colormax);
   btarget=random(colormax);
   
   rdelta = rtarget-r;
   gdelta = gtarget-g;
   bdelta = btarget-b;

   float ardelta = abs(rdelta);
   float agdelta = abs(gdelta);
   float abdelta = abs(bdelta);

   //get max of ardelta, agdelta, abdelta
   int steps = (int)max(max(ardelta, agdelta), abdelta);
         
   Serial.write("r value: ");
   Serial.print(r);
   Serial.write("; r target: ");
   Serial.print(rtarget);
   Serial.write("; r delta: ");
   Serial.print(rdelta);
   Serial.write("\n");
   
   Serial.write("g value: ");
   Serial.print(g);
   Serial.write("; g target: ");
   Serial.print(gtarget);
   Serial.write("; g delta: ");
   Serial.print(gdelta);
   Serial.write("\n");
  
   Serial.write("b value: ");
   Serial.print(b);
   Serial.write("; b target: ");
   Serial.print(btarget);
   Serial.write("; b delta: ");
   Serial.print(bdelta);
   Serial.write("\n");


    for (int stepcount=0; stepcount <= steps; stepcount++) {
      r += rdelta/steps;
      g += gdelta/steps;
      b += bdelta/steps;
      
      Serial.write("Stepcount: ");
      Serial.print(stepcount);
      Serial.write("; Steps: ");
      Serial.print(steps);
      Serial.write("\n");
      
      Serial.write("Target (r,g,b): (");
      Serial.print((int)r);
      Serial.write(", ");
      Serial.print((int)g);
      Serial.write(", ");
      Serial.print((int)b);
      Serial.write(")\n");
      
      analogWrite(REDPIN, (int)r);
      analogWrite(GREENPIN, (int)g);
      analogWrite(BLUEPIN, (int)b);
      
      delay(FADESPEED*10);
       
   }
   



//  // fade from blue to violet
//  for (r = 0; r < 256; r++) { 
//    analogWrite(REDPIN, r);
//    delay(FADESPEED);
//  } 
//  // fade from violet to red
//  for (b = 255; b > 0; b--) { 
//    analogWrite(BLUEPIN, b);
//    delay(FADESPEED);
//  } 
//  // fade from red to yellow
//  for (g = 0; g < 256; g++) { 
//    analogWrite(GREENPIN, g);
//    delay(FADESPEED);
//  } 
//  // fade from yellow to green
//  for (r = 255; r > 0; r--) { 
//    analogWrite(REDPIN, r);
//    delay(FADESPEED);
//  } 
//  // fade from green to teal
//  for (b = 0; b < 256; b++) { 
//    analogWrite(BLUEPIN, b);
//    delay(FADESPEED);
//  } 
//  // fade from teal to blue
//  for (g = 255; g > 0; g--) { 
//    analogWrite(GREENPIN, g);
//    delay(FADESPEED);
//  } 
}
