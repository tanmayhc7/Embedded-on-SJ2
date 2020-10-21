#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockboard_io.h"
#include "Mockfake_gps.h"
#include "Mockgpio.h"
#include "Mockgps.h"

// Include the source we wish to test
#include "periodic_callbacks.h"
#define GPS_UART 0
#ifdef GPS_UART
#define SEND 0
#endif


void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  if (GPS_UART) {
    if (SEND) {
      fake_gps__init_Expect();
    } else {
      gps__init_Expect();
    }
  }
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
    if (GPS_UART) {
      if (SEND) {
        fake_gps__run_once_Expect();
      } else {
        gps__run_once_Expect();
        gps_coordinates_t temp_coordinates;
        gps__get_coordinates_ExpectAndReturn(temp_coordinates);
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
