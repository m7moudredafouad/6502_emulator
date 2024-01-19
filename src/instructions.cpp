#include <addressing.h>
#include <instructions.h>
#include <iostream>

#define GET_BIT(value, bit) (((value) >> (bit)) & 0x1)
#define SIGN_BIT(value) GET_BIT((value), 7)

#define ISTRUCTION_UNREACHABLE(op)                                             \
    ASSERT(0, "Unknown op_code: 0x" << std::hex << int(op) << std::dec)

#define ADD_CYCLE(cpu) cpu.mem_read(0)

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
        ISTRUCTION_UNREACHABLE(op_code);
    }

    uint8_t operand = cpu.mem_read(address);
    uint8_t ac = cpu.AC;
    uint16_t val = operand + cpu.AC + cpu.SR.C;
    cpu.AC = (val & 0xFF);

    cpu.SR.N = SIGN_BIT(cpu.AC);
    cpu.SR.Z = cpu.AC == 0;
    cpu.SR.C = GET_BIT(val, 8);
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
        ISTRUCTION_UNREACHABLE(op_code);
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
        ISTRUCTION_UNREACHABLE(op_code);
    }

    ADD_CYCLE(cpu);
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
    cpu.SR.C = GET_BIT(val, 8);
}

void INST_BRANCH(CPU& cpu, uint8_t op_code) {
    uint8_t offset = cpu.Fetch();
    bool condition = false;
    switch (op_code) {
    case Instruction::BCC: {
        condition = (cpu.SR.C == 0);
    } break;
    case Instruction::BCS: {
        condition = (cpu.SR.C == 1);
    } break;
    case Instruction::BEQ: {
        condition = (cpu.SR.Z == 1);
    } break;
    case Instruction::BNE: {
        condition = (cpu.SR.Z == 0);
    } break;
    case Instruction::BMI: {
        condition = (cpu.SR.N == 1);
    } break;
    case Instruction::BPL: {
        condition = (cpu.SR.N == 0);
    } break;
    case Instruction::BVC: {
        condition = (cpu.SR.V == 0);
    } break;
    case Instruction::BVS: {
        condition = (cpu.SR.V == 1);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    if (condition) {
        ADD_CYCLE(cpu);
        cpu.PC = cpu.get_address(cpu.PC, offset);
    }
}

void INST_BIT(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::BIT_ZP: {
        address = ZeroPageAddress(cpu);
    } break;
    case Instruction::BIT_ABS: {
        address = AbsoluteAddress(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    uint8_t operand = cpu.mem_read(address);
    cpu.SR.Z = ((cpu.AC & operand) == 0);
    cpu.SR.N = GET_BIT(operand, 7);
    cpu.SR.V = GET_BIT(operand, 6);
}

void INST_STATUS(CPU& cpu, uint8_t op_code) {
    switch (op_code) {
    case Instruction::CLC: {
        cpu.SR.C = 0;
    } break;
    case Instruction::CLD: {
        cpu.SR.D = 0;
    } break;
    case Instruction::CLI: {
        cpu.SR.I = 0;
    } break;
    case Instruction::CLV: {
        cpu.SR.V = 0;
    } break;
    case Instruction::SEC: {
        cpu.SR.C = 1;
    } break;
    case Instruction::SED: {
        cpu.SR.D = 1;
    } break;
    case Instruction::SEI: {
        cpu.SR.I = 1;
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    ADD_CYCLE(cpu);
}

void INST_CMP(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::CMP_IMM: {
        address = ImmediateAddress(cpu);
    } break;
    case Instruction::CMP_ZP: {
        address = ZeroPageAddress(cpu);
    } break;
    case Instruction::CMP_ZPX: {
        address = ZeroPageXAddress(cpu);
    } break;
    case Instruction::CMP_ABS: {
        address = AbsoluteAddress(cpu);
    } break;
    case Instruction::CMP_ABSX: {
        address = AbsoluteXAddress(cpu);
    } break;
    case Instruction::CMP_ABSY: {
        address = AbsoluteYAddress(cpu);
    } break;
    case Instruction::CMP_INDX: {
        address = IndexedIndirectAddress(cpu);
    } break;
    case Instruction::CMP_INDY: {
        address = IndirectIndexedAddress(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    auto value = cpu.mem_read(address);
    uint8_t result = cpu.AC - value;

    cpu.SR.N = SIGN_BIT(result);
    cpu.SR.Z = (result == 0);
    cpu.SR.C = (value >= cpu.AC);
}

void INST_CMX(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::CMX_IMM: {
        address = ImmediateAddress(cpu);
    } break;
    case Instruction::CMX_ZP: {
        address = ZeroPageAddress(cpu);
    } break;
    case Instruction::CMX_ABS: {
        address = AbsoluteAddress(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    auto value = cpu.mem_read(address);
    uint8_t result = cpu.X - value;

    cpu.SR.N = SIGN_BIT(result);
    cpu.SR.Z = (result == 0);
    cpu.SR.C = (value >= cpu.X);
}

void INST_CMY(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::CMY_IMM: {
        address = ImmediateAddress(cpu);
    } break;
    case Instruction::CMY_ZP: {
        address = ZeroPageAddress(cpu);
    } break;
    case Instruction::CMY_ABS: {
        address = AbsoluteAddress(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    auto value = cpu.mem_read(address);
    uint8_t result = cpu.Y - value;

    cpu.SR.N = SIGN_BIT(result);
    cpu.SR.Z = (result == 0);
    cpu.SR.C = (value >= cpu.Y);
}

void INST_DEC(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::DEC_ZP: {
        address = ImmediateAddress(cpu);
    } break;
    case Instruction::DEC_ZPX: {
        address = ZeroPageXAddress(cpu);
    } break;
    case Instruction::DEC_ABS: {
        address = AbsoluteAddress(cpu);
    } break;
    case Instruction::DEC_ABSX: {
        address = AbsoluteXAddress(cpu, true);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    uint8_t value = cpu.mem_read(address);
    value--;
    ADD_CYCLE(cpu);
    cpu.mem_write(address, value);

    cpu.SR.N = SIGN_BIT(value);
    cpu.SR.Z = (value == 0);
}

void INST_DEX(CPU& cpu, uint8_t op_code) {
    switch (op_code) {
    case Instruction::DEX: {
        cpu.X--;
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    ADD_CYCLE(cpu);
    cpu.SR.N = SIGN_BIT(cpu.X);
    cpu.SR.Z = (cpu.X);
}

void INST_DEY(CPU& cpu, uint8_t op_code) {
    switch (op_code) {
    case Instruction::DEY: {
        cpu.Y--;
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    ADD_CYCLE(cpu);
    cpu.SR.N = SIGN_BIT(cpu.Y);
    cpu.SR.Z = (cpu.Y);
}

void INST_EOR(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::CMP_IMM: {
        address = ImmediateAddress(cpu);
    } break;
    case Instruction::CMP_ZP: {
        address = ZeroPageAddress(cpu);
    } break;
    case Instruction::CMP_ZPX: {
        address = ZeroPageXAddress(cpu);
    } break;
    case Instruction::CMP_ABS: {
        address = AbsoluteAddress(cpu);
    } break;
    case Instruction::CMP_ABSX: {
        address = AbsoluteXAddress(cpu);
    } break;
    case Instruction::CMP_ABSY: {
        address = AbsoluteYAddress(cpu);
    } break;
    case Instruction::CMP_INDX: {
        address = IndexedIndirectAddress(cpu);
    } break;
    case Instruction::CMP_INDY: {
        address = IndirectIndexedAddress(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    uint8_t result = cpu.AC ^ cpu.mem_read(address);

    cpu.SR.N = SIGN_BIT(result);
    cpu.SR.Z = (result == 0);
}

void INST_INC(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::INC_ZP: {
        address = ImmediateAddress(cpu);
    } break;
    case Instruction::INC_ZPX: {
        address = ZeroPageXAddress(cpu);
    } break;
    case Instruction::INC_ABS: {
        address = AbsoluteAddress(cpu);
    } break;
    case Instruction::INC_ABSX: {
        address = AbsoluteXAddress(cpu, true);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    uint8_t value = cpu.mem_read(address);
    value++;
    ADD_CYCLE(cpu);
    cpu.mem_write(address, value);

    cpu.SR.N = SIGN_BIT(value);
    cpu.SR.Z = (value == 0);
}

void INST_INX(CPU& cpu, uint8_t op_code) {
    switch (op_code) {
    case Instruction::INX: {
        cpu.X++;
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    ADD_CYCLE(cpu);
    cpu.SR.N = SIGN_BIT(cpu.X);
    cpu.SR.Z = (cpu.X);
}

void INST_INY(CPU& cpu, uint8_t op_code) {
    switch (op_code) {
    case Instruction::INY: {
        cpu.Y++;
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    ADD_CYCLE(cpu);
    cpu.SR.N = SIGN_BIT(cpu.Y);
    cpu.SR.Z = (cpu.Y);
}

void INST_JMP(CPU& cpu, uint8_t op_code) {
    switch (op_code) {
    case Instruction::JMP_ABS: {
        cpu.PC = AbsoluteAddress(cpu);
    } break;
    case Instruction::JMP_IND: {
        cpu.PC = IndirectAddress(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
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
        ISTRUCTION_UNREACHABLE(op_code);
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
        ISTRUCTION_UNREACHABLE(op_code);
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
        ISTRUCTION_UNREACHABLE(op_code);
    }

    cpu.Y = cpu.mem_read(address);
    cpu.SR.N = SIGN_BIT(cpu.Y);
    cpu.SR.Z = cpu.Y == 0;
}

void INST_LSR(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::LSR_ACC:
        break;
    case Instruction::LSR_ZP: {
        address = ZeroPageAddress(cpu);
    } break;
    case Instruction::LSR_ZPX: {
        address = ZeroPageXAddress(cpu);
    } break;
    case Instruction::LSR_ABS: {
        address = AbsoluteAddress(cpu);
    } break;
    case Instruction::LSR_ABSX: {
        address = AbsoluteXAddress(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    ADD_CYCLE(cpu);
    uint16_t val = 0;

    if (op_code == Instruction::LSR_ACC) {
        val = cpu.AC;
        cpu.SR.C = GET_BIT(val, 0);
        val >>= 1;
        cpu.AC = val & 0xFF;
    } else {
        val = cpu.mem_read(address);
        cpu.SR.C = GET_BIT(val, 0);
        val >>= 1;
        cpu.mem_write(address, val & 0xFF);
    }

    cpu.SR.N = 0;
    cpu.SR.Z = val == 0;
}

void INST_NOP(CPU& cpu, uint8_t op_code) {
    switch (op_code) {
    case Instruction::NOP: {
        ADD_CYCLE(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }
}

void INST_ORA(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::ORA_IMM: {
        address = ImmediateAddress(cpu);
    } break;
    case Instruction::ORA_ZP: {
        address = ZeroPageAddress(cpu);
    } break;
    case Instruction::ORA_ZPX: {
        address = ZeroPageXAddress(cpu);
    } break;
    case Instruction::ORA_ABS: {
        address = AbsoluteAddress(cpu);
    } break;
    case Instruction::ORA_ABSX: {
        address = AbsoluteXAddress(cpu);
    } break;
    case Instruction::ORA_ABSY: {
        address = AbsoluteYAddress(cpu);
    } break;
    case Instruction::ORA_INDX: {
        address = IndexedIndirectAddress(cpu);
    } break;
    case Instruction::ORA_INDY: {
        address = IndirectIndexedAddress(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    cpu.AC = cpu.AC | cpu.mem_read(address);
    cpu.SR.N = SIGN_BIT(cpu.AC);
    cpu.SR.Z = cpu.AC == 0;
}

void INST_PUSH(CPU& cpu, uint8_t op_code) {

    switch (op_code) {
    case Instruction::PHA: {
        cpu.mem_write(cpu.SP++, cpu.AC);
    } break;
    case Instruction::PHP: {
        cpu.mem_write(cpu.SP++, cpu.SR.Value());
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }
}

void INST_PULL(CPU& cpu, uint8_t op_code) {

    switch (op_code) {
    case Instruction::PLA: {
        cpu.AC = cpu.mem_read(cpu.SP--);
    } break;
    case Instruction::PLP: {
        uint8_t val = cpu.mem_read(cpu.SP--);
        cpu.SR.N = GET_BIT(val, 7);
        cpu.SR.V = GET_BIT(val, 6);
        cpu.SR.B = GET_BIT(val, 4);
        cpu.SR.D = GET_BIT(val, 3);
        cpu.SR.I = GET_BIT(val, 2);
        cpu.SR.Z = GET_BIT(val, 1);
        cpu.SR.C = GET_BIT(val, 0);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }
}

void INST_ROL(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::ROL_ACC:
        break;
    case Instruction::ROL_ZP: {
        address = ZeroPageAddress(cpu);
    } break;
    case Instruction::ROL_ZPX: {
        address = ZeroPageXAddress(cpu);
    } break;
    case Instruction::ROL_ABS: {
        address = AbsoluteAddress(cpu);
    } break;
    case Instruction::ROL_ABSX: {
        address = AbsoluteXAddress(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    ADD_CYCLE(cpu);
    uint16_t val = 0;

    if (op_code == Instruction::ROL_ACC) {
        val = cpu.AC;
        cpu.SR.C = GET_BIT(val, 7);
        val <<= 1;
        cpu.AC = val & 0xFF | cpu.SR.C;
    } else {
        val = cpu.mem_read(address);
        cpu.SR.C = GET_BIT(val, 7);
        val <<= 1;
        cpu.mem_write(address, val & 0xFF | cpu.SR.C);
    }

    cpu.SR.N = SIGN_BIT(val);
    cpu.SR.Z = cpu.AC == 0;
}

void INST_ROR(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::ROR_ACC:
        break;
    case Instruction::ROR_ZP: {
        address = ZeroPageAddress(cpu);
    } break;
    case Instruction::ROR_ZPX: {
        address = ZeroPageXAddress(cpu);
    } break;
    case Instruction::ROR_ABS: {
        address = AbsoluteAddress(cpu);
    } break;
    case Instruction::ROR_ABSX: {
        address = AbsoluteXAddress(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    ADD_CYCLE(cpu);
    uint16_t val = 0;

    if (op_code == Instruction::ROL_ACC) {
        val = cpu.AC;
        cpu.SR.C = GET_BIT(val, 0);
        val >>= 1;
        cpu.AC = val & 0xFF | (cpu.SR.C << 7);
    } else {
        val = cpu.mem_read(address);
        cpu.SR.C = GET_BIT(val, 0);
        val >>= 1;
        cpu.mem_write(address, val & 0xFF | (cpu.SR.C << 7));
    }

    cpu.SR.N = SIGN_BIT(val);
    cpu.SR.Z = cpu.AC == 0;
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

    inst_map[Instruction::BCC] = inst_map[Instruction::BCS] =
        inst_map[Instruction::BEQ] = inst_map[Instruction::BMI] =
            inst_map[Instruction::BNE] = inst_map[Instruction::BPL] =
                inst_map[Instruction::BVC] = inst_map[Instruction::BVS] =
                    INST_BRANCH;

    inst_map[Instruction::BIT_ZP] = inst_map[Instruction::BIT_ABS] = INST_BIT;

    inst_map[Instruction::CLC] = inst_map[Instruction::CLD] =
        inst_map[Instruction::CLI] = inst_map[Instruction::CLV] =
            inst_map[Instruction::SEC] = inst_map[Instruction::SED] =
                inst_map[Instruction::SEI] = INST_STATUS;

    inst_map[Instruction::CMP_IMM] = inst_map[Instruction::CMP_ZP] =
        inst_map[Instruction::CMP_ZPX] = inst_map[Instruction::CMP_ABS] =
            inst_map[Instruction::CMP_ABSX] = inst_map[Instruction::CMP_ABSY] =
                inst_map[Instruction::CMP_INDX] =
                    inst_map[Instruction::CMP_INDY] = INST_CMP;

    inst_map[Instruction::CMX_IMM] = inst_map[Instruction::CMX_ZP] =
        inst_map[Instruction::CMX_ABS] = INST_CMX;

    inst_map[Instruction::CMY_IMM] = inst_map[Instruction::CMY_ZP] =
        inst_map[Instruction::CMY_ABS] = INST_CMY;

    inst_map[Instruction::DEC_ZP] = inst_map[Instruction::DEC_ZPX] =
        inst_map[Instruction::DEC_ABS] = inst_map[Instruction::DEC_ABSX] =
            INST_DEC;

    inst_map[Instruction::DEX] = INST_DEX;
    inst_map[Instruction::DEY] = INST_DEY;

    inst_map[Instruction::EOR_IMM] = inst_map[Instruction::EOR_ZP] =
        inst_map[Instruction::EOR_ZPX] = inst_map[Instruction::EOR_ABS] =
            inst_map[Instruction::EOR_ABSX] = inst_map[Instruction::EOR_ABSY] =
                inst_map[Instruction::EOR_INDX] =
                    inst_map[Instruction::EOR_INDY] = INST_EOR;

    inst_map[Instruction::INC_ZP] = inst_map[Instruction::INC_ZPX] =
        inst_map[Instruction::INC_ABS] = inst_map[Instruction::INC_ABSX] =
            INST_INC;

    inst_map[Instruction::INX] = INST_INX;
    inst_map[Instruction::INY] = INST_INY;

    inst_map[Instruction::JMP_ABS] = inst_map[Instruction::JMP_IND] = INST_JMP;

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

    inst_map[Instruction::LSR_ACC] = inst_map[Instruction::LSR_ZP] =
        inst_map[Instruction::LSR_ZPX] = inst_map[Instruction::LSR_ABS] =
            inst_map[Instruction::LSR_ABSX] = INST_LSR;

    inst_map[Instruction::NOP] = INST_NOP;

    inst_map[Instruction::ORA_IMM] = inst_map[Instruction::ORA_ZP] =
        inst_map[Instruction::ORA_ZPX] = inst_map[Instruction::ORA_ABS] =
            inst_map[Instruction::ORA_ABSX] = inst_map[Instruction::ORA_ABSY] =
                inst_map[Instruction::ORA_INDX] =
                    inst_map[Instruction::ORA_INDY] = INST_ORA;

    inst_map[Instruction::PHA] = inst_map[Instruction::PHP] = INST_PUSH;
    inst_map[Instruction::PLA] = inst_map[Instruction::PLP] = INST_PULL;

    inst_map[Instruction::ROL_ACC] = inst_map[Instruction::ROL_ZP] =
        inst_map[Instruction::ROL_ZPX] = inst_map[Instruction::ROL_ABS] =
            inst_map[Instruction::ROL_ABSX] = INST_ROL;

    inst_map[Instruction::ROR_ACC] = inst_map[Instruction::ROR_ZP] =
        inst_map[Instruction::ROR_ZPX] = inst_map[Instruction::ROR_ABS] =
            inst_map[Instruction::ROR_ABSX] = INST_ROR;
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

        INSERT_INST(Instruction::BCC);
        INSERT_INST(Instruction::BCS);
        INSERT_INST(Instruction::BEQ);
        INSERT_INST(Instruction::BIT_ZP);
        INSERT_INST(Instruction::BIT_ABS);
        INSERT_INST(Instruction::BMI);
        INSERT_INST(Instruction::BNE);
        INSERT_INST(Instruction::BPL);
        INSERT_INST(Instruction::BVC);
        INSERT_INST(Instruction::BVS);

        INSERT_INST(Instruction::CLC);
        INSERT_INST(Instruction::CLD);
        INSERT_INST(Instruction::CLI);
        INSERT_INST(Instruction::CLV);

        INSERT_INST(Instruction::CMP_IMM);
        INSERT_INST(Instruction::CMP_ZP);
        INSERT_INST(Instruction::CMP_ZPX);
        INSERT_INST(Instruction::CMP_ABS);
        INSERT_INST(Instruction::CMP_ABSX);
        INSERT_INST(Instruction::CMP_ABSY);
        INSERT_INST(Instruction::CMP_INDX);
        INSERT_INST(Instruction::CMP_INDY);

        INSERT_INST(Instruction::CMX_IMM);
        INSERT_INST(Instruction::CMX_ZP);
        INSERT_INST(Instruction::CMX_ABS);

        INSERT_INST(Instruction::CMY_IMM);
        INSERT_INST(Instruction::CMY_ZP);
        INSERT_INST(Instruction::CMY_ABS);

        INSERT_INST(Instruction::DEC_ZP);
        INSERT_INST(Instruction::DEC_ZPX);
        INSERT_INST(Instruction::DEC_ABS);
        INSERT_INST(Instruction::DEC_ABSX);

        INSERT_INST(Instruction::DEX);
        INSERT_INST(Instruction::DEY);

        INSERT_INST(Instruction::EOR_IMM);
        INSERT_INST(Instruction::EOR_ZP);
        INSERT_INST(Instruction::EOR_ZPX);
        INSERT_INST(Instruction::EOR_ABS);
        INSERT_INST(Instruction::EOR_ABSX);
        INSERT_INST(Instruction::EOR_ABSY);
        INSERT_INST(Instruction::EOR_INDX);
        INSERT_INST(Instruction::EOR_INDY);

        INSERT_INST(Instruction::INC_ZP);
        INSERT_INST(Instruction::INC_ZPX);
        INSERT_INST(Instruction::INC_ABS);
        INSERT_INST(Instruction::INC_ABSX);

        INSERT_INST(Instruction::INX);
        INSERT_INST(Instruction::INY);

        INSERT_INST(Instruction::JMP_ABS);
        INSERT_INST(Instruction::JMP_IND);

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

        INSERT_INST(Instruction::LSR_ACC);
        INSERT_INST(Instruction::LSR_ZP);
        INSERT_INST(Instruction::LSR_ZPX);
        INSERT_INST(Instruction::LSR_ABS);
        INSERT_INST(Instruction::LSR_ABSX);

        INSERT_INST(Instruction::ORA_IMM);
        INSERT_INST(Instruction::ORA_ZP);
        INSERT_INST(Instruction::ORA_ZPX);
        INSERT_INST(Instruction::ORA_ABS);
        INSERT_INST(Instruction::ORA_ABSX);
        INSERT_INST(Instruction::ORA_ABSY);
        INSERT_INST(Instruction::ORA_INDX);
        INSERT_INST(Instruction::ORA_INDY);

        INSERT_INST(Instruction::PHA);
        INSERT_INST(Instruction::PHP);

        INSERT_INST(Instruction::PLA);
        INSERT_INST(Instruction::PLP);

        INSERT_INST(Instruction::ROL_ACC);
        INSERT_INST(Instruction::ROL_ZP);
        INSERT_INST(Instruction::ROL_ZPX);
        INSERT_INST(Instruction::ROL_ABS);
        INSERT_INST(Instruction::ROL_ABSX);

        INSERT_INST(Instruction::ROR_ACC);
        INSERT_INST(Instruction::ROR_ZP);
        INSERT_INST(Instruction::ROR_ZPX);
        INSERT_INST(Instruction::ROR_ABS);
        INSERT_INST(Instruction::ROR_ABSX);

        INSERT_INST(Instruction::SEC);
        INSERT_INST(Instruction::SED);
        INSERT_INST(Instruction::SEI);

        INSERT_INST(Instruction::NOP);
    }
#undef INSERT_INST

    return "";
}
