
#define SAMPLE_SIZE 256
int sample_array[SAMPLE_SIZE];

#define ROLLING_AVG_SIZE 10
int rolling_array[ROLLING_AVG_SIZE];
int rolling_index = 0;
float rolling_average = 0;

// m5 LED
#include "M5Atom.h"

// couleur du led
#define WHITE 0xff, 0xff, 0xff
#define RED 0xff, 0x00, 0x00
#define YELLOW 0xff, 0xff, 0x00
#define GREEN 0x00, 0xff, 0x00
#define CYAN 0x00, 0xff, 0xff
#define BLUE 0x00, 0x00, 0xff
#define PINK 0xff, 0x00, 0xff
#define OFF 0x00, 0x00, 0x00



uint8_t DisBuff[2 + 5 * 5 * 3];

void set_m5_led(uint8_t Rdata, uint8_t Gdata, uint8_t Bdata)
{
  DisBuff[0] = 0x05;
  DisBuff[1] = 0x05;
  for (int i = 0; i < 25; i++) {
    DisBuff[2 + i * 3 + 1] = Rdata;  // -> fix : grb? weird?
    DisBuff[2 + i * 3 + 0] = Gdata;  //->  fix : grb? weird?
    DisBuff[2 + i * 3 + 2] = Bdata;
  }
  M5.dis.displaybuff(DisBuff);
}

void setup()
{
  M5.begin(true, false, true);
  set_m5_led(BLUE);

  //set the resolution to 12 bits (0-4096)
  analogReadResolution(12);
}

void loop()
{
  // read the analog value for pin 32:
  for (int i = 0; i < SAMPLE_SIZE; i++)
  {
    sample_array[i] = analogRead(32);
  }

  int ADC_AVG_BOOL = (int(average(sample_array, SAMPLE_SIZE))) > 3000;
  rolling_index++;
  if (rolling_index > ROLLING_AVG_SIZE) {
    rolling_index = 0;
  }
  rolling_array[rolling_index] = ADC_AVG_BOOL;
  rolling_average = average(rolling_array, ROLLING_AVG_SIZE);



  delay(10);  // delay in between reads for clear read from serial
  Serial.printf("proximity %f\n", rolling_average);


  if (rolling_average == 0)
  {
    set_m5_led(WHITE);
  } else if (rolling_average < 0.2) {
    set_m5_led(GREEN);
  } else if (rolling_average < 0.5) {
    set_m5_led(YELLOW);
  } else if (rolling_average < 0.8) {
    set_m5_led(RED);
  }

}



float average (int * array, int len)  // assuming array is int.
{
  long sum = 0L ;  // sum will be larger than an item, long for safety.
  for (int i = 0 ; i < len ; i++)
    sum += array [i] ;
  return  ((float) sum) / len ;  // average will be fractional, so float may be appropriate.
}
