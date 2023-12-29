#include <addressing.h>
#include <instructions.h>
#include <iostream>

void INST_LDA(CPU& cpu, uint8_t op_code) {
    switch (op_code) {
    case Instruction::LDA_IMM: {
        cpu.AC = cpu.Fetch();
    } break;
    case Instruction::LDA_ZP: {
        cpu.AC = ZeroPageAddress(cpu);
    } break;
    case Instruction::LDA_ZPX: {
        cpu.AC = ZeroPageXAddress(cpu);
    } break;
    case Instruction::LDA_ABS: {
        cpu.AC = AbsoluteAddress(cpu);
    } break;
    case Instruction::LDA_ABSX: {
        cpu.AC = AbsoluteXAddress(cpu);
    } break;
    case Instruction::LDA_ABSY: {
        cpu.AC = AbsoluteYAddress(cpu);
    } break;
    case Instruction::LDA_INDX: {
        cpu.AC = IndexedIndirectAddress(cpu);
    } break;
    case Instruction::LDA_INDY: {
        cpu.AC = IndirectIndexedAddress(cpu);
    } break;
    default:
        ASSERT(0,
               "Unknown op_code: 0x" << std::hex << int(op_code) << std::dec);
    }

    cpu.SR.N = ((cpu.AC & 0x80) > 0);
    cpu.SR.Z = cpu.AC == 0;
}

void INST_LDX(CPU& cpu, uint8_t op_code) {
    switch (op_code) {
    case Instruction::LDX_IMM: {
        cpu.X = cpu.Fetch();
    } break;
    case Instruction::LDX_ZP: {
        cpu.X = ZeroPageAddress(cpu);
    } break;
    case Instruction::LDX_ZPY: {
        cpu.X = ZeroPageYAddress(cpu);
    } break;
    case Instruction::LDX_ABS: {
        cpu.X = AbsoluteAddress(cpu);
    } break;
    case Instruction::LDX_ABSY: {
        cpu.X = AbsoluteYAddress(cpu);
    } break;
    default:
        ASSERT(0,
               "Unknown op_code: 0x" << std::hex << int(op_code) << std::dec);
    }

    cpu.SR.N = ((cpu.X & 0x80) > 0);
    cpu.SR.Z = cpu.X == 0;
}

void INST_LDY(CPU& cpu, uint8_t op_code) {
    switch (op_code) {
    case Instruction::LDY_IMM: {
        cpu.Y = cpu.Fetch();
    } break;
    case Instruction::LDY_ZP: {
        cpu.Y = ZeroPageAddress(cpu);
    } break;
    case Instruction::LDY_ZPX: {
        cpu.Y = ZeroPageXAddress(cpu);
    } break;
    case Instruction::LDY_ABS: {
        cpu.Y = AbsoluteAddress(cpu);
    } break;
    case Instruction::LDY_ABSX: {
        cpu.Y = AbsoluteXAddress(cpu);
    } break;
    default:
        ASSERT(0,
               "Unknown op_code: 0x" << std::hex << int(op_code) << std::dec);
    }

    cpu.SR.N = ((cpu.Y & 0x80) > 0);
    cpu.SR.Z = cpu.Y == 0;
}

void initialize_map(std::unordered_map<uint8_t, inst_func_t>& inst_map) {
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
