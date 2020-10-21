#include "unity.h"
#include <stdlib.h>
#include <string.h>

// Include the source we wish to test
#include "line_buffer.h"

static line_buffer_s line_buffer;
static char memory[256];

void setUp(void) { line_buffer__init(&line_buffer, memory, sizeof(memory)); }

void tearDown(void) { memset(&memory, 0, sizeof(memory)); }

void test_line_buffer__init(void) {
  TEST_ASSERT_EQUAL_UINT32(&memory, line_buffer.memory);
  TEST_ASSERT_EQUAL_UINT(sizeof(memory), line_buffer.max_size);
  TEST_ASSERT_EQUAL_UINT(0, line_buffer.write_index);
  TEST_ASSERT_EQUAL_UINT(0, line_buffer.no_of_strings_in_buffer);
}

void test_line_buffer__nominal_case(void) {
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'a'));
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'b'));
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'c'));
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, '\n'));

  char line[8];
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "abc");
}

void test_line_buffer__slash_r_slash_n_case(void) {
  line_buffer__add_byte(&line_buffer, 'a');
  line_buffer__add_byte(&line_buffer, 'b');
  line_buffer__add_byte(&line_buffer, '\r');
  line_buffer__add_byte(&line_buffer, '\n');

  char line[8];
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "ab\r");
}

// Line buffer should be able to add multiple lines and we should be able to remove them one at a time
void test_line_buffer__multiple_lines(void) {
  line_buffer__add_byte(&line_buffer, 'a');
  line_buffer__add_byte(&line_buffer, 'b');
  line_buffer__add_byte(&line_buffer, '\n');
  line_buffer__add_byte(&line_buffer, 'c');
  line_buffer__add_byte(&line_buffer, 'd');
  line_buffer__add_byte(&line_buffer, '\n');
  line_buffer__add_byte(&line_buffer, 'e');
  line_buffer__add_byte(&line_buffer, 'f');
  line_buffer__add_byte(&line_buffer, '\n');

  char line[8];
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "ab");

  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "cd");

  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "ef");
}

void test_line_buffer__overflow_case(void) {
  // Add chars until full capacity
  for (size_t i = 0; i < sizeof(memory) - 1; i++) {
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, 'a'));
  }
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, '\n'));
  // Buffer should be full now
  TEST_ASSERT_FALSE(line_buffer__add_byte(&line_buffer, 'b'));

  // Retreive truncated output (without the newline char)
  // Do not modify this test; instead, change your API to make this test pass
  char line[8];
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(line, "aaaaaaa");
}

void test_gps_string_byte_by_byte(void) {
  const char gps_str[] = "$GPGGA,230612.015,3907.3815,N,12102.4634,W,0,04,5.7,508.3,M,,,,0000*13";
  for (size_t i = 0; i < strlen(gps_str); i++) {
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, gps_str[i]));
  }
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, '\n'));
  char line[200];
  memset(&line, 0, sizeof(line));
  TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, &line, sizeof(line)));
  TEST_ASSERT_EQUAL_STRING(gps_str, line);
}

void test_push_multiple_gps_strings_byte_by_byte_n_retrieve(void) {
  const char gps_str[] = "$GPGGA,230612.015,3907.3815,N,12102.4634,W,0,04,5.7,508.3,M,,,,0000*13";
  for (size_t i = 0; i < strlen(gps_str); i++) {
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, gps_str[i]));
  }
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, '\n'));
  for (size_t i = 0; i < strlen(gps_str); i++) {
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, gps_str[i]));
  }
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, '\n'));
  for (size_t i = 0; i < strlen(gps_str); i++) {
    TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, gps_str[i]));
  }
  TEST_ASSERT_TRUE(line_buffer__add_byte(&line_buffer, '\n'));
  char line[100];
  memset(&line, 0, sizeof(line));
  for (int i = 0; i < 3; i++) {
    TEST_ASSERT_TRUE(line_buffer__remove_line(&line_buffer, &line, sizeof(line)));
    TEST_ASSERT_EQUAL_STRING(gps_str, line);
  }
}

void test_pull_line_(void) {
  char line[8];
  TEST_ASSERT_FALSE(line_buffer__remove_line(&line_buffer, line, sizeof(line)));
}