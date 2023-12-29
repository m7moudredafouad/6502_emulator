#include <CPU.h>
#include <gtest/gtest.h>
#include <instructions.h>

static void check_registers(CPU& cpu, uint16_t cycles, size_t program_size) {
    EXPECT_EQ(cpu.GetCycles(), cycles);
    EXPECT_EQ(cpu.PC, 0x200 + program_size);

    EXPECT_EQ(cpu.SR.N, ((cpu.Y & 0x80) > 0));
    EXPECT_EQ(cpu.SR.Z, (cpu.Y == 0));

    EXPECT_EQ(cpu.SR.V, 0);
    EXPECT_EQ(cpu.SR.B, 0);
    EXPECT_EQ(cpu.SR.D, 0);
    EXPECT_EQ(cpu.SR.I, 0);
    EXPECT_EQ(cpu.SR.C, 0);
}

TEST(LDYTestSuite, IMM) {
    uint8_t program[] = {Instruction::LDY_IMM, 0x80};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 2, sizeof(program));
    EXPECT_EQ(cpu.Y, 0x80);
}

TEST(LDYTestSuite, ZP) {
    uint8_t program[] = {Instruction::LDY_ZP, 0x01};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 3, sizeof(program));
    EXPECT_EQ(cpu.Y, 0x0);
}

TEST(LDYTestSuite, ZPX) {
    uint8_t program[] = {Instruction::LDY_ZPX, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program));
    EXPECT_EQ(cpu.X, 0x0);
}

TEST(LDYTestSuite, ABS) {
    uint8_t program[] = {Instruction::LDY_ABS, 0x00, 0x02};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program));
    EXPECT_EQ(cpu.Y, Instruction::LDY_ABS);
}

TEST(LDYTestSuite, ABSY) {
    uint8_t program[] = {Instruction::LDY_ABSX, 0x00, 0x02};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program));
    EXPECT_EQ(cpu.Y, Instruction::LDY_ABSX);
}
