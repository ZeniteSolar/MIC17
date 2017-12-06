/**
 * @file machine.h
 *
 * @defgroup MACHINE State Machine Module
 *
 * @brief Implements the main state machine of the system.
 *
 */

#ifndef _MACHINE_H_
#define _MACHINE_H_ 

#include <avr/io.h>
#include <avr/wdt.h>

#include "conf.h"
#include "adc.h"
#include "usart.h"
#include "dbg_vrb.h"
#ifdef CAN_ON
#include "can.h"
#include "can_app.h"
#endif

typedef enum state_machine{
    STATE_INITIALIZING,
    STATE_RUNNING,
    STATE_ERROR,
} state_machine_t;

typedef union system_flags{
    struct{
        uint8_t     can_enabled     :1;
        uint8_t     pump1_on        :1;
        uint8_t     pump2_on        :1;
        uint8_t     pump3_on        :1;
        uint8_t     dms             :1;
        uint8_t     motor_on        :1;
        uint8_t     mppt_on         :1;
        uint8_t     boat_on         :1;
    };
    uint8_t   all;
} system_flags_t;

typedef union error_flags{
    struct{
        uint8_t     none       :1;
    };
    uint8_t   all;
}error_flags_t;

typedef struct control{
    uint8_t     motor_D_raw_target;   // value for target pwm, from 0 to 255
    uint8_t     motor_I_raw_target;   // value for target pwm, from 0 to 255
    uint8_t     mppts_I_raw_target;   // value for target pwm, from 0 to 255
}control_t;

control_t control;

// machine checks
void check_switches(void);
void check_buffers(void);

// machine tasks
void task_initializing(void);
void task_running(void);
void task_error(void);

// the machine itself
void machine_init(void);
void machine_run(void);
void set_state_error(void);
void set_state_initializing(void);
void set_state_running(void);

// machine variables
state_machine_t state_machine;
system_flags_t system_flags;
error_flags_t error_flags;
uint8_t total_errors;   // Contagem de ERROS
volatile uint8_t machine_clk;

// other variables
uint8_t led_clk_div;

ISR(TIMER2_COMPA_vect);

// externs

#endif /* ifndef _MACHINE_H_ */
