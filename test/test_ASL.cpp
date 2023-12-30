#include <CPU.h>
#include <gtest/gtest.h>
#include <instructions.h>

static void check_registers(CPU& cpu, uint16_t cycles, size_t program_size, bool C = false) {
    EXPECT_EQ(cpu.GetCycles(), cycles);
    EXPECT_EQ(cpu.PC, 0x200 + program_size);

    // EXPECT_EQ(cpu.SR.N, ((cpu.Y & 0x80) > 0));
    // EXPECT_EQ(cpu.SR.Z, (cpu.Y == 0));
    EXPECT_EQ(cpu.SR.C, C);

    EXPECT_EQ(cpu.SR.V, 0);
    EXPECT_EQ(cpu.SR.B, 0);
    EXPECT_EQ(cpu.SR.D, 0);
    EXPECT_EQ(cpu.SR.I, 0);
}

TEST(ASLTestSuite, ACC) {
    uint8_t program[] = {Instruction::ASL_ACC};
    CPU cpu(program, sizeof(program));
    cpu.AC = 0x7F;
    cpu.Execute();
    check_registers(cpu, 2, sizeof(program));
    EXPECT_EQ(cpu.AC, 0xFE);
}

TEST(ASLTestSuite, ZP) {
    uint8_t program[] = {Instruction::ASL_ZP, 0x01};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 5, sizeof(program));
}

TEST(ASLTestSuite, ZPX) {
    uint8_t program[] = {Instruction::ASL_ZPX, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 6, sizeof(program));
}

TEST(ASLTestSuite, ABS) {
    uint8_t program[] = {Instruction::ASL_ABS, 0x00, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 6, sizeof(program));
}

TEST(ASLTestSuite, ABSX) {
    uint8_t program[] = {Instruction::ASL_ABSX, 0x00, 0x02};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 7, sizeof(program));
}
