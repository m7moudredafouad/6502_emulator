#include <addressing.h>
#include <instructions.h>
#include <iostream>

#define ISTRUCTION_UNREACHABLE(op)                                             \
    ASSERT(0, "Unknown op_code: 0x" << std::hex << int(op) << std::dec)

void INST_ADC(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::ADC_IMM: {
        address = ADDR_IMM(cpu);
    } break;
    case Instruction::ADC_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::ADC_ZPX: {
        address = ADDR_ZPX(cpu);
    } break;
    case Instruction::ADC_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    case Instruction::ADC_ABSX: {
        address = ADDR_ABSX(cpu);
    } break;
    case Instruction::ADC_ABSY: {
        address = ADDR_ABSY(cpu);
    } break;
    case Instruction::ADC_INDX: {
        address = ADDR_INDX(cpu);
    } break;
    case Instruction::ADC_INDY: {
        address = ADDR_INDY(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    uint8_t operand = cpu.read(address);
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
        address = ADDR_IMM(cpu);
    } break;
    case Instruction::AND_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::AND_ZPX: {
        address = ADDR_ZPX(cpu);
    } break;
    case Instruction::AND_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    case Instruction::AND_ABSX: {
        address = ADDR_ABSX(cpu);
    } break;
    case Instruction::AND_ABSY: {
        address = ADDR_ABSY(cpu);
    } break;
    case Instruction::AND_INDX: {
        address = ADDR_INDX(cpu);
    } break;
    case Instruction::AND_INDY: {
        address = ADDR_INDY(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    cpu.AC &= cpu.read(address);

    cpu.SR.N = SIGN_BIT(cpu.AC);
    cpu.SR.Z = cpu.AC == 0;
}

void INST_ASL(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::ASL_ACC:
        break;
    case Instruction::ASL_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::ASL_ZPX: {
        address = ADDR_ZPX(cpu);
    } break;
    case Instruction::ASL_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    case Instruction::ASL_ABSX: {
        address = ADDR_ABSX(cpu, true);
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
        val = cpu.read(address);
        val <<= 1;
        cpu.write(address, val & 0xFF);
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
        auto old_pc = cpu.PC;
        cpu.PC += int8_t(offset);
        if ((cpu.PC >> 8) != (old_pc >> 8)) {
            ADD_CYCLE(cpu);
        }
    }
}

void INST_BIT(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::BIT_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::BIT_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    uint8_t operand = cpu.read(address);
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

void INST_BRK(CPU& cpu, uint8_t op_code) {
    switch (op_code) {
    case Instruction::BRK: {
        auto [low, high] = bytes_from_address(cpu.PC);

        cpu.PUSH(low);
        cpu.PUSH(high);
        cpu.PUSH(cpu.SR.Value());

        cpu.PC = address_from_bytes(cpu.read(0xFFFE), cpu.read(0xFFFF));

        cpu.SR.B = 1;

        ADD_CYCLE(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }
}

void INST_CMP(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::CMP_IMM: {
        address = ADDR_IMM(cpu);
    } break;
    case Instruction::CMP_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::CMP_ZPX: {
        address = ADDR_ZPX(cpu);
    } break;
    case Instruction::CMP_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    case Instruction::CMP_ABSX: {
        address = ADDR_ABSX(cpu);
    } break;
    case Instruction::CMP_ABSY: {
        address = ADDR_ABSY(cpu);
    } break;
    case Instruction::CMP_INDX: {
        address = ADDR_INDX(cpu);
    } break;
    case Instruction::CMP_INDY: {
        address = ADDR_INDY(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    auto value = cpu.read(address);
    uint8_t result = cpu.AC - value;

    cpu.SR.N = SIGN_BIT(result);
    cpu.SR.Z = (result == 0);
    cpu.SR.C = (cpu.AC >= value);
}

void INST_CMX(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::CMX_IMM: {
        address = ADDR_IMM(cpu);
    } break;
    case Instruction::CMX_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::CMX_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    auto value = cpu.read(address);
    uint8_t result = cpu.X - value;

    cpu.SR.N = SIGN_BIT(result);
    cpu.SR.Z = (result == 0);
    cpu.SR.C = (value >= cpu.X);
}

void INST_CMY(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::CMY_IMM: {
        address = ADDR_IMM(cpu);
    } break;
    case Instruction::CMY_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::CMY_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    auto value = cpu.read(address);
    uint8_t result = cpu.Y - value;

    cpu.SR.N = SIGN_BIT(result);
    cpu.SR.Z = (result == 0);
    cpu.SR.C = (value >= cpu.Y);
}

void INST_DEC(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::DEC_ZP: {
        address = ADDR_IMM(cpu);
    } break;
    case Instruction::DEC_ZPX: {
        address = ADDR_ZPX(cpu);
    } break;
    case Instruction::DEC_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    case Instruction::DEC_ABSX: {
        address = ADDR_ABSX(cpu, true);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    uint8_t value = cpu.read(address);
    value--;
    ADD_CYCLE(cpu);
    cpu.write(address, value);

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
    cpu.SR.Z = (cpu.X == 0);
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
        address = ADDR_IMM(cpu);
    } break;
    case Instruction::CMP_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::CMP_ZPX: {
        address = ADDR_ZPX(cpu);
    } break;
    case Instruction::CMP_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    case Instruction::CMP_ABSX: {
        address = ADDR_ABSX(cpu);
    } break;
    case Instruction::CMP_ABSY: {
        address = ADDR_ABSY(cpu);
    } break;
    case Instruction::CMP_INDX: {
        address = ADDR_INDX(cpu);
    } break;
    case Instruction::CMP_INDY: {
        address = ADDR_INDY(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    uint8_t result = cpu.AC ^ cpu.read(address);

    cpu.SR.N = SIGN_BIT(result);
    cpu.SR.Z = (result == 0);
}

void INST_INC(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::INC_ZP: {
        address = ADDR_IMM(cpu);
    } break;
    case Instruction::INC_ZPX: {
        address = ADDR_ZPX(cpu);
    } break;
    case Instruction::INC_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    case Instruction::INC_ABSX: {
        address = ADDR_ABSX(cpu, true);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    uint8_t value = cpu.read(address);
    value++;
    ADD_CYCLE(cpu);
    cpu.write(address, value);

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
    cpu.SR.Z = (cpu.Y == 0);
}

void INST_JMP(CPU& cpu, uint8_t op_code) {
    switch (op_code) {
    case Instruction::JMP_ABS: {
        cpu.PC = ADDR_ABS(cpu);
    } break;
    case Instruction::JMP_IND: {
        cpu.PC = ADDR_IND(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }
}

void INST_JSR(CPU& cpu, uint8_t op_code) {
    switch (op_code) {
    case Instruction::JSR: {
        auto new_add = ADDR_ABS(cpu);
        auto [low, high] = bytes_from_address(cpu.PC - 1);
        cpu.PUSH(low);
        cpu.PUSH(high);
        ADD_CYCLE(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }
}

void INST_LDA(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::LDA_IMM: {
        address = ADDR_IMM(cpu);
    } break;
    case Instruction::LDA_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::LDA_ZPX: {
        address = ADDR_ZPX(cpu);
    } break;
    case Instruction::LDA_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    case Instruction::LDA_ABSX: {
        address = ADDR_ABSX(cpu);
    } break;
    case Instruction::LDA_ABSY: {
        address = ADDR_ABSY(cpu);
    } break;
    case Instruction::LDA_INDX: {
        address = ADDR_INDX(cpu);
    } break;
    case Instruction::LDA_INDY: {
        address = ADDR_INDY(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    cpu.AC = cpu.read(address);
    cpu.SR.N = SIGN_BIT(cpu.AC);
    cpu.SR.Z = cpu.AC == 0;
}

void INST_LDX(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::LDX_IMM: {
        address = ADDR_IMM(cpu);
    } break;
    case Instruction::LDX_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::LDX_ZPY: {
        address = ADDR_ZPY(cpu);
    } break;
    case Instruction::LDX_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    case Instruction::LDX_ABSY: {
        address = ADDR_ABSY(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    cpu.X = cpu.read(address);
    cpu.SR.N = SIGN_BIT(cpu.X);
    cpu.SR.Z = cpu.X == 0;
}

void INST_LDY(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::LDY_IMM: {
        address = ADDR_IMM(cpu);
    } break;
    case Instruction::LDY_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::LDY_ZPX: {
        address = ADDR_ZPX(cpu);
    } break;
    case Instruction::LDY_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    case Instruction::LDY_ABSX: {
        address = ADDR_ABSX(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    cpu.Y = cpu.read(address);
    cpu.SR.N = SIGN_BIT(cpu.Y);
    cpu.SR.Z = cpu.Y == 0;
}

void INST_LSR(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::LSR_ACC:
        break;
    case Instruction::LSR_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::LSR_ZPX: {
        address = ADDR_ZPX(cpu);
    } break;
    case Instruction::LSR_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    case Instruction::LSR_ABSX: {
        address = ADDR_ABSX(cpu);
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
        val = cpu.read(address);
        cpu.SR.C = GET_BIT(val, 0);
        val >>= 1;
        cpu.write(address, val & 0xFF);
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
        address = ADDR_IMM(cpu);
    } break;
    case Instruction::ORA_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::ORA_ZPX: {
        address = ADDR_ZPX(cpu);
    } break;
    case Instruction::ORA_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    case Instruction::ORA_ABSX: {
        address = ADDR_ABSX(cpu);
    } break;
    case Instruction::ORA_ABSY: {
        address = ADDR_ABSY(cpu);
    } break;
    case Instruction::ORA_INDX: {
        address = ADDR_INDX(cpu);
    } break;
    case Instruction::ORA_INDY: {
        address = ADDR_INDY(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    cpu.AC = cpu.AC | cpu.read(address);
    cpu.SR.N = SIGN_BIT(cpu.AC);
    cpu.SR.Z = cpu.AC == 0;
}

void INST_PUSH(CPU& cpu, uint8_t op_code) {
    switch (op_code) {
    case Instruction::PHA: {
        cpu.PUSH(cpu.AC);
    } break;
    case Instruction::PHP: {
        cpu.PUSH(cpu.SR.Value());
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }
    ADD_CYCLE(cpu);
}

void INST_PULL(CPU& cpu, uint8_t op_code) {

    switch (op_code) {
    case Instruction::PLA: {
        cpu.AC = cpu.POP();
    } break;
    case Instruction::PLP: {
        cpu.SR.Set(cpu.POP());
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }
    ADD_CYCLE(cpu);
    ADD_CYCLE(cpu);
}

void INST_ROL(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::ROL_ACC:
        break;
    case Instruction::ROL_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::ROL_ZPX: {
        address = ADDR_ZPX(cpu);
    } break;
    case Instruction::ROL_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    case Instruction::ROL_ABSX: {
        address = ADDR_ABSX(cpu);
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
        val = cpu.read(address);
        cpu.write(address, (val << 1) & 0xFF | cpu.SR.C);
        cpu.SR.C = GET_BIT(val, 7);
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
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::ROR_ZPX: {
        address = ADDR_ZPX(cpu);
    } break;
    case Instruction::ROR_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    case Instruction::ROR_ABSX: {
        address = ADDR_ABSX(cpu);
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
        val = cpu.read(address);
        cpu.write(address, (val >> 1) & 0xFF | (cpu.SR.C << 7));
        cpu.SR.C = GET_BIT(val, 0);
    }

    cpu.SR.N = SIGN_BIT(val);
    cpu.SR.Z = cpu.AC == 0;
}

void INST_RTI(CPU& cpu, uint8_t op_code) {
    switch (op_code) {
    case Instruction::RTI: {
        cpu.SR.Set(cpu.POP());
        cpu.PC = address_from_bytes(cpu.POP(), cpu.POP());
        ADD_CYCLE(cpu);
        ADD_CYCLE(cpu);

    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }
}

void INST_RTS(CPU& cpu, uint8_t op_code) {
    switch (op_code) {
    case Instruction::RTS: {
        cpu.PC = address_from_bytes(cpu.POP(), cpu.POP());
        ADD_CYCLE(cpu);
        ADD_CYCLE(cpu);
        ADD_CYCLE(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }
}

void INST_SBC(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::SBC_IMM: {
        address = ADDR_IMM(cpu);
    } break;
    case Instruction::SBC_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::SBC_ZPX: {
        address = ADDR_ZPX(cpu);
    } break;
    case Instruction::SBC_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    case Instruction::SBC_ABSX: {
        address = ADDR_ABSX(cpu);
    } break;
    case Instruction::SBC_ABSY: {
        address = ADDR_ABSY(cpu);
    } break;
    case Instruction::SBC_INDX: {
        address = ADDR_INDX(cpu);
    } break;
    case Instruction::SBC_INDY: {
        address = ADDR_INDY(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    uint8_t operand = cpu.read(address);
    uint8_t ac = cpu.AC;
    uint16_t val = cpu.AC - operand - (1 - cpu.SR.C);
    cpu.AC = (val & 0xFF);

    cpu.SR.N = SIGN_BIT(cpu.AC);
    cpu.SR.Z = cpu.AC == 0;

    cpu.SR.V = 0;
    if ((SIGN_BIT(ac) == SIGN_BIT(operand))) {
        cpu.SR.V = (SIGN_BIT(ac) != SIGN_BIT(cpu.AC));
        if (cpu.SR.V) {
            cpu.SR.C = 0;
        }
    }
}

void INST_STA(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::STA_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::STA_ZPX: {
        address = ADDR_ZPX(cpu);
    } break;
    case Instruction::STA_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    case Instruction::STA_ABSX: {
        address = ADDR_ABSX(cpu, true);
    } break;
    case Instruction::STA_ABSY: {
        address = ADDR_ABSY(cpu, true);
    } break;
    case Instruction::STA_INDX: {
        address = ADDR_INDX(cpu);
    } break;
    case Instruction::STA_INDY: {
        address = ADDR_INDY(cpu, true);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    cpu.write(address, cpu.AC);
}

void INST_STX(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::STX_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::STX_ZPY: {
        address = ADDR_ZPX(cpu);
    } break;
    case Instruction::STX_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    cpu.write(address, cpu.X);
}

void INST_STY(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::STY_ZP: {
        address = ADDR_ZP(cpu);
    } break;
    case Instruction::STY_ZPX: {
        address = ADDR_ZPX(cpu);
    } break;
    case Instruction::STY_ABS: {
        address = ADDR_ABS(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }

    cpu.write(address, cpu.Y);
}

void INST_TRANSFER(CPU& cpu, uint8_t op_code) {
    uint16_t address;

    switch (op_code) {
    case Instruction::TAX: {
        cpu.X = cpu.AC;
        ADD_CYCLE(cpu);
    } break;
    case Instruction::TAY: {
        ADD_CYCLE(cpu);
        cpu.Y = cpu.AC;
    } break;
    case Instruction::TSX: {
        cpu.X = cpu.read(cpu.SP);
    } break;
    case Instruction::TXA: {
        cpu.AC = cpu.X;
        ADD_CYCLE(cpu);
    } break;
    case Instruction::TXS: {
        cpu.write(cpu.SP, cpu.X);
    } break;
    case Instruction::TYA: {
        cpu.AC = cpu.X;
        ADD_CYCLE(cpu);
    } break;
    default:
        ISTRUCTION_UNREACHABLE(op_code);
    }
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

    inst_map[Instruction::BRK] = INST_BRK;

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

    inst_map[Instruction::JSR] = INST_JSR;

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

    inst_map[Instruction::RTI] = INST_RTI;
    inst_map[Instruction::RTS] = INST_RTS;

    inst_map[Instruction::SBC_IMM] = inst_map[Instruction::SBC_ZP] =
        inst_map[Instruction::SBC_ZPX] = inst_map[Instruction::SBC_ABS] =
            inst_map[Instruction::SBC_ABSX] = inst_map[Instruction::SBC_ABSY] =
                inst_map[Instruction::SBC_INDX] =
                    inst_map[Instruction::SBC_INDY] = INST_SBC;

    inst_map[Instruction::STA_ZP] = inst_map[Instruction::STA_ZPX] =
        inst_map[Instruction::STA_ABS] = inst_map[Instruction::STA_ABSX] =
            inst_map[Instruction::STA_ABSY] = inst_map[Instruction::STA_INDX] =
                inst_map[Instruction::STA_INDY] = INST_STA;

    inst_map[Instruction::STX_ZP] = inst_map[Instruction::STX_ZPY] =
        inst_map[Instruction::STX_ABS] = INST_STX;

    inst_map[Instruction::STY_ZP] = inst_map[Instruction::STY_ZPX] =
        inst_map[Instruction::STY_ABS] = INST_STY;

    inst_map[Instruction::TAX] = inst_map[Instruction::TAY] =
        inst_map[Instruction::TSX] = inst_map[Instruction::TXA] =
            inst_map[Instruction::TXS] = inst_map[Instruction::TYA] =
                INST_TRANSFER;
}

std::string ToString(Instruction inst) {
#define INSERT_INST(v)                                                         \
    case Instruction::v:                                                                    \
        return #v
    switch (inst) {
        INSERT_INST(ADC_IMM);
        INSERT_INST(ADC_ZP);
        INSERT_INST(ADC_ZPX);
        INSERT_INST(ADC_ABS);
        INSERT_INST(ADC_ABSX);
        INSERT_INST(ADC_ABSY);
        INSERT_INST(ADC_INDX);
        INSERT_INST(ADC_INDY);

        INSERT_INST(AND_IMM);
        INSERT_INST(AND_ZP);
        INSERT_INST(AND_ZPX);
        INSERT_INST(AND_ABS);
        INSERT_INST(AND_ABSX);
        INSERT_INST(AND_ABSY);
        INSERT_INST(AND_INDX);
        INSERT_INST(AND_INDY);

        INSERT_INST(ASL_ACC);
        INSERT_INST(ASL_ZP);
        INSERT_INST(ASL_ZPX);
        INSERT_INST(ASL_ABS);
        INSERT_INST(ASL_ABSX);

        INSERT_INST(BCC);
        INSERT_INST(BCS);
        INSERT_INST(BEQ);
        INSERT_INST(BIT_ZP);
        INSERT_INST(BIT_ABS);
        INSERT_INST(BMI);
        INSERT_INST(BNE);
        INSERT_INST(BPL);
        INSERT_INST(BVC);
        INSERT_INST(BVS);

        INSERT_INST(BRK);

        INSERT_INST(CLC);
        INSERT_INST(CLD);
        INSERT_INST(CLI);
        INSERT_INST(CLV);

        INSERT_INST(CMP_IMM);
        INSERT_INST(CMP_ZP);
        INSERT_INST(CMP_ZPX);
        INSERT_INST(CMP_ABS);
        INSERT_INST(CMP_ABSX);
        INSERT_INST(CMP_ABSY);
        INSERT_INST(CMP_INDX);
        INSERT_INST(CMP_INDY);

        INSERT_INST(CMX_IMM);
        INSERT_INST(CMX_ZP);
        INSERT_INST(CMX_ABS);

        INSERT_INST(CMY_IMM);
        INSERT_INST(CMY_ZP);
        INSERT_INST(CMY_ABS);

        INSERT_INST(DEC_ZP);
        INSERT_INST(DEC_ZPX);
        INSERT_INST(DEC_ABS);
        INSERT_INST(DEC_ABSX);

        INSERT_INST(DEX);
        INSERT_INST(DEY);

        INSERT_INST(EOR_IMM);
        INSERT_INST(EOR_ZP);
        INSERT_INST(EOR_ZPX);
        INSERT_INST(EOR_ABS);
        INSERT_INST(EOR_ABSX);
        INSERT_INST(EOR_ABSY);
        INSERT_INST(EOR_INDX);
        INSERT_INST(EOR_INDY);

        INSERT_INST(INC_ZP);
        INSERT_INST(INC_ZPX);
        INSERT_INST(INC_ABS);
        INSERT_INST(INC_ABSX);

        INSERT_INST(INX);
        INSERT_INST(INY);

        INSERT_INST(JMP_ABS);
        INSERT_INST(JMP_IND);

        INSERT_INST(JSR);

        INSERT_INST(LDA_IMM);
        INSERT_INST(LDA_ZP);
        INSERT_INST(LDA_ZPX);
        INSERT_INST(LDA_ABS);
        INSERT_INST(LDA_ABSX);
        INSERT_INST(LDA_ABSY);
        INSERT_INST(LDA_INDX);
        INSERT_INST(LDA_INDY);

        INSERT_INST(LDX_IMM);
        INSERT_INST(LDX_ZP);
        INSERT_INST(LDX_ZPY);
        INSERT_INST(LDX_ABS);
        INSERT_INST(LDX_ABSY);

        INSERT_INST(LDY_IMM);
        INSERT_INST(LDY_ZP);
        INSERT_INST(LDY_ZPX);
        INSERT_INST(LDY_ABS);
        INSERT_INST(LDY_ABSX);

        INSERT_INST(LSR_ACC);
        INSERT_INST(LSR_ZP);
        INSERT_INST(LSR_ZPX);
        INSERT_INST(LSR_ABS);
        INSERT_INST(LSR_ABSX);

        INSERT_INST(ORA_IMM);
        INSERT_INST(ORA_ZP);
        INSERT_INST(ORA_ZPX);
        INSERT_INST(ORA_ABS);
        INSERT_INST(ORA_ABSX);
        INSERT_INST(ORA_ABSY);
        INSERT_INST(ORA_INDX);
        INSERT_INST(ORA_INDY);

        INSERT_INST(PHA);
        INSERT_INST(PHP);

        INSERT_INST(PLA);
        INSERT_INST(PLP);

        INSERT_INST(NOP);

        INSERT_INST(ROL_ACC);
        INSERT_INST(ROL_ZP);
        INSERT_INST(ROL_ZPX);
        INSERT_INST(ROL_ABS);
        INSERT_INST(ROL_ABSX);

        INSERT_INST(ROR_ACC);
        INSERT_INST(ROR_ZP);
        INSERT_INST(ROR_ZPX);
        INSERT_INST(ROR_ABS);
        INSERT_INST(ROR_ABSX);

        INSERT_INST(RTI);
        INSERT_INST(RTS);

        INSERT_INST(SBC_IMM);
        INSERT_INST(SBC_ZP);
        INSERT_INST(SBC_ZPX);
        INSERT_INST(SBC_ABS);
        INSERT_INST(SBC_ABSX);
        INSERT_INST(SBC_ABSY);
        INSERT_INST(SBC_INDX);
        INSERT_INST(SBC_INDY);

        INSERT_INST(SEC);
        INSERT_INST(SED);
        INSERT_INST(SEI);

        INSERT_INST(STA_ZP);
        INSERT_INST(STA_ZPX);
        INSERT_INST(STA_ABS);
        INSERT_INST(STA_ABSX);
        INSERT_INST(STA_ABSY);
        INSERT_INST(STA_INDX);
        INSERT_INST(STA_INDY);

        INSERT_INST(STX_ZP);
        INSERT_INST(STX_ZPY);
        INSERT_INST(STX_ABS);

        INSERT_INST(STY_ZP);
        INSERT_INST(STY_ZPX);
        INSERT_INST(STY_ABS);

        INSERT_INST(TAX);
        INSERT_INST(TAY);
        INSERT_INST(TSX);
        INSERT_INST(TXA);
        INSERT_INST(TXS);
        INSERT_INST(TYA);
    }
#undef INSERT_INST

    return "";
}
