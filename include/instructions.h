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

    // CMP Compare Memory with Accumulator
    CMP_IMM = 0xC9,
    CMP_ZP = 0xC5,
    CMP_ZPX = 0xD5,
    CMP_ABS = 0xCD,
    CMP_ABSX = 0xDD,
    CMP_ABSY = 0xD9,
    CMP_INDX = 0xC1,
    CMP_INDY = 0xD1,

    // CMX Compare Memory and Index X
    CMX_IMM = 0xE0,
    CMX_ZP = 0xE4,
    CMX_ABS = 0xEC,

    // CMY Compare Memory and Index Y
    CMY_IMM = 0xC0,
    CMY_ZP = 0xC4,
    CMY_ABS = 0xCC,

    // DEC Decrement Memory by One
    DEC_ZP = 0xC6,
    DEC_ZPX = 0xD6,
    DEC_ABS = 0xCE,
    DEC_ABSX = 0xDE,

    // DEX Decrement Index X by One
    DEX = 0xCA,

    // DEY Decrement Index Y by One
    DEY = 0x88,

    // EOR Exclusive-OR Memory with Accumulator
    EOR_IMM = 0x49,
    EOR_ZP = 0x45,
    EOR_ZPX = 0x55,
    EOR_ABS = 0x4D,
    EOR_ABSX = 0x5D,
    EOR_ABSY = 0x59,
    EOR_INDX = 0x41,
    EOR_INDY = 0x51,

    // INC Increment Memory by One
    INC_ZP = 0xE6,
    INC_ZPX = 0xF6,
    INC_ABS = 0xEE,
    INC_ABSX = 0xFE,

    // INX Increment Index X by One
    INX = 0xE8,

    // INY Increment Index Y by One
    INY = 0xC8,

    // JMP Jump to New Location
    JMP_ABS = 0x4C,
    JMP_IND = 0x6C,

    // JSR Jump to New Location Saving Return Address
    // JSR = 0x20,

    // LDA Load Accumulator with Memory
    LDA_IMM = 0xA9,
    LDA_ZP = 0xA5,
    LDA_ZPX = 0xB5,
    LDA_ABS = 0xAD,
    LDA_ABSX = 0xBD,
    LDA_ABSY = 0xB9,
    LDA_INDX = 0xA1,
    LDA_INDY = 0xB1,

    // LDX Load Index X with Memory
    LDX_IMM = 0xA2,
    LDX_ZP = 0xA6,
    LDX_ZPY = 0xB6,
    LDX_ABS = 0xAE,
    LDX_ABSY = 0xBE,

    // LDY Load Index Y with Memory
    LDY_IMM = 0xA0,
    LDY_ZP = 0xA4,
    LDY_ZPX = 0xB4,
    LDY_ABS = 0xAC,
    LDY_ABSX = 0xBC,

    // LSR Shift One Bit Right (Memory or Accumulator)
    LSR_ACC = 0x4A,
    LSR_ZP = 0x46,
    LSR_ZPX = 0x56,
    LSR_ABS = 0x4E,
    LSR_ABSX = 0x5E,

    // NOP No Operation
    NOP = 0xEA,

    // ORA OR Memory with Accumulator
    ORA_IMM = 0x09,
    ORA_ZP = 0x05,
    ORA_ZPX = 0x15,
    ORA_ABS = 0x0D,
    ORA_ABSX = 0x1D,
    ORA_ABSY = 0x19,
    ORA_INDX = 0x01,
    ORA_INDY = 0x11,

    // PHA Push Accumulator on Stack
    PHA = 0x48,

    // PHP Push Processor Status on Stack
    PHP = 0x08,

    // PLA Pull Accumulator from Stack
    PLA = 0x68,

    // PLP Pull Processor Status from Stack
    PLP = 0x28,

    // ROL Rotate One Bit Left (Memory or Accumulator)
    // ROL_ACC = 0x6A,
    // ROL_ZP = 0x66,
    // ROL_ZPX = 0x76,
    // ROL_ABS = 0x6E,
    // ROL_ABSX = 0x7E,

    // RTI Return from Interrupt
    // RTI = 0x40,

    // RTS Return from Subroutine
    // RTS = 0x60,
    
    // SBC Subtract Memory from Accumulator with Borrow
    // SBC_IMM = 0xE9,
    // SBC_ZP = 0xE5,
    // SBC_ZPX = 0xF5,
    // SBC_ABS = 0xED,
    // SBC_ABSX = 0xFD,
    // SBC_ABSY = 0xF9,
    // SBC_INDX = 0xE1,
    // SBC_INDY = 0xF1,

    // SEC Set Carry Flag
    // SEC = 0x38,

    // SED Set Decimal Flag
    // SED = 0xF8,

    // SEI Set Interrupt Disable Status
    // SEI = 0x78,

    // STA Load Accumulator with Memory
    // STA_ZP = 0x85,
    // STA_ZPX = 0x95,
    // STA_ABS = 0x8D,
    // STA_ABSX = 0x9D,
    // STA_ABSY = 0x99,
    // STA_INDX = 0x81,
    // STA_INDY = 0x91,

    // STX Load Index X with Memory
    // STX_ZP = 0x86,
    // STX_ZPY = 0x96,
    // STX_ABS = 0x8E,

    // LDY Load Index Y with Memory
    // LDY_ZP = 0x84,
    // LDY_ZPX = 0x94,
    // LDY_ABS = 0x8C,

    // TAX Transfer Accumulator to Index X
    // TAX = 0xAA,
    
    // TAY Transfer Accumulator to Index Y
    // TAY = 0xA8,

    // TSX Transfer Stack Pointer to Index X
    // TSX = 0xBA,

    // TXA Transfer Index X to Accumulator
    // TXA = 0x8A,

    // TXS Transfer Index X to Stack Register
    // TXS = 0x9A,

    // TYA Transfer Index Y to Accumulator
    // TYA = 0x98,
};

void initialize_map(std::unordered_map<uint8_t, inst_func_t>&);
std::string ToString(Instruction);