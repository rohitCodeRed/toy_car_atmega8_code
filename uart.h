/* 
 * File:   uart.h
 * Author: rohit
 *
 * Created on 25 October, 2024, 7:13 PM
 */

#ifndef UART_H
#define	UART_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef	__cplusplus
extern "C" {
#endif

    void init_uart(void);
    void send(unsigned char a);
    void formatMessage(char *s,unsigned int d1, unsigned int d3, bool flag,unsigned char d);
    void sendMessage(char *m,uint8_t len);
    


#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

