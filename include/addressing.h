#pragma once
#include <CPU.h>

uint16_t ImmediateAddress(CPU& cpu);
uint16_t ZeroPageAddress(CPU& cpu);
uint16_t ZeroPageXAddress(CPU& cpu);
uint16_t ZeroPageYAddress(CPU& cpu);
uint16_t AbsoluteAddress(CPU& cpu);
uint16_t AbsoluteXAddress(CPU& cpu);
uint16_t AbsoluteYAddress(CPU& cpu);
uint16_t IndexedIndirectAddress(CPU& cpu);
uint16_t IndirectIndexedAddress(CPU& cpu);