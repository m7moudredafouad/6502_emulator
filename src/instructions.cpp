#include <addressing.h>
#include <instructions.h>
#include <iostream>

#define SIGN_BIT(value) (((value)&0x80) > 0)

void INST_ADC(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::ADC_IMM: {
        address = ImmediateAddress(cpu);
    } break;
    case Instruction::ADC_ZP: {
        address = ZeroPageAddress(cpu);
    } break;
    case Instruction::ADC_ZPX: {
        address = ZeroPageXAddress(cpu);
    } break;
    case Instruction::ADC_ABS: {
        address = AbsoluteAddress(cpu);
    } break;
    case Instruction::ADC_ABSX: {
        address = AbsoluteXAddress(cpu);
    } break;
    case Instruction::ADC_ABSY: {
        address = AbsoluteYAddress(cpu);
    } break;
    case Instruction::ADC_INDX: {
        address = IndexedIndirectAddress(cpu);
    } break;
    case Instruction::ADC_INDY: {
        address = IndirectIndexedAddress(cpu);
    } break;
    default:
        ASSERT(0,
               "Unknown op_code: 0x" << std::hex << int(op_code) << std::dec);
    }

    uint8_t operand = cpu.mem_read(address);
    uint8_t ac = cpu.AC;
    uint16_t val = operand + cpu.AC + cpu.SR.C;
    cpu.AC = (val & 0xFF);

    cpu.SR.N = SIGN_BIT(cpu.AC);
    cpu.SR.Z = cpu.AC == 0;
    cpu.SR.C = ((val >> 8) == 1);
    cpu.SR.V = 0;
    if ((SIGN_BIT(ac) == SIGN_BIT(operand))) {
        cpu.SR.V = (SIGN_BIT(ac) != SIGN_BIT(cpu.AC));
    }
}

void INST_AND(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::AND_IMM: {
        address = ImmediateAddress(cpu);
    } break;
    case Instruction::AND_ZP: {
        address = ZeroPageAddress(cpu);
    } break;
    case Instruction::AND_ZPX: {
        address = ZeroPageXAddress(cpu);
    } break;
    case Instruction::AND_ABS: {
        address = AbsoluteAddress(cpu);
    } break;
    case Instruction::AND_ABSX: {
        address = AbsoluteXAddress(cpu);
    } break;
    case Instruction::AND_ABSY: {
        address = AbsoluteYAddress(cpu);
    } break;
    case Instruction::AND_INDX: {
        address = IndexedIndirectAddress(cpu);
    } break;
    case Instruction::AND_INDY: {
        address = IndirectIndexedAddress(cpu);
    } break;
    default:
        ASSERT(0,
               "Unknown op_code: 0x" << std::hex << int(op_code) << std::dec);
    }

    cpu.AC &= cpu.mem_read(address);

    cpu.SR.N = SIGN_BIT(cpu.AC);
    cpu.SR.Z = cpu.AC == 0;
}

void INST_ASL(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::ASL_ACC:
        break;
    case Instruction::ASL_ZP: {
        address = ZeroPageAddress(cpu);
    } break;
    case Instruction::ASL_ZPX: {
        address = ZeroPageXAddress(cpu);
    } break;
    case Instruction::ASL_ABS: {
        address = AbsoluteAddress(cpu);
    } break;
    case Instruction::ASL_ABSX: {
        address = AbsoluteXAddress(cpu, true);
    } break;
    default:
        ASSERT(0,
               "Unknown op_code: 0x" << std::hex << int(op_code) << std::dec);
    }

    cpu.mem_read(0); // Just to add 1 Cycle
    uint16_t val = 0;

    if (op_code == Instruction::ASL_ACC) {
        val = cpu.AC;
        val <<= 1;
        cpu.AC = val & 0xFF;
    } else {
        val = cpu.mem_read(address);
        val <<= 1;
        cpu.mem_write(address, val & 0xFF);
    }

    cpu.SR.N = SIGN_BIT(val);
    cpu.SR.Z = val == 0;
    cpu.SR.C = ((val >> 8) == 1);
}

void INST_LDA(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::LDA_IMM: {
        address = ImmediateAddress(cpu);
    } break;
    case Instruction::LDA_ZP: {
        address = ZeroPageAddress(cpu);
    } break;
    case Instruction::LDA_ZPX: {
        address = ZeroPageXAddress(cpu);
    } break;
    case Instruction::LDA_ABS: {
        address = AbsoluteAddress(cpu);
    } break;
    case Instruction::LDA_ABSX: {
        address = AbsoluteXAddress(cpu);
    } break;
    case Instruction::LDA_ABSY: {
        address = AbsoluteYAddress(cpu);
    } break;
    case Instruction::LDA_INDX: {
        address = IndexedIndirectAddress(cpu);
    } break;
    case Instruction::LDA_INDY: {
        address = IndirectIndexedAddress(cpu);
    } break;
    default:
        ASSERT(0,
               "Unknown op_code: 0x" << std::hex << int(op_code) << std::dec);
    }

    cpu.AC = cpu.mem_read(address);
    cpu.SR.N = SIGN_BIT(cpu.AC);
    cpu.SR.Z = cpu.AC == 0;
}

void INST_LDX(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::LDX_IMM: {
        address = ImmediateAddress(cpu);
    } break;
    case Instruction::LDX_ZP: {
        address = ZeroPageAddress(cpu);
    } break;
    case Instruction::LDX_ZPY: {
        address = ZeroPageYAddress(cpu);
    } break;
    case Instruction::LDX_ABS: {
        address = AbsoluteAddress(cpu);
    } break;
    case Instruction::LDX_ABSY: {
        address = AbsoluteYAddress(cpu);
    } break;
    default:
        ASSERT(0,
               "Unknown op_code: 0x" << std::hex << int(op_code) << std::dec);
    }

    cpu.X = cpu.mem_read(address);
    cpu.SR.N = SIGN_BIT(cpu.X);
    cpu.SR.Z = cpu.X == 0;
}

void INST_LDY(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::LDY_IMM: {
        address = ImmediateAddress(cpu);
    } break;
    case Instruction::LDY_ZP: {
        address = ZeroPageAddress(cpu);
    } break;
    case Instruction::LDY_ZPX: {
        address = ZeroPageXAddress(cpu);
    } break;
    case Instruction::LDY_ABS: {
        address = AbsoluteAddress(cpu);
    } break;
    case Instruction::LDY_ABSX: {
        address = AbsoluteXAddress(cpu);
    } break;
    default:
        ASSERT(0,
               "Unknown op_code: 0x" << std::hex << int(op_code) << std::dec);
    }

    cpu.Y = cpu.mem_read(address);
    cpu.SR.N = SIGN_BIT(cpu.Y);
    cpu.SR.Z = cpu.Y == 0;
}

void initialize_map(std::unordered_map<uint8_t, inst_func_t>& inst_map) {
    inst_map[Instruction::ADC_IMM] = inst_map[Instruction::ADC_ZP] =
        inst_map[Instruction::ADC_ZPX] = inst_map[Instruction::ADC_ABS] =
            inst_map[Instruction::ADC_ABSX] = inst_map[Instruction::ADC_ABSY] =
                inst_map[Instruction::ADC_INDX] =
                    inst_map[Instruction::ADC_INDY] = INST_ADC;

    inst_map[Instruction::AND_IMM] = inst_map[Instruction::AND_ZP] =
        inst_map[Instruction::AND_ZPX] = inst_map[Instruction::AND_ABS] =
            inst_map[Instruction::AND_ABSX] = inst_map[Instruction::AND_ABSY] =
                inst_map[Instruction::AND_INDX] =
                    inst_map[Instruction::AND_INDY] = INST_AND;

    inst_map[Instruction::ASL_ACC] = inst_map[Instruction::ASL_ZP] =
        inst_map[Instruction::ASL_ZPX] = inst_map[Instruction::ASL_ABS] =
            inst_map[Instruction::ASL_ABSX] = INST_ASL;

    inst_map[Instruction::LDA_IMM] = inst_map[Instruction::LDA_ZP] =
        inst_map[Instruction::LDA_ZPX] = inst_map[Instruction::LDA_ABS] =
            inst_map[Instruction::LDA_ABSX] = inst_map[Instruction::LDA_ABSY] =
                inst_map[Instruction::LDA_INDX] =
                    inst_map[Instruction::LDA_INDY] = INST_LDA;

    inst_map[Instruction::LDX_IMM] = inst_map[Instruction::LDX_ZP] =
        inst_map[Instruction::LDX_ZPY] = inst_map[Instruction::LDX_ABS] =
            inst_map[Instruction::LDX_ABSY] = INST_LDX;

    inst_map[Instruction::LDY_IMM] = inst_map[Instruction::LDY_ZP] =
        inst_map[Instruction::LDY_ZPX] = inst_map[Instruction::LDY_ABS] =
            inst_map[Instruction::LDY_ABSX] = INST_LDY;
}

std::string ToString(Instruction inst) {
#define INSERT_INST(v)                                                         \
    case v:                                                                    \
        return #v
    switch (inst) {
        INSERT_INST(Instruction::ADC_IMM);
        INSERT_INST(Instruction::ADC_ZP);
        INSERT_INST(Instruction::ADC_ZPX);
        INSERT_INST(Instruction::ADC_ABS);
        INSERT_INST(Instruction::ADC_ABSX);
        INSERT_INST(Instruction::ADC_ABSY);
        INSERT_INST(Instruction::ADC_INDX);
        INSERT_INST(Instruction::ADC_INDY);

        INSERT_INST(Instruction::AND_IMM);
        INSERT_INST(Instruction::AND_ZP);
        INSERT_INST(Instruction::AND_ZPX);
        INSERT_INST(Instruction::AND_ABS);
        INSERT_INST(Instruction::AND_ABSX);
        INSERT_INST(Instruction::AND_ABSY);
        INSERT_INST(Instruction::AND_INDX);
        INSERT_INST(Instruction::AND_INDY);

        INSERT_INST(Instruction::ASL_ACC);
        INSERT_INST(Instruction::ASL_ZP);
        INSERT_INST(Instruction::ASL_ZPX);
        INSERT_INST(Instruction::ASL_ABS);
        INSERT_INST(Instruction::ASL_ABSX);

        INSERT_INST(Instruction::LDA_IMM);
        INSERT_INST(Instruction::LDA_ZP);
        INSERT_INST(Instruction::LDA_ZPX);
        INSERT_INST(Instruction::LDA_ABS);
        INSERT_INST(Instruction::LDA_ABSX);
        INSERT_INST(Instruction::LDA_ABSY);
        INSERT_INST(Instruction::LDA_INDX);
        INSERT_INST(Instruction::LDA_INDY);

        INSERT_INST(Instruction::LDX_IMM);
        INSERT_INST(Instruction::LDX_ZP);
        INSERT_INST(Instruction::LDX_ZPY);
        INSERT_INST(Instruction::LDX_ABS);
        INSERT_INST(Instruction::LDX_ABSY);

        INSERT_INST(Instruction::LDY_IMM);
        INSERT_INST(Instruction::LDY_ZP);
        INSERT_INST(Instruction::LDY_ZPX);
        INSERT_INST(Instruction::LDY_ABS);
        INSERT_INST(Instruction::LDY_ABSX);
    }
#undef INSERT_INST

    return "";
}
