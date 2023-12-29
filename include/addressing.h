#pragma once
#include <CPU.h>

uint16_t swap_bytes(uint16_t value);
uint8_t ZeroPageAddress(CPU& cpu);
uint8_t ZeroPageXAddress(CPU& cpu);
uint8_t ZeroPageYAddress(CPU& cpu);
uint8_t AbsoluteAddress(CPU& cpu);
uint8_t AbsoluteXAddress(CPU& cpu);
uint8_t AbsoluteYAddress(CPU& cpu);
uint8_t IndexedIndirectAddress(CPU& cpu);
uint8_t IndirectIndexedAddress(CPU& cpu);