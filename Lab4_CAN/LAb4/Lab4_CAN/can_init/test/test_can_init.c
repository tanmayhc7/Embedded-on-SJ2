#include "unity.h"
#include <stdlib.h>
#include <string.h>

#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockgpio.h"

#include "can_initializer.h"

void setUp(void) {}

void tearDown(void) {}

void test_can__init(void) {
  can__init_ExpectAndReturn(can1, 100, 100, 100, NULL, NULL, true);
  can__bypass_filter_accept_all_msgs_Expect();
  can__reset_bus_Expect(can1);
  can__init_can_bus(can1, 100, 100, 100, NULL, NULL);
}

void test_can__init_gpio(void) {
  gpio_s switch_button[4];
  gpio__construct_as_input_ExpectAndReturn(GPIO__PORT_0, 29, switch_button[0]);
  gpio__construct_as_input_ExpectAndReturn(GPIO__PORT_0, 30, switch_button[1]);
  gpio__construct_as_input_ExpectAndReturn(GPIO__PORT_1, 15, switch_button[2]);
  gpio__construct_as_input_ExpectAndReturn(GPIO__PORT_1, 19, switch_button[3]);
  can__init_gpio();
}