#include "timer.h"
#include <avr/io.h>



void init_timer(){
    /*Enable timer 0 with  prescaler 1024*/
    TCCR0 |= (1U<<CS02); 
    TCCR0 |= (1U<<CS00);
    TCCR0 &= ~(1U<<CS01);
    
    /*Initial value of timer(8 bit) counter 0*/
    TCNT0 = 0;  //0-255
    
    /*Enable interrupt masking*/
    TIMSK |= (1U<<TOIE0);
    
}

/*Disable and enable timer0 functions*/
void disableTimer(){
    //disable timer
    TCCR0 &= ~(1U<<CS02); 
    TCCR0 &= ~(1U<<CS00);
    TCCR0 &= ~(1U<<CS01);
    
}
void enableTimer(){
    //enable timer
    TCCR0 |= (1U<<CS02); 
    TCCR0 |= (1U<<CS00);
    TCCR0 &= ~(1U<<CS01);
}


