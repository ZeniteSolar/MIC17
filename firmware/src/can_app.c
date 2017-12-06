#include "can_app.h"

/**
 * @brief Prints a can message via usart
 */
inline void can_app_print_msg(can_t *msg)
{
	usart_send_string("ID: ");
	usart_send_uint16(msg->id);
    usart_send_string(" L: ");
    usart_send_uint16(msg->length);
	usart_send_string(". D: ");

	for(uint8_t i = 0; i < msg->length; i++){
	    usart_send_uint16(msg->data[i]);
	    usart_send_char(' ');
	}

	usart_send_string(". ERR: ");
	can_error_register_t err = can_read_error_register();
	usart_send_uint16(err.rx);
	usart_send_char(' ');
	usart_send_uint16(err.tx);
	usart_send_char('\n');
}

/**
* @brief Manages the canbus application protocol
*/
inline void can_app_task(void)
{
    //check_can();

    /*if(can_app_send_state_clk_div++ >= CAN_APP_SEND_STATE_CLK_DIV){
        can_app_send_state_clk_div = 0;
        VERBOSE_MSG(usart_send_string("state msg was sent.\n"));
        can_app_send_state();
    }*/

    if(can_app_send_motor_clk_div++ >= CAN_APP_SEND_MOTOR_CLK_DIV){
        can_app_send_motor_clk_div = 0;
        VERBOSE_MSG(usart_send_string("motor msg was sent.\n"));
        can_app_send_motor();
    }
}

inline void can_app_send_state(void)
{
    can_t msg;

    msg.id                                  = CAN_FILTER_MSG_MIC17_STATE;
    msg.length                              = CAN_LENGTH_MSG_STATE;

    msg.data[CAN_SIGNATURE_BYTE]            = CAN_SIGNATURE_SELF;
    msg.data[CAN_STATE_MSG_STATE_BYTE]      = (uint8_t) state_machine;
    msg.data[CAN_STATE_MSG_ERROR_BYTE]      = error_flags.all;

    can_app_print_msg(&msg);
    can_send_message(&msg);
}

inline void can_app_send_motor(void)
{
    can_t msg;

    msg.id                                  = CAN_FILTER_MSG_MIC17_MOTOR;
    msg.length                              = CAN_LENGTH_MSG_MIC17_MOTOR;

    for(uint8_t i = msg.length; i; i--)     msg.data[i-1] = 0;

    msg.data[CAN_SIGNATURE_BYTE]            = CAN_SIGNATURE_SELF;
    msg.data[CAN_MSG_MIC17_MOTOR_D_RAW_BYTE]    = control.motor_D_raw_target;
    msg.data[CAN_MSG_MIC17_MOTOR_I_RAW_BYTE]    = control.motor_I_raw_target;

    msg.data[CAN_MSG_MIC17_MOTOR_MOTOR_ON_BYTE] |= 
        ((system_flags.motor_on) << CAN_MSG_MIC17_MOTOR_MOTOR_ON_BIT);

    msg.data[CAN_MSG_MIC17_MOTOR_DMS_BYTE] |= 
        ((system_flags.dms) << CAN_MSG_MIC17_MOTOR_DMS_BIT);

    can_app_print_msg(&msg);
    can_send_message(&msg); 
}

inline void can_app_send_pumps(void)
{
    can_t msg;

    msg.id                                  = CAN_FILTER_MSG_MIC17_PUMPS;
    msg.length                              = CAN_LENGTH_MSG_MIC17_PUMPS;

    for(uint8_t i = msg.length; i; i--)     msg.data[i-1] = 0;

    msg.data[CAN_SIGNATURE_BYTE]            = CAN_SIGNATURE_SELF;
    msg.data[CAN_MSG_MIC17_PUMPS_PUMPS_BYTE]    |= 
        ((system_flags.pump1_on) << CAN_MSG_MIC17_PUMPS_PUMP1_BIT);
    msg.data[CAN_MSG_MIC17_PUMPS_PUMPS_BYTE]    |= 
        ((system_flags.pump2_on) << CAN_MSG_MIC17_PUMPS_PUMP2_BIT);
    msg.data[CAN_MSG_MIC17_PUMPS_PUMPS_BYTE]    |= 
        ((system_flags.pump3_on) << CAN_MSG_MIC17_PUMPS_PUMP3_BIT);

    can_app_print_msg(&msg);
    can_send_message(&msg); 
}

inline void can_app_send_mppts(void)
{
    can_t msg;

    msg.id                                  = CAN_FILTER_MSG_MIC17_PUMPS;
    msg.length                              = CAN_LENGTH_MSG_MIC17_PUMPS;

    for(uint8_t i = msg.length; i; i--)     msg.data[i-1] = 0;

    msg.data[CAN_SIGNATURE_BYTE]            = CAN_SIGNATURE_SELF;

    msg.data[CAN_MSG_MIC17_MPPTS_POT_BYTE]    = control.mppts_I_raw_target;

    msg.data[CAN_MSG_MIC17_MPPTS_MPPTS_ON_BYTE] |= 
        ((system_flags.mppt_on) << CAN_MSG_MIC17_MPPTS_MPPTS_ON_BIT);
     
    can_app_print_msg(&msg);
    can_send_message(&msg); 
}

inline void can_app_send_mcs(void)
{
    can_t msg;

    msg.id                                  = CAN_FILTER_MSG_MIC17_PUMPS;
    msg.length                              = CAN_LENGTH_MSG_MIC17_PUMPS;

    for(uint8_t i = msg.length; i; i--)     msg.data[i-1] = 0;

    msg.data[CAN_SIGNATURE_BYTE]            = CAN_SIGNATURE_SELF;

    msg.data[CAN_MSG_MIC17_MCS_BOAT_ON_BYTE] |= 
        ((system_flags.boat_on) << CAN_MSG_MIC17_MCS_BOAT_ON_BIT);
     
    can_app_print_msg(&msg);
    can_send_message(&msg); 
} 

/**
 * @brief redirects a specific message extractor to a given message
 * @param *msg pointer to the message to be extracted
 */
inline void can_app_msg_extractors_switch(can_t *msg)
{
    if(msg->data[CAN_SIGNATURE_BYTE] == CAN_SIGNATURE_MIC17){
        switch(msg->id){
            default:
                break;
        }    
    }
}

/**
 * @brief Manages to receive and extract specific messages from canbus
 */
inline void check_can(void)
{
    if(can_check_message()){
        can_t msg;
        if(can_get_message(&msg)){
            can_app_msg_extractors_switch(&msg);
        }
    }
}

