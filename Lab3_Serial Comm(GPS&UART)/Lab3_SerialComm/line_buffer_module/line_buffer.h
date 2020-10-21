#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
  char *memory;
  size_t max_size;
  size_t no_of_strings_in_buffer;
  size_t write_index;
} line_buffer_s;

// Initialize *line_buffer_s with the user provided buffer space and size
void line_buffer__init(line_buffer_s *buffer, char *memory, size_t size);

// Adds a byte to the buffer, and returns true if the buffer had enough space to add the byte
bool line_buffer__add_byte(line_buffer_s *buffer, char byte);

/**
 * If the line buffer has a complete line, it will remove that contents and save it to "char * line"
 * @param line_max_size This is the max size of line
 */
bool line_buffer__remove_line(line_buffer_s *buffer, char *line, size_t line_max_size);