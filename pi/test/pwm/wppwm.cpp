

// by Lewis Loflin www.bristolwatch.com lewis@bvu.net
// URL: http://www.bristolwatch.com/rpi/pwmRpi.htm

#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
// only P1 and P26 work for PWM0
// at the same time!

int main (void)   {
  int num ;
  printf ("Raspberry Pi wiringPi PWM test program\n") ;
  if (wiringPiSetup () == -1)
    exit (1) ;

  // set pin 1 to PWM
  pinMode (1, PWM_OUTPUT) ;

  int dT = 5;
  for (;;)   {

    for (num = 0 ; num < 1024 ; ++num)
    {
      pwmWrite (1, num) ;
      delay (dT) ;
    }
    delay(1000);
    for (num = 1023 ; num >= 0 ; --num)
    {
      pwmWrite (1, num) ;
      delay (dT) ;
    }

    delay(1000);
  } // end for

  return 0 ;
}

