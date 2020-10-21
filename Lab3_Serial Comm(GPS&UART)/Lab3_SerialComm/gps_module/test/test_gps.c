#include "unity.h"
#include <stdbool.h>

// Mocks
#include "Mockclock.h"
#include "Mockgpio.h"
#include "Mockqueue.h"
#include "Mockuart.h"
// Use the real implementation (not mocks) for:
#include "line_buffer.h"

// Include the source we wish to test
#include "gps.c"

void setUp(void) { line_buffer__init(&line, line_buffer, sizeof(line_buffer)); }
void tearDown(void) { memset(&line_buffer, 0, sizeof(line_buffer)); }

static bool stub_for_uart_get(uart_e uart, char *input_byte, uint32_t timeout_ms, int cmock_num_calls);

void test_gps_init(void) {
  TEST_ASSERT_EQUAL_UINT32(&line_buffer, line.memory);
  TEST_ASSERT_EQUAL_UINT(sizeof(line_buffer), line.max_size);
  TEST_ASSERT_EQUAL_UINT(0, line.write_index);
  TEST_ASSERT_EQUAL_UINT(0, line.no_of_strings_in_buffer);
  clock__get_peripheral_clock_hz_ExpectAndReturn(96000000);
  uart__init_Expect(UART__3, 96000000, 38400);
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_4, 28, GPIO__FUNCTION_2, txd);
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_4, 29, GPIO__FUNCTION_2, rxd);
  QueueHandle_t rxq, txq;
  xQueueCreate_ExpectAndReturn(100, sizeof(char), rxq);
  xQueueCreate_ExpectAndReturn(8, sizeof(char), txq);
  uart__enable_queues_ExpectAndReturn(gps_uart, rxq, txq, true);
  gps__init();
}

void test_gps_multiple_strings(void) {
  const char gps_str[] =
      "$GPGGA,230612,3907.381,N,12102.4634,W,0,04,5.7,508.3,M,,,,0000*13\n$GPGGA,230612,4000.000,N,"
      "20000.4634,W,0,04,5.7,508.3,M,,,,0000*13\n$GPGGA,230612,3907.381,N,12102.4634,W,0,04,5.7,508.3,M,,,,0000*13";

  for (size_t i = 0; i < strlen(gps_str); i++) {
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line, gps_str[i]));
  }
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line, '\n'));
  uart__get_IgnoreAndReturn(false);
  gps__run_once();
  TEST_ASSERT_EQUAL_FLOAT(3907.381, parsed_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(12102.4634, parsed_coordinates.longitude);
  gps__run_once();
  TEST_ASSERT_EQUAL_FLOAT(4000.000, parsed_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(20000.4634, parsed_coordinates.longitude);
  gps__run_once();
  TEST_ASSERT_EQUAL_FLOAT(3907.381, parsed_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(12102.4634, parsed_coordinates.longitude);
}

void test_gps_absorb_data(void) {
  uart__get_StubWithCallback(stub_for_uart_get);
  gps__run_once();
  TEST_ASSERT_EQUAL_FLOAT(3907.381, parsed_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(12102.4634, parsed_coordinates.longitude);
  gps__run_once();
  TEST_ASSERT_EQUAL_FLOAT(4000.000, parsed_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(20000.4634, parsed_coordinates.longitude);
  gps__run_once();
  TEST_ASSERT_EQUAL_FLOAT(3907.381, parsed_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(12102.4634, parsed_coordinates.longitude);
}

static bool stub_for_uart_get(uart_e uart, char *input_byte, uint32_t timeout_ms, int cmock_num_calls) {
  const char gps_str[] =
      "$GPGGA,230612,3907.381,N,12102.4634,W,0,04,5.7,508.3,M,,,,0000*13\n$GPGGA,230612,4000.000,N,"
      "20000.4634,W,0,04,5.7,508.3,M,,,,0000*13\n$GPGGA,230612,3907.381,N,12102.4634,W,0,04,5.7,508.3,M,,,,0000*13\n";
  if (cmock_num_calls <= strlen(gps_str)) {
    *input_byte = gps_str[cmock_num_calls];
    return 1;
  } else {
    return 0;
  }
}