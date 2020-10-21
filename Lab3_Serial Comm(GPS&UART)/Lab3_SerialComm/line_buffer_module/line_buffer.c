#include "line_buffer.h"
#include <string.h>

// Initialize *line_buffer_s with the user provided buffer space and size
void line_buffer__init(line_buffer_s *buffer, char *memory, size_t size) {
  buffer->memory = memory;
  buffer->max_size = size;
  buffer->write_index = 0;
  buffer->no_of_strings_in_buffer = 0;
}

// Adds a byte to the buffer, and returns true if the buffer had enough space to add the byte
bool line_buffer__add_byte(line_buffer_s *buffer, char byte) {
  if (buffer->write_index == buffer->max_size) {
    return 0;
  } else {
    if ('\n' == byte || '\0' == byte) {
      buffer->no_of_strings_in_buffer++;
    }
    buffer->memory[buffer->write_index++] = byte;
    return 1;
  }
}

/**
 * If the line buffer has a complete line, it will remove that contents and save it to "char * line"
 * @param line_max_size This is the max size of line
 */
bool line_buffer__remove_line(line_buffer_s *buffer, char *line, size_t line_max_size) {
  if (buffer->no_of_strings_in_buffer > 0) {
    size_t new_line_pos = 0;
    while (buffer->memory[new_line_pos] != '\n') {
      new_line_pos++;
    }
    if (new_line_pos > line_max_size) {
      memcpy(line, &buffer->memory[0], line_max_size - 1);
      return 1;
    }
    memcpy(line, &buffer->memory[0], new_line_pos);
    buffer->no_of_strings_in_buffer--;
    if (buffer->no_of_strings_in_buffer > 0) {
      memmove(&buffer->memory[0], &buffer->memory[new_line_pos + 1], buffer->write_index);
      buffer->write_index -= new_line_pos - 1;
    } else {
      buffer->write_index = 0;
    }
    return 1;
  } else {
    return 0;
  }
}