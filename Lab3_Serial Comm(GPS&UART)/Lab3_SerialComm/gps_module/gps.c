// gps.c
#include "gps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "sl_unit_test_facilitator.h"

// Rtos dependency for the UART driver
#include "FreeRTOS.h"
#include "queue.h"

#include "clock.h" // needed for UART initialization

#include "gpio.h"

// GPS module dependency
#include "line_buffer.h"
#include "uart.h"

// Change this according to which UART you plan to use
static uart_e gps_uart = UART__3;

static gpio_s rxd, txd;

// Space for the line buffer, and the line buffer data structure instance
static char line_buffer[256];
static line_buffer_s line;

static gps_coordinates_t parsed_coordinates;

static void gps__absorb_data(void) {
  char byte;
  while (uart__get(gps_uart, &byte, 0)) {
    line_buffer__add_byte(&line, byte);
  }
}

static void gps__handle_line(void) {
  char gps_line[80];
  if (line_buffer__remove_line(&line, gps_line, sizeof(gps_line))) {
    // TODO: Parse the line to store GPS coordinates etc.
    // TODO: parse and store to parsed_coordinates
    char *token = strtok(gps_line, ",");
    char lat[10];
    for (int i = 0; token != NULL; i++) {
      /* if (i == 2) {
        parsed_coordinates.latitude = strtof(token, NULL);
      }
      if (i == 4) {
        parsed_coordinates.longitude = strtof(token, NULL);
      } */
      if (i == 2) {
        memcpy(&lat, token, strlen(token));
        parsed_coordinates.latitude = atof(lat);
      }
      if (i == 4) {
        memcpy(&lat, token, strlen(token));
        parsed_coordinates.longitude = atof(lat);
      }
      token = strtok(NULL, ",");
    }
  }
}

void gps__init(void) {
  line_buffer__init(&line, line_buffer, sizeof(line_buffer));
  uart__init(gps_uart, clock__get_peripheral_clock_hz(), 38400);
  txd = gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2); // P4.28 as TXD3
  rxd = gpio__construct_with_function(GPIO__PORT_4, 29, GPIO__FUNCTION_2); // P4.29 as RXD3

  // RX queue should be sized such that can buffer data in UART driver until gps__run_once() is called
  // Note: Assuming 38400bps, we can get 4 chars per ms, and 40 chars per 10ms (100Hz)
  QueueHandle_t rxq_handle = xQueueCreate(100, sizeof(char));
  QueueHandle_t txq_handle = xQueueCreate(8, sizeof(char)); // We don't send anything to the GPS
  uart__enable_queues(gps_uart, rxq_handle, txq_handle);
}

/// Public functions:
void gps__run_once(void) {
  gps__absorb_data();
  gps__handle_line();
}

gps_coordinates_t gps__get_coordinates(void) {
  // TODO return parsed_coordinates
  return parsed_coordinates;
}