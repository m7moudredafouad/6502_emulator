#include <CPU.h>
#include <gtest/gtest.h>
#include <instructions.h>

static void check_registers(CPU& cpu, uint16_t cycles, size_t program_size) {
    EXPECT_EQ(cpu.GetCycles(), cycles);
    EXPECT_EQ(cpu.PC, 0x200 + program_size);

    EXPECT_EQ(cpu.SR.N, ((cpu.AC & 0x80) > 0));
    EXPECT_EQ(cpu.SR.Z, (cpu.AC == 0));

    EXPECT_EQ(cpu.SR.V, 0);
    EXPECT_EQ(cpu.SR.B, 0);
    EXPECT_EQ(cpu.SR.D, 0);
    EXPECT_EQ(cpu.SR.I, 0);
    EXPECT_EQ(cpu.SR.C, 0);
}

TEST(LDATestSuite, BigProgram) {
    uint8_t program[] = {
        Instruction::LDA_IMM,
        0x00,
        Instruction::LDA_IMM,
        0x80,
        Instruction::LDA_ZP,
        0x01,
        Instruction::LDA_ZPX,
        0x00,
        Instruction::LDA_ABS,
        0x00,
        0x02,
    };

    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 15, sizeof(program));
    EXPECT_EQ(cpu.AC, 0xA9);
}

TEST(LDATestSuite, IMM) {
    uint8_t program[] = {Instruction::LDA_IMM, 0x80};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 2, sizeof(program));
    EXPECT_EQ(cpu.AC, 0x80);
}

TEST(LDATestSuite, ZP) {
    uint8_t program[] = {Instruction::LDA_ZP, 0x01};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 3, sizeof(program));
    EXPECT_EQ(cpu.AC, 0x0);
}

TEST(LDATestSuite, ZPX) {
    uint8_t program[] = {Instruction::LDA_ZPX, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program));
    EXPECT_EQ(cpu.AC, 0x0);
}

TEST(LDATestSuite, ABS) {
    uint8_t program[] = {Instruction::LDA_ABS, 0x00, 0x02};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program));
    EXPECT_EQ(cpu.AC, Instruction::LDA_ABS);
}

TEST(LDATestSuite, ABSX) {
    uint8_t program[] = {Instruction::LDA_ABSX, 0x00, 0x02};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program));
    EXPECT_EQ(cpu.AC, Instruction::LDA_ABSX);
}

TEST(LDATestSuite, ABSX_CROSS) {
    uint8_t program[] = {Instruction::LDA_ABSX, 0xFF, 0x44};
    CPU cpu(program, sizeof(program));
    cpu.X = 0x1;
    // 0x44FF + 1 cross page boundry
    cpu.Execute();
    check_registers(cpu, 5, sizeof(program));
}

TEST(LDATestSuite, ABSY) {
    uint8_t program[] = {Instruction::LDA_ABSY, 0x00, 0x02};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program));
    EXPECT_EQ(cpu.AC, Instruction::LDA_ABSY);
}

TEST(LDATestSuite, INDX) {
    uint8_t program[] = {Instruction::LDA_INDX, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 6, sizeof(program));
}

TEST(LDATestSuite, INDY) {
    uint8_t program[] = {Instruction::LDA_INDY, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 5, sizeof(program));
}