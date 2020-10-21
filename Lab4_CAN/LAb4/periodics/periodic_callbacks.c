#include "periodic_callbacks.h"
#include "board_io.h"
#include "can_initializer.h"
#include "can_msg_handler.h"
#include "gpio.h"
#include "tilt_sensor.h"
#include <stddef.h>

#define CAN_TEST 1
#ifdef CAN_TEST
#define CAN_TX 1
#endif

#define DEVICE_ID 1

/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */

void periodic_callbacks__initialize(void) {
  if (CAN_TEST) {
    can__init_can_bus(can1, 100, 100, 100, NULL, NULL);
    can__init_gpio();
    tilt_sensor__init();
  }
}

void periodic_callbacks__1Hz(uint32_t callback_count) {}

void periodic_callbacks__10Hz(uint32_t callback_count) {
  if (CAN_TEST) {
    if (CAN_TX) {
      can__monitor_sensor_n_send_data(DEVICE_ID);
    } else {
      can__receive_data_n_control_led();
    }
  }
}
void periodic_callbacks__100Hz(uint32_t callback_count) {}

void periodic_callbacks__1000Hz(uint32_t callback_count) {}