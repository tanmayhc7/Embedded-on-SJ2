#include "periodic_callbacks.h"
#include "board_io.h"
#include "fake_gps.h"
#include "gpio.h"
#include "gps.h"
#include "switch_logic.h"
#include "task.h"
#include <stdio.h>
/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */
#define GPS_UART 0
#ifdef GPS_UART
#define SEND 0
#endif


void periodic_callbacks__initialize(void) {
  if (GPS_UART) {
    if (SEND) {
      fake_gps__init();
    } else {
      gps__init();
    }
  }
}

void periodic_callbacks__1Hz(uint32_t callback_count) {
    if (GPS_UART) {
      if (SEND) {
        fake_gps__run_once();
      } else {
        gps__run_once();
        gps_coordinates_t temp_coordinates;
        temp_coordinates = gps__get_coordinates();
        char buffer[100];
        sprintf(buffer, "Latitude: %4.3f  Longitude: %5.4f \n", temp_coordinates.latitude, temp_coordinates.longitude);
        puts(buffer);
      }
    }
}

void periodic_callbacks__10Hz(uint32_t callback_count) {
  
}
void periodic_callbacks__100Hz(uint32_t callback_count) { gpio__set(board_io__get_led2()); }

void periodic_callbacks__1000Hz(uint32_t callback_count) { gpio__reset(board_io__get_led3()); }
