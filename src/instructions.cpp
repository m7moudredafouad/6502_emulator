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
    switch (inst) {
    case Instruction::ADC_IMM:
        return "adc_immediate";
    case Instruction::ADC_ZP:
        return "adc_zeropage";
    case Instruction::ADC_ZPX:
        return "adc_zeropage_x";
    case Instruction::ADC_ABS:
        return "adc_absolute";
    case Instruction::ADC_ABSX:
        return "adc_absolute_x";
    case Instruction::ADC_ABSY:
        return "adc_absolute_y";
    case Instruction::ADC_INDX:
        return "adc_indirect_x";
    case Instruction::ADC_INDY:
        return "adc_indirect_y";

    case Instruction::LDA_IMM:
        return "lda_immediate";
    case Instruction::LDA_ZP:
        return "lda_zeropage";
    case Instruction::LDA_ZPX:
        return "lda_zeropage_x";
    case Instruction::LDA_ABS:
        return "lda_absolute";
    case Instruction::LDA_ABSX:
        return "lda_absolute_x";
    case Instruction::LDA_ABSY:
        return "lda_absolute_y";
    case Instruction::LDA_INDX:
        return "lda_indirect_x";
    case Instruction::LDA_INDY:
        return "lda_indirect_y";

    case Instruction::LDX_IMM:
        return "ldx_immediate";
    case Instruction::LDX_ZP:
        return "ldx_zeropage";
    case Instruction::LDX_ZPY:
        return "ldx_zeropage_y";
    case Instruction::LDX_ABS:
        return "ldx_absolute";
    case Instruction::LDX_ABSY:
        return "ldx_absolute_y";

    case Instruction::LDY_IMM:
        return "ldy_immediate";
    case Instruction::LDY_ZP:
        return "ldy_zeropage";
    case Instruction::LDY_ZPX:
        return "ldy_zeropage_x";
    case Instruction::LDY_ABS:
        return "ldy_absolute";
    case Instruction::LDY_ABSX:
        return "ldy_absolute_x";
    }

    return "";
}
