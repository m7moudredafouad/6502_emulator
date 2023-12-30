#include <CPU.h>
#include <gtest/gtest.h>
#include <instructions.h>

static void check_registers(CPU& cpu, uint16_t cycles, size_t program_size) {
    EXPECT_EQ(cpu.GetCycles(), cycles);
    EXPECT_EQ(cpu.PC, 0x200 + program_size);

    EXPECT_EQ(cpu.SR.N, ((cpu.AC & 0x80) > 0));
    EXPECT_EQ(cpu.SR.Z, (cpu.AC == 0));

    EXPECT_EQ(cpu.SR.C, 0);
    EXPECT_EQ(cpu.SR.V, 0);
    EXPECT_EQ(cpu.SR.B, 0);
    EXPECT_EQ(cpu.SR.D, 0);
    EXPECT_EQ(cpu.SR.I, 0);
}

TEST(ANDTestSuite, IMM) {
    uint8_t program[] = {Instruction::AND_IMM, 0x80};
    CPU cpu(program, sizeof(program));
    cpu.AC = 0xFF;
    cpu.Execute();
    check_registers(cpu, 2, sizeof(program));
    EXPECT_EQ(cpu.AC, 0x80);
}

TEST(ANDTestSuite, ZP) {
    uint8_t program[] = {Instruction::AND_ZP, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 3, sizeof(program));
    EXPECT_EQ(cpu.AC, 0x0);
}

TEST(ANDTestSuite, ZPX) {
    uint8_t program[] = {Instruction::AND_ZPX, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.AC = 0xFF;
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program));
    EXPECT_EQ(cpu.AC, 0x00);
}

TEST(ANDTestSuite, ABS) {
    uint8_t program[] = {Instruction::AND_ABS, 0x00, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program));
    EXPECT_EQ(cpu.AC, 0x0);
}

TEST(ANDTestSuite, ABSX) {
    uint8_t program[] = {Instruction::AND_ABSX, 0x00, 0x02};
    CPU cpu(program, sizeof(program));
    cpu.AC = 0xFF;
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program));
    EXPECT_EQ(cpu.AC, Instruction::AND_ABSX);
}

TEST(ANDTestSuite, ABSX_CROSS) {
    uint8_t program[] = {Instruction::AND_ABSX, 0xFF, 0x44};
    CPU cpu(program, sizeof(program));
    cpu.X = 0x1;
    cpu.Execute();
    check_registers(cpu, 5, sizeof(program));
    EXPECT_EQ(cpu.AC, 0x00);
}

TEST(ANDTestSuite, ABSY) {
    uint8_t program[] = {Instruction::AND_ABSY, 0x00, 0x02};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program));
    EXPECT_EQ(cpu.AC, 0);
}

TEST(ANDTestSuite, INDX) {
    uint8_t program[] = {Instruction::AND_INDX, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 6, sizeof(program));
    EXPECT_EQ(cpu.AC, 0);
}

TEST(ANDTestSuite, INDY) {
    uint8_t program[] = {Instruction::AND_INDY, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 5, sizeof(program));
    EXPECT_EQ(cpu.AC, 0);
}