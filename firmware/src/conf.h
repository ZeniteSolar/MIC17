/**
 * @file conf.h
 *
 * @defgroup CONF Configurations 
 *
 * @brief General configuration of the system.
 *
 */

#ifndef _CONF_H_
#define _CONF_H_

// CONFIGURACOES DE COMPILACAO
//#define DEBUG_ON
//#define VERBOSE_ON

// MODULES ACTIVATION
#define ADC_ON
#define USART_ON
#define PWM_ON
#define CTRL_ON
#define MACHINE_ON                  // this one is absolutly necessary
#define SLEEP_ON
#define WATCHDOG_ON
#define CAN_ON

// INPUT PINS DEFINITIONS
#define     SWITCHES_PORT           PORTD
#define     SWITCHES_PIN            PIND
#define     SWITCHES_DDR            DDRD
#define     PUMP1_ON_SWITCH         PD2
#define     PUMP2_ON_SWITCH         PD3
#define     PUMP3_ON_SWITCH         PD4
#define     DMS                     PD5
#define     MOTOR_ON_SWITCH         PD6
#define     MPPT_ON_SWITCH          PD7

// OUTPUT PINS DEFINITIONS
#define     LED_PORT                PORTB
#define     LED_PIN                 PINB
#define     LED_DDR                 DDRB
#define     LED                     PB0

#define     cpl_led()               cpl_bit(LED_PORT, LED);
#define     set_led()               set_bit(LED_PORT, LED);
#define     clr_led()               clr_bit(LED_PORT, LED);

// number of checks before reset the pwm fault counter.
#define CHECKS_BEFORE_RESET_FAULT_COUNTER 100
// maximum of consecutive faults before state an error
#define FAULT_COUNT_LIMIT           50

// CANBUS DEFINITIONS
#define SUPPORT_EXTENDED_CANID      0   // 11bits ID
#define SUPPORT_TIMESTAMPS          0   // timestamp not supported in atmega328p

#endif /* ifndef _CONF_H_ */
