#ifndef _CAN_APP_H_
#define _CAN_APP_H_ 

#include "conf.h"
#include "machine.h"
#include "can.h"
#include "can_ids.h"
#include "usart.h"
#include "dbg_vrb.h"

void can_app_print_msg(can_t *msg);
void can_app_task(void);
void can_app_send_state(void);
void can_app_send_motor(void);
void can_app_send_pumps(void);
void can_app_send_mcs(void);
void can_app_send_mppts(void);
void can_app_extractor_mic17_state(can_t *msg);
void can_app_extractor_mic17_motor(can_t *msg);
void can_app_msg_extractors_switch(can_t *msg);
void check_can(void);

#define CAN_APP_SEND_STATE_CLK_DIV 100
#define CAN_APP_SEND_MOTOR_CLK_DIV 0
#define CAN_APP_SEND_PUMPS_CLK_DIV 6
#define CAN_APP_SEND_MCS_CLK_DIV 6
#define CAN_APP_SEND_MPPTS_CLK_DIV 6

uint8_t can_app_send_state_clk_div;
uint8_t can_app_send_motor_clk_div;
uint8_t can_app_send_pumps_clk_div;
uint8_t can_app_send_mcs_clk_div;
uint8_t can_app_send_mppts_clk_div;


#endif /* ifndef _CAN_APP_H_ */
