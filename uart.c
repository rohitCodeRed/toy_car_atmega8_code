#include "uart.h"
#include <avr/io.h>



#define BAUDRATE 9600
#define CLK_SPEED 8000000  // 8 Mhz
#define UBBR_VAL CLK_SPEED/16/BAUDRATE-1


void init_uart(){
    /*Set input mode for receiver pin and output mode for transmitter pin*/
    DDRD |= (1U<<1);  // transmitter
    DDRD &= ~(1U<<0);  //Receiver
    
    //Set output low for both pin
    PORTD &= ~(1U<<1);
    PORTD &= ~(1U<<0);
    
    //Reset uart control and status register A
    UCSRA = 0x00;
    
    /*Enable transmit and receiver complete interrupt*/
    UCSRB |= (1U<<RXEN) | (1U<<RXCIE) | (1U<<TXEN);
    UCSRB &= ~(1U<<TXCIE); //disable the trasmitter complete interrupt
    
    
    /*Set baudrate UBBR register*/
    unsigned int ubrr = UBBR_VAL;
    UBRRH = (unsigned char)(ubrr>>8);  //right shift by 8 bit
    UBRRL = (unsigned char)ubrr;
    
    /*Set data format, 8 data bit and 1 stop bit*/
    UCSRC = (1<<URSEL) | (3<<UCSZ0);
    
    
}


/*Send message function and for receive data we use interrupt function in main file*/
void send(unsigned char a){

    /* Wait for empty transmit buffer */
    while ( !( UCSRA & (1<<UDRE)) );
    
    /* Put data into buffer, sends the data */
    UDR = a;
}


/*Send whole message in string*/
void sendMessage(char *m,unsigned char len){

    unsigned char i = 0;
    if(m != NULL){
        while((*(m+i) != '\0') && (i < len)){
        
            send((unsigned char) *(m+i));
            
            i++;
        }
    }
}


/*format message through sprintf*/
void formatMessage(char *s,unsigned int d1, unsigned int d3, bool flag,unsigned char d){
    
    if(flag == true){
        //s character mean start and e character mean end
        sprintf(s,"s%d:%d:t:%ce",d1,d3,d);
    }else{
        sprintf(s,"s%d:%d:f:%ce",d1,d3,d);
    }
    
}






