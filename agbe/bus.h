﻿#pragma once

#include <cstdint>

uint8_t bus_read(uint16_t address);
void bus_write(uint16_t address, uint8_t value);