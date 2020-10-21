#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockboard_io.h"
#include "Mockcan_initializer.h"
#include "Mockcan_message_handler.h"
#include "Mockgpio.h"


// Include the source we wish to test
#include "periodic_callbacks.h"

#define CAN_TEST 1
#ifdef CAN_TEST
#define CAN_TX 1
#endif


void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  if (CAN_TEST) {
    can__init_can_bus_Expect(can1, 100, 100, 100, NULL, NULL);
    can__init_gpio_Expect();
  }
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
  if (CAN_TEST) {
    if (CAN_TX) {
      can__check_switch_n_send_Expect();
    } else {
      can__receive_data_n_control_led_Expect();
    }
  }
  periodic_callbacks__1Hz(0);
}

void test__periodic_callbacks__10Hz(void) {
  
  periodic_callbacks__10Hz(10);
}

void test__periodic_callbacks__100Hz(void) {
  periodic_callbacks__100Hz(0);
}

void test__periodic_callbacks__1000Hz(void) {
  periodic_callbacks__1000Hz(0);
}
