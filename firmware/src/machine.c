#include "machine.h"

/**
 * @brief 
 */
void machine_init(void)
{
    TCCR2A  =   (1 << WGM21) | (0 << WGM20)         // Timer 2 in Mode 2 = CTC (clear on compar  e)
            | (0 << COM2A1) | (0 << COM2A0)         // do nothing with OC2A
            | (0 << COM2B1) | (0 << COM2B0);        // do nothing with OC2B
    TCCR2B  =   (0 << WGM22)                        // Timer 0 in Mode 2 = CTC (clear on compar  e)
            | (0 << FOC0A) | (0 << FOC0B)           // dont force outputs
            | (1 << CS22)                           // clock enabled, prescaller = 1024
            | (1 << CS21)
            | (1 << CS20);
    OCR2A   =   80;                                // Valor para igualdade de comparacao A par  a frequencia de 150 Hz
    TIMSK2 |=   (1 << OCIE2A);                      // Ativa a interrupcao na igualdade de comp  aração do TC2 com OCR2A

    dms_counter_is_set = dms_counter_is_clear = 0;
    boat_on_counter_is_set = boat_on_counter_is_clear = 0;

} 

void check_boat_on(void)
{
    VERBOSE_MSG(usart_send_string("BOAT: "));
    if(bit_is_clear(CTRL_SWITCHES_PIN, BOAT_ON_SWITCH)){
        boat_on_counter_is_set = 0;
        if(boat_on_counter_is_clear++ >= BOAT_ON_COUNTER_IS_CLEAR_LIMIT){
            boat_on_counter_is_clear = 0;
            VERBOSE_MSG(usart_send_string("On.  "));
            system_flags.boat_on = 1;   
        }
    }else{
        boat_on_counter_is_clear = 0;
        if(boat_on_counter_is_set >= BOAT_ON_COUNTER_IS_SET_LIMIT){
            boat_on_counter_is_set = 0
            VERBOSE_MSG(usart_send_string("Off. "));
            system_flags.boat_on = 0;   
        }
    } 
}

/**
 * @brief checks if the switches updating system flags
 */
void check_switches(void)
{
    VERBOSE_MSG(usart_send_string("DMS: "));
    if(bit_is_set(DMS_PIN, DMS)){
        dms_counter_is_clear = 0;
        if(dms_counter_is_set++ >= DMS_COUNTER_IS_SET_LIMIT){
            VERBOSE_MSG(usart_send_string("On.  "));           
            system_flags.dms = 1;                               
        }
    }else{
        dms_counter_is_set = 0;
        if(dms_counter_is_clear++ >= DMS_COUNTER_IS_CLEAR_LIMIT){
            VERBOSE_MSG(usart_send_string("Off. "));
            system_flags.dms = 0;   
        }
    }
 
    

    VERBOSE_MSG(usart_send_string("Motor: "));
    if(bit_is_clear(CTRL_SWITCHES_PIN, MOTOR_ON_SWITCH)){
        VERBOSE_MSG(usart_send_string("On.  "));
        system_flags.motor_on = 1;
    }else{
        VERBOSE_MSG(usart_send_string("Off. "));
        system_flags.motor_on = 0;
    }

    VERBOSE_MSG(usart_send_string("MPPT: "));
    if(bit_is_clear(CTRL_SWITCHES_PIN, MPPT_ON_SWITCH)){
        VERBOSE_MSG(usart_send_string("On.  "));
        system_flags.mppt_on = 1;   
    }else{
        VERBOSE_MSG(usart_send_string("Off. "));
        system_flags.mppt_on = 0;   
    } 

    VERBOSE_MSG(usart_send_string("Pump 1: "));
    if(bit_is_clear(PUMPS_SWITCHES_PIN, PUMP1_ON_SWITCH)){
        VERBOSE_MSG(usart_send_string("On.  "));
        system_flags.pump1_on = 1;   
    }else{
        VERBOSE_MSG(usart_send_string("Off. "));
        system_flags.pump1_on = 0;   
    }

    VERBOSE_MSG(usart_send_string("Pump 2: "));
    if(bit_is_clear(PUMPS_SWITCHES_PIN, PUMP2_ON_SWITCH)){
        VERBOSE_MSG(usart_send_string("On.  "));
        system_flags.pump2_on = 1;   
    }else{
        VERBOSE_MSG(usart_send_string("Off. "));
        system_flags.pump2_on = 0;   
    }
    
    VERBOSE_MSG(usart_send_string("Pump 3: "));
    if(bit_is_clear(PUMPS_SWITCHES_PIN, PUMP3_ON_SWITCH)){
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
    VERBOSE_MSG(usart_send_string(" \t\t0...\n")); 
    while(!CBUF_IsFull(cbuf_adc1));
    VERBOSE_MSG(usart_send_string(" \t\t1...\n")); 
    while(!CBUF_IsFull(cbuf_adc2));
    VERBOSE_MSG(usart_send_string(" \t\t2...\n")); 
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
* @brief set idle state
*/ 
inline void set_state_idle(void)
{
    state_machine = STATE_IDLE;
}

/**
* @brief set running state
*/ 
inline void set_state_running(void)
{
    state_machine = STATE_RUNNING;
}

/**
* @brief checks the motor duty cylce
*/ 
inline void check_motor_dutycycle(void){
    control.motor_dutycycle_raw_target = ma_adc0();
}

/**
* @brief checks the motor increment of duty cycle
*/ 
inline void check_motor_increment(void){
    control.motor_increment_raw_target = ma_adc1();
}

/**
* @brief checks mppt power
*/ 
inline void check_mppts_power(void){
   control.mppts_power_raw_target = ma_adc2();
}
 
/**
* @brief prints the system flags
*/
inline void print_system_flags(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string(" PMP1: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+system_flags.pump1_on));

    VERBOSE_MSG_MACHINE(usart_send_string(" PMP2: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+system_flags.pump2_on));
    
    VERBOSE_MSG_MACHINE(usart_send_string(" PMP3: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+system_flags.pump3_on));

    VERBOSE_MSG_MACHINE(usart_send_string(" DMS: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+system_flags.dms));

    VERBOSE_MSG_MACHINE(usart_send_string(" MOTOR: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+system_flags.motor_on));

    VERBOSE_MSG_MACHINE(usart_send_string(" MPPT: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+system_flags.mppt_on));

    VERBOSE_MSG_MACHINE(usart_send_string(" BOAT: "));
    VERBOSE_MSG_MACHINE(usart_send_char(48+system_flags.boat_on));

    VERBOSE_MSG_MACHINE(usart_send_char('\n'));
}

/**
* @brief prints the error flags
*/
inline void print_error_flags(void)
{
    // VERBOSE_MSG_MACHINE(usart_send_string(" NOCAN: "));
    // VERBOSE_MSG_MACHINE(usart_send_char(48+error_flags.no_canbus));
}
 
/**
* @brief prints the error flags
*/
inline void print_control(void)
{
    VERBOSE_MSG_MACHINE(usart_send_string(" D: "));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.motor_dutycycle_raw_target));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
    
    VERBOSE_MSG_MACHINE(usart_send_string(" D_INC: "));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.motor_increment_raw_target));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));

    VERBOSE_MSG_MACHINE(usart_send_string(" MPPT_P: "));
    VERBOSE_MSG_MACHINE(usart_send_uint16(control.mppts_power_raw_target));
    VERBOSE_MSG_MACHINE(usart_send_char(' '));
}

/**
 * @brief Checks if the system is OK to run:
 *  - all ring_buffers needed to be full
 *  - checks the current
 *  - checks the voltage
 *
 */
inline void task_initializing(void)
{
    set_led();

#ifdef CAN_ON
    check_can();
#endif

#ifdef ADC_ON
    check_buffers();
#endif    

    if(!error_flags.all){
        VERBOSE_MSG_INIT(usart_send_string("System initialized without errors.\n"));
        set_state_idle();
    }
    else{
        VERBOSE_MSG_ERROR(usart_send_string("Sorry. I have have found errors in the initialilation process. \n\nI will begin to process it...\n"));
        set_state_error();
    }
}

/**
 * @brief waits for commands while checking the system:
 *  - checks the deadman's switch state
 *  - checks the on_off_switch state
 *  - checks the potentiometer state
 *  then if its ok, enable the system to operate
 */
inline void task_idle(void)
{
    if(led_clk_div++ >= 50){
        cpl_led();
        led_clk_div = 0;
    }

    check_boat_on();

    if(system_flags.boat_on) {
        VERBOSE_MSG_MACHINE(usart_send_string("Enjoy, the system is at its RUNNING STATE!!\n"));
        set_state_running(); 
    }
}


/**
 * @brief running task checks the system and apply the control action to pwm.
 */
inline void task_running(void)
{
    if(led_clk_div++ >= 10){
        cpl_led();
        led_clk_div = 0;
    }

    if(buzzer_clk_div++ == 0){
        set_buzzer();
    }else if(buzzer_clk_div >= 4){
        clr_buzzer();
    }else if(buzzer_clk_div >= buzzer_max){
        buzzer_clk_div = 0;
    }
    
    check_boat_on();
    check_switches();
    check_motor_dutycycle();
    check_motor_increment();
    check_mppts_power();

    if(!system_flags.boat_on){
        set_state_idle();
    }
        

    VERBOSE_MSG(usart_send_string(" ADC0(D) :"));
    VERBOSE_MSG(usart_send_uint16(control.motor_dutycycle_raw_target));
    VERBOSE_MSG(usart_send_string(" ADC1(I) :"));
    VERBOSE_MSG(usart_send_uint16(control.motor_increment_raw_target));
    VERBOSE_MSG(usart_send_string(" ADC2(I) :"));
    VERBOSE_MSG(usart_send_uint16(control.mppts_power_raw_target));
    VERBOSE_MSG(usart_send_char(' '));

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
    VERBOSE_MSG_ERROR(usart_send_string("The error code is: "));
    VERBOSE_MSG_ERROR(usart_send_uint16(error_flags.all));
    VERBOSE_MSG_ERROR(usart_send_char('\n'));

    // if(error_flags.no_canbus)
    //     VERBOSE_MSG_ERROR(usart_send_string("\t - No canbus communication with MIC17!\n"));
    if(!error_flags.all)
        VERBOSE_MSG_ERROR(usart_send_string("\t - Oh no, it was some unknown error.\n"));
 
    VERBOSE_MSG_ERROR(usart_send_string("The error level is: "));
    VERBOSE_MSG_ERROR(usart_send_uint16(total_errors));
    VERBOSE_MSG_ERROR(usart_send_char('\n'));
    
    if(total_errors < 2){
        VERBOSE_MSG_ERROR(usart_send_string("I will reset the machine state.\n"));
    }
    if(total_errors >= 20){
        VERBOSE_MSG_ERROR(usart_send_string("The watchdog will reset the whole system.\n"));
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
	#ifdef CAN_ON
    can_app_task();
	#endif

    print_system_flags();
    print_error_flags();
    print_control();

    if(machine_clk){
        machine_clk = 0;
        switch(state_machine){
            case STATE_INITIALIZING:
                task_initializing();

                break;
            case STATE_IDLE:
                task_idle();

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
}

/**
 * @brief Interrupcao das chaves: se alguma chave desligar, o motor desliga.
 */

ISR(INT0_vect) //overvoltage
{
	// DEBUG1;
}
ISR(INT1_vect) //enable
{    
    // DEBUG1;
}

/**
* @brief ISR para ações de controle
*/
ISR(TIMER2_COMPA_vect)
{
	machine_clk = 1;
}

