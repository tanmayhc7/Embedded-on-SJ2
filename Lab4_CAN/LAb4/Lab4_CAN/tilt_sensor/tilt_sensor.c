#include "tilt_sensor.h"
#include <stdio.h>

acceleration__axis_data_s axis_data;

void tilt_sensor__init(void) {
  if (acceleration__init()) {
    puts("Tilt Sensor initialized");
  } else {
    puts("Tilt Sensor NOT initialized");
  }
}

uint8_t tilt_sensor__get_data(void) {
  axis_data = acceleration__get_data();
  uint16_t var = axis_data.y;
  /* char buffer[100];
  sprintf(buffer, "Value: %d\n", var);
  puts(buffer); */
  if (var > 500 && var < 1000) {
    return 3;
  } else if (var > 200 && var < 450) {
    return 2;
  } else if (var > 3000 && var < 3600) {
    return 0;
  } else if (var > 3650 && var < 4100) {
    return 1;
  } else {
    return 4;
  }
  return axis_data.y;
}
