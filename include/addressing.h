#pragma once
#include <CPU.h>

uint16_t ADDR_IMM(CPU& cpu);
uint16_t ADDR_ZP(CPU& cpu);
uint16_t ADDR_ZPX(CPU& cpu);
uint16_t ADDR_ZPY(CPU& cpu);
uint16_t ADDR_ABS(CPU& cpu);
uint16_t ADDR_ABSX(CPU& cpu, bool force_cycle = false);
uint16_t ADDR_ABSY(CPU& cpu, bool force_cycle = false);
uint16_t ADDR_IND(CPU& cpu);
uint16_t ADDR_INDX(CPU& cpu);
uint16_t ADDR_INDY(CPU& cpu, bool force_cycle = false);