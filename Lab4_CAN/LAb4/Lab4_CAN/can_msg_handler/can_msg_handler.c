#include <stdio.h>

#include "board_io.h"

#include "can_initializer.h"
#include "can_msg_handler.h"
#include "tilt_sensor.h"

/***********************************************
 *                  GPIO Init for switch
 **********************************************/
static gpio_s switch_button0, switch_button1, switch_button2, switch_button3;
void can__init_gpio(void) {
  switch_button0 = gpio__construct_as_input(GPIO__PORT_0, 29);
  switch_button1 = gpio__construct_as_input(GPIO__PORT_0, 30);
  switch_button2 = gpio__construct_as_input(GPIO__PORT_1, 15);
  switch_button3 = gpio__construct_as_input(GPIO__PORT_1, 19);
}

/**********************************************************
 *               TX functions
 **********************************************************/
void can__tx_msg(can__num_e can, can__msg_t *can_message_ptr) { can__tx(can, can_message_ptr, 0.5); }

void can__monitor_sensor_n_send_data(sensor_t dev) {
  can__msg_t msg = {};
  msg.frame_fields.is_29bit = 0;
  msg.frame_fields.data_len = 8;
  if (!dev) {
    can__check_switch_status(&msg);
  } else {
    can__check_tilt_sensor_status(&msg);
  }
  can__tx_msg(can1, &msg);
}

void can__check_switch_status(can__msg_t *msg) {
  msg->msg_id = 0x123;
  msg->frame_fields.is_29bit = 0;
  msg->frame_fields.data_len = 8;
  switch (get_button_status()) {
  case 0:
    msg->data.qword = 0x1122334400000000;
    puts("0x00 Sent\n");
    break;
  case 1:
    msg->data.qword = 0x1122334400000011;
    puts("0x11 Sent\n");
    break;
  case 2:
    msg->data.qword = 0x1122334400000022;
    puts("0x22 Sent\n");
    break;
  case 3:
    msg->data.qword = 0x1122334400000033;
    puts("0x33 Sent\n");
    break;
  default:
    msg->data.qword = 0x11223344000000AA;
    puts("0xAA Sent\n");
    break;
  }
}

void can__check_tilt_sensor_status(can__msg_t *msg) {
  msg->msg_id = 0x234;
  msg->frame_fields.is_29bit = 0;
  msg->frame_fields.data_len = 8;
  switch (tilt_sensor__get_data()) {
  case 0:
    msg->data.qword = 0x1122334400000000;
    puts("0x00 Sent\n");
    break;
  case 1:
    msg->data.qword = 0x1122334400000011;
    puts("0x11 Sent\n");
    break;
  case 2:
    msg->data.qword = 0x1122334400000022;
    puts("0x22 Sent\n");
    break;
  case 3:
    msg->data.qword = 0x1122334400000033;
    puts("0x33 Sent\n");
    break;
  default:
    msg->data.qword = 0x11223344000000AA;
    puts("0xAA Sent\n");
    break;
  }
}

uint8_t get_button_status(void) {
  if (gpio__get(switch_button0)) {
    return 0;
  } else if (gpio__get(switch_button1)) {
    return 1;
  } else if (gpio__get(switch_button2)) {
    return 2;
  } else if (gpio__get(switch_button3)) {
    return 3;
  } else {
    return 4;
  }
}

/**********************************************************
 *               RX functions
 **********************************************************/

void can__receive_data_n_control_led(void) {
  can__msg_t msg = {};
  while (can__rx(can1, &msg, 1)) {
  }
  if (0x123 == (0x00000000FFFFFFFF & msg.msg_id)) {
    can__control_led_for_switch(&msg);
  } else if (0x123 == (0x00000000FFFFFFFF & msg.msg_id)) {
    can__control_led_for_tilt_sensor(&msg);
  } else {
    puts("Wrong Msg Id");
  }
}

void can__control_led_for_tilt_sensor(can__msg_t *msg) {
  if (0x00 == (0x0000000000000000FF & msg->data.qword)) {
    gpio__reset(board_io__get_led0());
    gpio__set(board_io__get_led1());
    gpio__set(board_io__get_led2());
    gpio__set(board_io__get_led3());
  } else if (0x11 == (0x0000000000000000FF & msg->data.qword)) {
    gpio__set(board_io__get_led0());
    gpio__reset(board_io__get_led1());
    gpio__set(board_io__get_led2());
    gpio__set(board_io__get_led3());
  } else if (0x22 == (0x0000000000000000FF & msg->data.qword)) {
    gpio__set(board_io__get_led0());
    gpio__set(board_io__get_led1());
    gpio__reset(board_io__get_led2());
    gpio__set(board_io__get_led3());
  } else if (0x33 == (0x0000000000000000FF & msg->data.qword)) {
    gpio__set(board_io__get_led0());
    gpio__set(board_io__get_led1());
    gpio__set(board_io__get_led2());
    gpio__reset(board_io__get_led3());
  } else {
    gpio__set(board_io__get_led0());
    gpio__set(board_io__get_led1());
    gpio__set(board_io__get_led2());
    gpio__set(board_io__get_led3());
  }
}

void can__control_led_for_switch(can__msg_t *msg) {
  if (0x00 == (0x0000000000000000FF & msg->data.qword)) {
    gpio__reset(board_io__get_led0());
    gpio__set(board_io__get_led1());
    gpio__set(board_io__get_led2());
    gpio__set(board_io__get_led3());
  } else if (0x11 == (0x0000000000000000FF & msg->data.qword)) {
    gpio__set(board_io__get_led0());
    gpio__reset(board_io__get_led1());
    gpio__set(board_io__get_led2());
    gpio__set(board_io__get_led3());
  } else if (0x22 == (0x0000000000000000FF & msg->data.qword)) {
    gpio__set(board_io__get_led0());
    gpio__set(board_io__get_led1());
    gpio__reset(board_io__get_led2());
    gpio__set(board_io__get_led3());
  } else if (0x03 == (0x0000000000000000FF & msg->data.qword)) {
    gpio__set(board_io__get_led0());
    gpio__set(board_io__get_led1());
    gpio__set(board_io__get_led2());
    gpio__reset(board_io__get_led3());
  } else {
    puts("No Data Received for Switch\n");
  }
}