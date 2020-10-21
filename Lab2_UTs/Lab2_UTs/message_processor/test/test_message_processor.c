#include "unity.h"

#include "Mockmessage.h"

#include "message_processor.c"

static bool get_test_message_with_dollar(message_s *message, int callback_count);
static bool get_test_message_with_hash(message_s *message, int callback_count);
static bool get_test_message_with_no_sign(message_s *message, int callback_count);

void setUp(void) {}

void tearDown(void) {}

// This only tests if we process at most 3 messages
void test_process_at_most_3_messages(void) {
  for (int i = 0; i < 3; i++) {
    message__read_ExpectAndReturn(NULL, true);
    message__read_IgnoreArg_message_to_read();
  }
  // Since we did not return a message that starts with '$' this should return false
  TEST_ASSERT_FALSE(message_processor());
}

void test_process_message_with_dollar_sign(void) {
  message_s message = {"$cmpe243"};
  TEST_ASSERT_TRUE(message_processor_third_message_has_dollar_or_pound_sign(&message));
}

void test_process_message_with_hash_sign(void) {
  message_s message = {"#cmpe243"};
  TEST_ASSERT_TRUE(message_processor_third_message_has_dollar_or_pound_sign(&message));
}

void test_process_messages_without_any_dollar_sign(void) {
  message_s message = {"cmpe243"};
  TEST_ASSERT_FALSE(message_processor_third_message_has_dollar_or_pound_sign(&message));
}

void test_process_messages_with_callbacks(void) {
  message__read_StubWithCallback(get_test_message_with_dollar);
  TEST_ASSERT_TRUE(message_processor());

  message__read_StubWithCallback(get_test_message_with_hash);
  TEST_ASSERT_TRUE(message_processor());

  message__read_StubWithCallback(get_test_message_with_no_sign);
  TEST_ASSERT_FALSE(message_processor());
}

void test_process_messages_return_thru_ptr(void) {
  message_s message = {"$1000"};
  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&message);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&message);

  message__read_ExpectAndReturn(NULL, true);
  message__read_IgnoreArg_message_to_read();
  message__read_ReturnThruPtr_message_to_read(&message);

  TEST_ASSERT_TRUE(message_processor());
}

static bool get_test_message_with_dollar(message_s *message, int callback_count) {
  if (callback_count <= 2) {
    message->data[0] = '$';
    return 1;
  } else {
    TEST_FAIL();
  }
}

static bool get_test_message_with_hash(message_s *message, int callback_count) {
  if (callback_count <= 6) {
    message->data[0] = '#';
    return 1;
  } else {
    TEST_FAIL();
  }
}

static bool get_test_message_with_no_sign(message_s *message, int callback_count) {
  if (callback_count <= 9) {
    message->data[0] = '7';
    return 1;
  } else {
    TEST_FAIL();
  }
}
