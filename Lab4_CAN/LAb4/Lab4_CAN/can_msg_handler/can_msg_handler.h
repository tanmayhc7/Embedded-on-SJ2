#pragma once
#include "can_bus.h"
#include "gpio.h"
typedef enum { SWITCH = 0, TILT = 1 } sensor_t;
void can__tx_msg(can__num_e can, can__msg_t *can_message_ptr);
void can__check_switch_status(can__msg_t *msg);
void can__monitor_sensor_n_send_data(sensor_t dev);
void can__receive_data_n_control_led(void);
void can__control_led_for_tilt_sensor(can__msg_t *msg);
void can__control_led_for_switch(can__msg_t *msg);
uint8_t get_button_status(void);
void can__check_tilt_sensor_status(can__msg_t *msg);
void can__init_gpio(void);