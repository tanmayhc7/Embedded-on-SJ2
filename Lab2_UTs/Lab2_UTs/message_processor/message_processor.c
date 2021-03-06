#include "message_processor.h"
static bool message_processor_third_message_has_dollar_or_pound_sign(message_s *message);
/**
 * This processes messages by calling message__read() until:
 *   - There are no messages to process -- which happens when message__read() returns false
 *   - At most 3 messages have been read
 */
bool message_processor(void) {
  bool symbol_found = false;
  message_s message;
  memset(&message, 0, sizeof(message));

  const static size_t max_messages_to_process = 3;
  for (size_t message_count = 0; message_count < max_messages_to_process; message_count++) {
    if (!message__read(&message)) {
      break;
    } else if (message_count < 2) {
      continue;
    } else {
      symbol_found = message_processor_third_message_has_dollar_or_pound_sign(&message);
    }
  }

  return symbol_found;
}

static bool message_processor_third_message_has_dollar_or_pound_sign(message_s *message) {
  bool symbol_found = false;
  if (message->data[0] == '$') {
    message->data[1] = '^';
    symbol_found = true;
  } else if (message->data[0] == '#') {
    message->data[1] = '%';
    symbol_found = true;
  } else {
    symbol_found = false;
  }
  return symbol_found;
}
