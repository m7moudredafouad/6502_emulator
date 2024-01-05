#pragma once

#include <CPU.h>

enum Instruction : uint8_t {
    // ADC Add Memory to Accumulator with Carry
    ADC_IMM = 0x69,
    ADC_ZP = 0x65,
    ADC_ZPX = 0x75,
    ADC_ABS = 0x6D,
    ADC_ABSX = 0x7D,
    ADC_ABSY = 0x79,
    ADC_INDX = 0x61,
    ADC_INDY = 0x71,

    // AND AND Memory with Accumulator
    AND_IMM = 0x29,
    AND_ZP = 0x25,
    AND_ZPX = 0x35,
    AND_ABS = 0x2D,
    AND_ABSX = 0x3D,
    AND_ABSY = 0x39,
    AND_INDX = 0x21,
    AND_INDY = 0x31,

    // ASL Shift Left One Bit (Memory or Accumulator)
    ASL_ACC = 0x0A,
    ASL_ZP = 0x06,
    ASL_ZPX = 0x16,
    ASL_ABS = 0x0E,
    ASL_ABSX = 0x1E,

    // BCC Branch on Carry Clear
    BCC = 0x90,

    // BCS Branch on Carry Set
    BCS = 0xB0,

    // BEQ Branch on Result Zero
    BEQ = 0xF0,

    // BIT Test Bits in Memory with Accumulator
    BIT_ZP = 0x24,
    BIT_ABS = 0x2C,

    // BMI Branch on Result Minus
    BMI = 0x30,

    // BNE Branch on Result not Zero
    BNE = 0xD0,

    // BPL Branch on Result Plus
    BPL = 0x10,

    // BRK Force Break
    BRK = 0x00,

    // BVC Branch on Overflow Clear
    BVC = 0x50,

    // BVS Branch on Overflow Set
    BVS = 0x70,

    // Clear Status
    CLC = 0x18,
    CLD = 0xD8,
    CLI = 0x58,
    CLV = 0xB8,

    // NOP No Operation
    NOP = 0xEA,

    LDA_IMM = 0xA9,
    LDA_ZP = 0xA5,
    LDA_ZPX = 0xB5,
    LDA_ABS = 0xAD,
    LDA_ABSX = 0xBD,
    LDA_ABSY = 0xB9,
    LDA_INDX = 0xA1,
    LDA_INDY = 0xB1,

    LDX_IMM = 0xA2,
    LDX_ZP = 0xA6,
    LDX_ZPY = 0xB6,
    LDX_ABS = 0xAE,
    LDX_ABSY = 0xBE,

    LDY_IMM = 0xA0,
    LDY_ZP = 0xA4,
    LDY_ZPX = 0xB4,
    LDY_ABS = 0xAC,
    LDY_ABSX = 0xBC,
};

void initialize_map(std::unordered_map<uint8_t, inst_func_t>&);
std::string ToString(Instruction);