/*
 * File:   toy_car_main.c
 * Author: rohit
 *
 * Created on 25 October, 2024, 7:09 PM
 */

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "pwm.h"
#include "timer.h"
#include "uart.h"


#define MAX_TIMER_OVERLOAD_COUNT 30

static unsigned char MOTOR_CODE ='x';
static uint8_t COUNTER = 0;
unsigned int MOTOR_1_DUTY = 0;
unsigned int MOTOR_2_DUTY = 0;
bool OBSTACLE_FLAG = false;

static unsigned char DIRECTION='o'; //forward direction 'o' (onward)
static char MESSAGE[25] = {'\0'};



/*Update status function*/
void updateStatus(){
    MOTOR_1_DUTY = ((getOCR1A() * 100) / MAX_SPEED);
    MOTOR_2_DUTY = ((getOCR1B() * 100) / MAX_SPEED);

//    if(OBSTACLE_FLAG == true){
//        if((PIND & (1U<<2)) == 0){
//            OBSTACLE_FLAG = true;
//        }else{
//            OBSTACLE_FLAG = false;
//        }  
//    }

    if(isPolarityChanged()){
        DIRECTION ='r'; //reverse direction
    }else{
        DIRECTION = 'o';
    }
        
}

void sendUpdatedMessage(){
    formatMessage(MESSAGE,MOTOR_1_DUTY,MOTOR_2_DUTY,DIRECTION);
    sendMessage(MESSAGE,strlen(MESSAGE));
}


int main(void) {
    
    init_pwm();
    init_uart();
    init_timer();
    
    //enable global interrupt
    sei();
    
    while (1) {
        //Check counter value after overload
        if(COUNTER >= MAX_TIMER_OVERLOAD_COUNT){
            COUNTER = 0;
            
            disableTimer();
            
            sendUpdatedMessage();
            
            enableTimer();
        
         }
         
         updateStatus();
         
    }
}


ISR(TIMER0_OVF_vect){
    /*Re initialize timer counter*/
    if(COUNTER > MAX_TIMER_OVERLOAD_COUNT){
        COUNTER = MAX_TIMER_OVERLOAD_COUNT;
    }else{
        COUNTER += 1;
    }
    
}



ISR(USART_RXC_vect){
    unsigned char a = UDR;
    MOTOR_CODE = a;
    
    switch(MOTOR_CODE){
            case 'b':
                stop();
                break;
            case 's':
                start();
                break;
            case 'l':
                turnLeft();
                break;
            case 'r':
                turnRight();
                break;
            case 'a':
                cruiseForward();
                break;
            case 'd':
                cruiseBackward();
                break;
        }
    
}
