
#ifndef NIGHTLIGHT_H
#define NIGHTLIGHT_H

#ifndef byte
#define byte uint8_t
#endif

typedef struct {
  byte r;
  byte g;
  byte b;
} RGB;

typedef struct {
  float h; //hue angle, 0-1 (portion of full circle)
  float s; //saturation value, 0-1
  float i; //intensity value, 0-1
} HSI;


#endif