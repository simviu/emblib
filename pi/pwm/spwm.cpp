#include <wiringPi.h>
#include <softPwm.h>

int main(void)
{
	const int pin = 1; // wiringPi pin 1 is GPIO18

	wiringPiSetup() ;
	// unit is 0.1ms, so create PWM cyle
	// of 100x0.1ms = 10ms
	//  ( PWM range 1.5ms +- 0.5 ms )
	softPwmCreate(pin,0,200) ; 
	while(1)
	{
	    for(int i=0;i<20;i++)
	    {
		int k = 5+i;
	        softPwmWrite(pin, k) ;
	        delay(500) ;
	    }
	    delay(1000);
	}

	return 0;
}
