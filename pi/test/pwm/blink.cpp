#include <wiringPi.h>
int main (void)
{
  int T = 1000;
  int pin = 2; // wiringPi's gpio2 is on pin 13

  wiringPiSetup () ;
  pinMode (pin, OUTPUT) ;
  for (;;)
  {
    digitalWrite (pin, HIGH) ; delay (T) ;
    digitalWrite (pin,  LOW) ; delay (T) ;
  }
  return 0 ;
}

