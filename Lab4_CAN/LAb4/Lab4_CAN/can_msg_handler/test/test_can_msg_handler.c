#include <stdio.h>
#include <string.h>

#include "unity.h"

#include "Mockboard_io.h"
#include "Mockcan_initializer.h"
#include "Mocktilt_sensor.h"

#include "can_msg_handler.h"

void test_can__send_bytes(void) {
  can__msg_t msg = {0};
  msg.msg_id = 0x123;
  msg.frame_fields.data_len = 8;
  msg.data.qword = 0xAA;
  can__tx_msg(can1, &msg);
  can__monitor_sensor_n_send_data(0);
}
