/* 
 * File:   pwm.h
 * Author: rohit
 *
 * Created on 25 October, 2024, 7:12 PM
 */

#ifndef PWM_H
#define	PWM_H
#include <stdbool.h>

#define FREQUENCY 32000
#define SPEED_LEVEL 30
#define MAX_SPEED 255
#define START_SPEED 75

#ifdef	__cplusplus
extern "C" {
#endif

    void init_pwm(void);
    unsigned int getOCR1A();
    unsigned int getOCR1B();
    
    void cruiseForward();
    void cruiseBackward();
    void start();
    void stop();
    bool isPolarityChanged();
    void turnLeft();
    void turnRight();



#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

