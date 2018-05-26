/**
 * @file conf.h
 *
 * @defgroup CONF Configurations 
 *
 * @brief General configuration of the system.
 *
 */

#ifndef CONF_H
#define CONF_H

// CONFIGURACOES DE COMPILACAO
//#define DEBUG_ON
//#define VERBOSE_ON
//#define VERBOSE_ON_CAN_APP
#define VERBOSE_ON_MACHINE
//#define VERBOSE_ON_ADC
#define VERBOSE_ON_INIT
#define VERBOSE_ON_ERROR
//#define CAN_PRINT_MSG

// TESTS
 

// MODULES ACTIVATION
#define USART_ON
#define CAN_ON
#define ADC_ON
#define MACHINE_ON
#define WATCHDOG_ON
#define SLEEP_ON
 
// INPUT PINS DEFINITIONS
#define     PUMPS_SWITCHES_PORT     PORTD
#define     PUMPS_SWITCHES_PIN      PIND
#define     PUMPS_SWITCHES_DDR      DDRD
#define     PUMP1_ON_SWITCH         PD2
#define     PUMP2_ON_SWITCH         PD3
#define     PUMP3_ON_SWITCH         PD4

#define     DMS_PORT                PORTD
#define     DMS_PIN                 PIND
#define     DMS_DDR                 DDRD
#define     DMS                     PD5

#define     CTRL_SWITCHES_PORT      PORTC
#define     CTRL_SWITCHES_PIN       PINC
#define     CTRL_SWITCHES_DDR       DDRC
#define     BOAT_ON_SWITCH          PC3
#define     MOTOR_ON_SWITCH         PC4
#define     MPPT_ON_SWITCH          PC5

// OUTPUT PINS DEFINITIONS
#define     LED_PORT                PORTD
#define     LED_PIN                 PIND
#define     LED_DDR                 DDRD
#define     LED                     PD6
#define     cpl_led()               cpl_bit(LED_PORT, LED)
#define     set_led()               set_bit(LED_PORT, LED)
#define     clr_led()               clr_bit(LED_PORT, LED)

#define     BUZZER_PORT             PORTD
#define     BUZZER_PIN              PIND
#define     BUZZER_DDR              DDRD
#define     BUZZER                  PD7
#define     cpl_buzzer()            cpl_bit(BUZZER_PORT, BUZZER)
#define     set_buzzer()            set_bit(BUZZER_PORT, BUZZER)
#define     clr_buzzer()            clr_bit(BUZZER_PORT, BUZZER)


// number of checks before reset the pwm fault counter.
#define CHECKS_BEFORE_RESET_FAULT_COUNTER 100
// maximum of consecutive faults before state an error
#define FAULT_COUNT_LIMIT           50

// CANBUS DEFINITONS
// ----------------------------------------------------------------------------
/* Global settings for building the can-lib and application program.
 *
 * The following two #defines must be set identically for the can-lib and
 * your application program. They control the underlying CAN struct. If the
 * settings disagree, the underlying CAN struct will be broken, with
 * unpredictable results.
 * If can.h detects that any of the #defines is not defined, it will set them
 * to the default values shown here, so it is in your own interest to have a
 * consistent setting. Ommiting the #defines in both can-lib and application
 * program will apply the defaults in a consistent way too.
 *
 * Select if you want to use 29 bit identifiers.
 */
#define	SUPPORT_EXTENDED_CANID	0

/* Select if you want to use timestamps.
 * Timestamps are sourced from a register internal to the AT90CAN.
 * Selecting them on any other controller will have no effect, they will
 * be 0 all the time.
 */
#define	SUPPORT_TIMESTAMPS		0



#endif /* ifndef CONF_H */
