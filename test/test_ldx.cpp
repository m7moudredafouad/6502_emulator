#include <CPU.h>
#include <gtest/gtest.h>
#include <instructions.h>

static void check_registers(CPU& cpu, uint16_t cycles, size_t program_size) {
    EXPECT_EQ(cpu.GetCycles(), cycles);
    EXPECT_EQ(cpu.PC, 0x00 + program_size);

    EXPECT_EQ(cpu.SR.N, ((cpu.X & 0x80) > 0));
    EXPECT_EQ(cpu.SR.Z, (cpu.X == 0));

    EXPECT_EQ(cpu.SR.V, 0);
    EXPECT_EQ(cpu.SR.B, 0);
    EXPECT_EQ(cpu.SR.D, 0);
    EXPECT_EQ(cpu.SR.I, 0);
    EXPECT_EQ(cpu.SR.C, 0);
}

TEST(LDXTestSuite, IMM) {
    uint8_t program[] = {Instruction::LDX_IMM, 0x80};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 2, sizeof(program));
    EXPECT_EQ(cpu.X, 0x80);
}

TEST(LDXTestSuite, ZP) {
    uint8_t program[] = {Instruction::LDX_ZP, 0x01};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 3, sizeof(program));
    EXPECT_EQ(cpu.X, 0x01);
}

TEST(LDXTestSuite, ZPY) {
    uint8_t program[] = {Instruction::LDX_ZPY, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program));
    EXPECT_EQ(cpu.X, Instruction::LDX_ZPY);
}

TEST(LDXTestSuite, ABS) {
    uint8_t program[] = {Instruction::LDX_ABS, 0x00, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program));
    EXPECT_EQ(cpu.X, Instruction::LDX_ABS);
}

TEST(LDXTestSuite, ABSY) {
    uint8_t program[] = {Instruction::LDX_ABSY, 0x00, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program));
    EXPECT_EQ(cpu.X, Instruction::LDX_ABSY);
}
