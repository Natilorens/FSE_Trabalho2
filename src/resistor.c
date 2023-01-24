/*

#include <stdio.h>
#include <unistd.h>      
#include <string.h>
#include <wiringPi.h> 
#include <stdlib.h>
//#include <Wire.h>



void LigaResistor(){

 const int PWM_pin = 4;
 int intensity ;            

  if (wiringPiSetupGpio () == -1)
    exit (1) ;

  pinMode (PWM_pin, PWM_OUTPUT) ; // set PWM pin as output

  while (1)
  {
	
    for (intensity = 0 ; intensity < 100 ; ++intensity)
    {
      pwmWrite (PWM_pin, intensity) ;	// provide PWM value for duty cycle
      delay (1) ;
    }
    delay(1);

    for (intensity = 100 ; intensity >= 0 ; --intensity)
    {
      pwmWrite (PWM_pin, intensity) ;
      delay (1) ;
    }
    delay(1);
  }
}

int main (void){

    LigaResistor();



    return 0;
}
*/