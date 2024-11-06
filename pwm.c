#include "pwm.h"
#include <avr/io.h>



/*Define two static value for duty cycle time period of each motor*/
static int D1=0;
static int D2=0;

void init_motor_second_pins(){  //used to reverse the motor direction
    /*set DDRD of 4 and 5 pin as output*/
    DDRD |= (1U<<4);
    DDRD |= (1U<<5);
    
    
}


/*Set inverting and non inverting mode*/
void setInvertingMode(){
    /*Set mode A channel*/
    TCCR1A |= (1U<<COM1A0);
    TCCR1A |= (1U<<COM1A1);
    
    /*Set mode B channel*/
    TCCR1A |= (1U<<COM1B0);
    TCCR1A |= (1U<<COM1B1);
}

void setNormalMode(){
    /*Clear mode A channel*/
    TCCR1A &= ~(1U<<COM1A0);
    TCCR1A |= (1U<<COM1A1);
    
    /*Clear mode B channel*/
    TCCR1A &= ~(1U<<COM1B0);
    TCCR1A |= (1U<<COM1B1);
}


/*Change polarity of motor functions*/
void backwardMotorPolarity(){
    PORTD |= (1U<<4);
    PORTD |= (1U<<5);
}

void forwardMotorPolarity(){
    PORTD &= ~(1U<<4);
    PORTD &= ~(1U<<5);
}


/*Function to check polarity*/
bool isPolarityChanged(){
    
    if((PIND & (1U<<4)) && (PIND & (1U<<5))){
        return true;
    }
    return false;
}


/*Function to move forward and backward*/
void setReverse(){
    setInvertingMode();
    backwardMotorPolarity();
}

void setForward(){
    setNormalMode();
    forwardMotorPolarity();
}


/*Return the value of duty cycle timer*/
unsigned int getOCR1A(){
    return D1;
}

unsigned int getOCR1B(){
    return D2;
}





void init_pwm(){
    /*Set PB1 and PB2 pins as output*/
    DDRB |= (1U<<1);
    DDRB |= (1U<<2);
    
    /*Init with low value for both pin*/
    PORTB &= ~(1U<<1);
    PORTB &= ~(1U<<2);
    
    init_motor_second_pins();
    forwardMotorPolarity();
    
    /*Pre scale set to 64*/
    TCCR1B |= (1U<<CS10);
    TCCR1B |= (1U<<CS11);
    TCCR1B &= ~(1U<<CS12);
    
    setNormalMode();
    
    /*Set PWM phase mode with 8 bit value */
    TCCR1B &= ~(1U<<WGM13);
    TCCR1B &= ~(1U<<WGM12);
    TCCR1A &= ~(1U<<WGM11);
    TCCR1A |= (1U<<WGM10);
    
    /*Init output capture Counter value for both A and B channel*/
    OCR1A = 0;
    OCR1B = 0;
    
    
    
}






/*Stop motor function*/
void stop(){
    //Set the forward mode to motors
    setForward();
    
    //Set output capture register to 0 and also set 0 to D1 and D2
    OCR1A = 0;
    OCR1B = 0;
    D1= 0;
    D2= 0;
    
}


/*Start the motor */
void start(){
    OCR1A = START_SPEED;
    D1= START_SPEED;
    OCR1B = START_SPEED;
    D2 = START_SPEED;
}

/*Set max speed*/
void setMaxSpeed(){
    OCR1A = MAX_SPEED;
    OCR1B = MAX_SPEED;
    D1 = MAX_SPEED;
    D2 = MAX_SPEED;
}


/*increase speed and decrease speed functions*/
void increaseSpeed(){
    if(D1 >= D2){
         D1 += SPEED_LEVEL;
         D2 = D1;
         
         if(D1 < MAX_SPEED){
            OCR1A = D1;
            OCR1B = D1;
         }else{
            setMaxSpeed();
         }
         
    }
    else{
        D2 += SPEED_LEVEL;
        D1 = D2;
         
        if(D2 < MAX_SPEED){
            OCR1A = D2;
            OCR1B = D2;
        }else{
            setMaxSpeed();
        }
         
    }
    
}

void decreaseSpeed(){
    if(D1 >= D2){
         D1 -= SPEED_LEVEL;
         D2 = D1;
         
         if(D1 > START_SPEED){
            OCR1A = D1;
            OCR1B = D1;
         }else{
            stop();
         }
         
    }
    else{
        D2 -= SPEED_LEVEL;
        D1 = D2;
         
        if(D2 > START_SPEED){
            OCR1A = D2;
            OCR1B = D2;
        }else{
            stop();
        }
         
    }
}



/*Turn left and right function*/
void turnLeft(){
    
    if(D1 == 0 && D2 == 0){
        setForward();
        start();
    }
    
    D1 -= SPEED_LEVEL;
    if(D1 > START_SPEED){
         OCR1A = D1;
    }else{
       OCR1A = 0;
       D1 = 0;
    }
    
    if(D1 == 0 && D2 == 0){
        setForward();
    }
}

void turnRight(){
    if(D1 == 0 && D2 == 0){
        setForward();
        start();
    }
    
    D2 -= SPEED_LEVEL;
    if(D2 > START_SPEED){
         OCR1B = D2;
    }else{
       OCR1B = 0;
       D2 = 0;
    }
    
    if(D1 == 0 && D2 == 0){
        setForward();
    }
   
}


/*Acceleration and deacceleration functions*/
void cruiseBackward(){
    if(D1 == 0 && D2 == 0){
        setReverse();
        start();
        
    }else{
        if(isPolarityChanged()){
            increaseSpeed();
        }else{
            decreaseSpeed();
        }
        
    }
}

void cruiseForward(){
    if(D1 == 0 && D2 == 0){
         setForward();
        start();
        
    }else{
        if(isPolarityChanged()){
            decreaseSpeed();
        }else{
            increaseSpeed();
        }
    }
}







