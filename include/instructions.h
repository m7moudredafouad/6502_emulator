#pragma once

#include <CPU.h>

void initialize_map(std::unordered_map<uint8_t, inst_func_t>&);

void LDA_INST(CPU& cpu, uint8_t op_code, std::string name);