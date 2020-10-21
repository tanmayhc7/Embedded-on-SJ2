#pragma once
#include "can_bus.h"
void can__init_can_bus(can__num_e can, uint32_t baudrate_kbps, uint16_t rxq_size, uint16_t txq_size,
                       can_void_func_t bus_off_cb, can_void_func_t data_ovr_cb);