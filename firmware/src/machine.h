/**
 * @file machine.h
 *
 * @defgroup MACHINE State Machine Module
 *
 * @brief Implements the main state machine of the system.
 *
 */

#ifndef MACHINE_H
#define MACHINE_H 

#include <avr/io.h>
#include <avr/wdt.h>

#include "conf.h"

#ifdef ADC_ON
#include "adc.h"
#endif
#ifdef USART_ON
#include "usart.h"
#endif
#include "dbg_vrb.h"
#ifdef CAN_ON
#include "can.h"
#include "can_app.h"
extern const uint8_t can_filter[];
#endif

typedef enum state_machine{
    STATE_INITIALIZING,
    STATE_RUNNING,
    STATE_IDLE,
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
    uint8_t     motor_dutycycle_raw_target;     // value for target pwm, from 0 to 255
    uint8_t     motor_increment_raw_target;     // value for target pwm, from 0 to 255
    uint8_t     mppts_power_raw_target;         // value for target pwm, from 0 to 255
}control_t;

control_t control;

uint8_t dms_counter_is_clear;
uint8_t dms_counter_is_set;
uint8_t boat_on_counter_is_clear;
uint8_t boat_on_counter_is_set;

#define DMS_COUNTER_IS_SET_LIMIT        0
#define DMS_COUNTER_IS_CLEAR_LIMIT      0
#define BOAT_ON_COUNTER_IS_SET_LIMIT    0
#define BOAT_ON_COUNTER_IS_CLEAR_LIMIT  0

void check_boat_on(void);


// machine checks
void check_switches(void);
void check_buffers(void);
void check_motor_dutycycle(void);
void check_motor_increment(void);
void check_mppts_power(void);

// debug functions
void print_system_flags(void);
void print_error_flags(void);
void print_control(void);

// machine tasks
void task_initializing(void);
void task_idle(void);
void task_running(void);
void task_error(void);

// the machine itself
void machine_init(void);
void machine_run(void);
void set_state_error(void);
void set_state_initializing(void);
void set_state_idle(void);
void set_state_running(void);

// machine variables
state_machine_t state_machine;
system_flags_t system_flags;
error_flags_t error_flags;
uint8_t total_errors;   // Contagem de ERROS
volatile uint8_t machine_clk;

// other variables
uint8_t led_clk_div;
uint8_t buzzer_clk_div;
#define buzzer_max       110      // defines the frequency for bip (running)

// ISRs
ISR(TIMER2_COMPA_vect);
ISR(PCINT2_vect);

#endif /* ifndef MACHINE_H */
