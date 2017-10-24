#include "can_app.h"

/**
 * @brief Prints a can message via usart
 */
inline void can_app_print_msg(can_t *msg)
{
	usart_send_string("ID: ");
	usart_send_uint16(msg->id);
	usart_send_string(". Data: ");

	for(uint8_t i = 0; i < msg->length; i++){
	    usart_send_uint16(msg->data[i]);
	    usart_send_char(' ');
	}

	usart_send_string(". ERR: ");
	
//	usart_send_uint16(can_msg_err);
	usart_send_char('\n');
}

/**
* @brief Manages the canbus application protocol
*/
inline void can_app_task(void)
{
    check_can();
    can_app_send_state();
}

inline void can_app_send_state(void)
{
    can_t msg;
    msg.id                                  = CAN_FILTER_MSG_AC17_STATE;
    msg.length                              = CAN_LENGTH_MSG_STATE;

    msg.data[CAN_SIGNATURE_BYTE]            = CAN_SIGNATURE_SELF;
    msg.data[CAN_STATE_MSG_STATE_BYTE]      = (uint8_t) state_machine;
    msg.data[CAN_STATE_MSG_ERROR_BYTE]      = error_flags.all;

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

