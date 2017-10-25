#include "machine.h"

/*
 * to-do:
 *      - modularize state sinalization
 *      - 
 *
 */

/**
 * @brief checks if the switches updating system flags
 */
inline void check_switches(void)
{
    VERBOSE_MSG(usart_send_string("DMS: "));
    if(bit_is_set(SWITCHES_PIN, DMS)){
        VERBOSE_MSG(usart_send_string("On.  "));
        system_flags.dms = 1;   
    }else{
        VERBOSE_MSG(usart_send_string("Off. "));
        system_flags.dms = 0;   
    }
 
    VERBOSE_MSG(usart_send_string("Motor: "));
    if(bit_is_clear(SWITCHES_PIN, MOTOR_ON_SWITCH)){
        VERBOSE_MSG(usart_send_string("On.  "));
        system_flags.motor_on = 1;
    }else{
        VERBOSE_MSG(usart_send_string("Off. "));
        system_flags.motor_on = 0;
    }

    VERBOSE_MSG(usart_send_string("MPPT: "));
    if(bit_is_set(SWITCHES_PIN, MPPT_ON_SWITCH)){
        VERBOSE_MSG(usart_send_string("On.  "));
        system_flags.mppt_on = 1;   
    }else{
        VERBOSE_MSG(usart_send_string("Off. "));
        system_flags.mppt_on = 0;   
    } 

    VERBOSE_MSG(usart_send_string("Pump 1: "));
    if(bit_is_set(SWITCHES_PIN, PUMP1_ON_SWITCH)){
        VERBOSE_MSG(usart_send_string("On.  "));
        system_flags.pump1_on = 1;   
    }else{
        VERBOSE_MSG(usart_send_string("Off. "));
        system_flags.pump1_on = 0;   
    }

    VERBOSE_MSG(usart_send_string("Pump 2: "));
    if(bit_is_set(SWITCHES_PIN, PUMP2_ON_SWITCH)){
        VERBOSE_MSG(usart_send_string("On.  "));
        system_flags.pump2_on = 1;   
    }else{
        VERBOSE_MSG(usart_send_string("Off. "));
        system_flags.pump2_on = 0;   
    }
    
    VERBOSE_MSG(usart_send_string("Pump 3: "));
    if(bit_is_set(SWITCHES_PIN, PUMP3_ON_SWITCH)){
        VERBOSE_MSG(usart_send_string("On.  "));
        system_flags.pump3_on = 1;   
    }else{
        VERBOSE_MSG(usart_send_string("Off. "));
        system_flags.pump3_on = 0;   
    }
    VERBOSE_MSG(usart_send_char('\n'));
}

/**
 * @brief checks the buffer and waits it fills up
 */
inline void check_buffers(void)
{
    VERBOSE_MSG(usart_send_string("Checking buffers..."));
    while(!CBUF_IsFull(cbuf_adc0));
    VERBOSE_MSG(usart_send_string(" \t\tdone.\n")); 
}

/**
 * @brief set error state
 */
inline void set_state_error(void)
{
    state_machine = STATE_ERROR;
}

/**
* @brief set initializing state
*/ 
inline void set_state_initializing(void)
{
    state_machine = STATE_INITIALIZING;
}

/**
* @brief set running state
*/ 
inline void set_state_running(void)
{
    state_machine = STATE_RUNNING;
}

/**
 * @brief Checks if the system is OK to run:
 *  - all ring_buffers needed to be full
 *  - checks the current
 *  - checks the voltage
 *  - checks the temperature
 *
 */
inline void task_initializing(void)
{
    set_led();

    check_can();
    //check_buffers();

    if(!error_flags.all){
        VERBOSE_MSG(usart_send_string("System initialized without errors.\n"));
        set_state_running();
    }
    else{
        VERBOSE_MSG(usart_send_string("Sorry. I have have found errors in the initialilation process. \n\nI will begin to process it...\n"));
        set_state_error();
    }
}

/**
 * @brief running task updates the system flags and send data to canbus
 */
inline void task_running(void)
{
    if(led_clk_div++ >= 10){
        cpl_led();
        led_clk_div = 0;
    }

    check_switches();

    can_app_task();

}

/**
 * @brief error task checks the system and tries to medicine it.
 */
inline void task_error(void)
{
    if(led_clk_div++ >= 5){
        cpl_led();
        led_clk_div = 0;
    }

    total_errors++;         // incrementa a contagem de erros
    VERBOSE_MSG(usart_send_string("The error code is: "));
    VERBOSE_MSG(usart_send_char(error_flags.all));
    VERBOSE_MSG(usart_send_char('\n'));

    if(!error_flags.all)
        VERBOSE_MSG(usart_send_string("\t - Oh no, it was some unknown error.\n"));
 
    VERBOSE_MSG(usart_send_string("The error level is: "));
    VERBOSE_MSG(usart_send_uint16(total_errors));
    VERBOSE_MSG(usart_send_char('\n'));
    
    if(total_errors < 2){
        VERBOSE_MSG(usart_send_string("I will reset the machine state.\n"));
    }
    if(total_errors >= 20){
        VERBOSE_MSG(usart_send_string("The watchdog will reset the whole system.\n"));
        for(;;);    // waits the watchdog to reset.
    }
    
    cpl_led();
    set_state_initializing();
}


/**
 * @brief this is the machine state itself.
 */
inline void machine_run(void)
{

    
    switch(state_machine){
        case STATE_INITIALIZING:
            task_initializing();

            break;
        case STATE_RUNNING:
            task_running();

            break;
        case STATE_ERROR:
            task_error();

        default:
            break;
    }
}

/**
 * @brief Interrupcao das chaves: se alguma chave desligar, o motor desliga.
 */
ISR(PCINT2_vect)
{    
    if(bit_is_set(SWITCHES_PIN, MOTOR_ON_SWITCH)
            || bit_is_clear(SWITCHES_PIN, DMS)){
        set_state_initializing();
    }

    DEBUG1;
}

