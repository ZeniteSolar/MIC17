/**
 * 
 * @todo: adicionar um filtro janelado para cada adc
 *
 */

#include "main.h"

/**
 * @brief configura os periféricos
 */
inline static void setup(void)
{

#ifdef USART_ON
    usart_init(MYUBRR,1,1);                         // inicializa a usart
	VERBOSE_MSG(usart_send_string("\n\n\nUSART... OK!\n"));
#endif

    _delay_ms(1000);

#ifdef CAN_ON
	VERBOSE_MSG(usart_send_string("CAN (125kbps)..."));
    can_init(BITRATE_500_KBPS);
	VERBOSE_MSG(usart_send_string(" OK!\n"));
	VERBOSE_MSG(usart_send_string("CAN filters..."));
    can_static_filter(can_filter);
	VERBOSE_MSG(usart_send_string(" OK!\n"));
#endif 

	VERBOSE_MSG(usart_send_string("I/O's..."));
    // configuracao dos pinos I/O
    set_bit(LED_DDR, LED);                      // LED como saída
    clr_bit(SWITCHES_DDR, DMS);                 // DEADMAN como entrada
    set_bit(SWITCHES_PORT, DMS);                // DEADMAN com pull-up
    clr_bit(SWITCHES_DDR, MOTOR_ON_SWITCH);     // MOTOR ON como entrada
    set_bit(SWITCHES_PORT, MOTOR_ON_SWITCH);    // MOTOR ON com pull-up
    clr_bit(SWITCHES_DDR, MPPT_ON_SWITCH);      // MPPT ON como entrada
    set_bit(SWITCHES_PORT, MPPT_ON_SWITCH);     // MPPT ON com pull-up
    clr_bit(SWITCHES_DDR, PUMP1_ON_SWITCH);     // PUMP1 como entrada
    set_bit(SWITCHES_PORT, PUMP1_ON_SWITCH);    // PUMP1 com pull-up
    clr_bit(SWITCHES_DDR, PUMP2_ON_SWITCH);     // PUMP2 como entrada
    set_bit(SWITCHES_PORT, PUMP2_ON_SWITCH);    // PUMP2 com pull-up
    clr_bit(SWITCHES_DDR, PUMP3_ON_SWITCH);     // PUMP3 como entrada
    set_bit(SWITCHES_PORT, PUMP3_ON_SWITCH);    // PUMP3 com pull-up
 	VERBOSE_MSG(usart_send_string(" OK!\n"));

/*	
 	VERBOSE_MSG(usart_send_string("External Interrupts..."));
    // Configuracoes da interrupcao externa para as chaves e a interrupcao externa por FAULT (IR2127)
    set_bit(PCMSK2, PCINT20);                   // DEADMAN com interrupcao
    set_bit(PCMSK2, PCINT21);                   // ON/OFF com interrupcao
    set_bit(PCICR, PCIE2);                      // enables external interrupts for PCINT23~16
    set_bit(PCIFR, PCIF2);                      // clears external interrupt requests for PCINT23~16
	 VERBOSE_MSG(usart_send_string(" OK!\n"));
     */

#ifdef DEBUG_ON
	VERBOSE_MSG(usart_send_string("Debug I/O's..."));
    set_bit(DDRB, PB5);
    set_bit(DDRB, PB4);
	VERBOSE_MSG(usart_send_string(" OK!\n"));
#endif 

#ifdef ADC_ON
	VERBOSE_MSG(usart_send_string("ADC..."));
    adc_init();
	VERBOSE_MSG(usart_send_string(" OK!\n"));
#endif

#ifdef MACHINE_ON
	VERBOSE_MSG(usart_send_string("MACHINE..."));
	machine_init();
	VERBOSE_MSG(usart_send_string(" OK!\n"));
#endif

#ifdef SLEEP_ON 
	VERBOSE_MSG(usart_send_string("SLEEP..."));
    sleep_init();
	VERBOSE_MSG(usart_send_string(" OK!\n"));
#endif

#ifdef WATCHDOG_ON
	VERBOSE_MSG(usart_send_string("WATCHDOG..."));
    wdt_init();
	VERBOSE_MSG(usart_send_string(" OK!\n"));
#endif
 
   sei();				                            // liga a chave geral das interrupcoes

}


/**
 * @brief main
 */
int main(void)
{
    #ifdef MACHINE_ON
        system_flags.all = error_flags.all = 0;
        state_machine = STATE_INITIALIZING;
    #endif

    setup();

    for(;;){
        #ifdef WATCHDOG_ON
            wdt_reset();                                // checkpoint: reset watchdog timer
        #endif

        #ifdef MACHINE_ON
            machine_run();
        #endif

        #ifdef SLEEP_ON 
            sleep_mode();           // entra em modo IDLE até a próxima interrupção
        #endif

    }
}

/**
 * @brief se em debug, fica chaveando os pinos de debugs até o reset do watchdog
 */
ISR(BADISR_vect)
{   
    for(;;){
#ifdef DEBUG_ON
        DEBUG0;
        DEBUG1;
        _delay_ms(100);
#endif
    }
}

 /*
EMPTY_INTERRUPT(TIMER0_COMPB_vect);
EMPTY_INTERRUPT(TIMER0_OVF_vect);
*/
/*
EMPTY_INTERRUPT(TIMER1_CAPT_vect);
EMPTY_INTERRUPT(TIMER1_COMPA_vect);
EMPTY_INTERRUPT(TIMER1_COMPB_vect);
EMPTY_INTERRUPT(TIMER1_OVF_vect);
*/
/*
EMPTY_INTERRUPT(TIMER2_COMPB_vect);
EMPTY_INTERRUPT(TIMER2_OVF_vect);
*/
