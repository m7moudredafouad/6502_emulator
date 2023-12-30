#include <CPU.h>
#include <gtest/gtest.h>
#include <instructions.h>

static void check_registers(CPU& cpu, uint16_t cycles, size_t program_size, bool C = false, bool V = false) {
    EXPECT_EQ(cpu.GetCycles(), cycles);
    EXPECT_EQ(cpu.PC, 0x200 + program_size);

    EXPECT_EQ(cpu.SR.N, ((cpu.AC & 0x80) > 0));
    EXPECT_EQ(cpu.SR.Z, (cpu.AC == 0));
    EXPECT_EQ(cpu.SR.C, C);
    EXPECT_EQ(cpu.SR.V, V);

    EXPECT_EQ(cpu.SR.B, 0);
    EXPECT_EQ(cpu.SR.D, 0);
    EXPECT_EQ(cpu.SR.I, 0);
}

TEST(ADCTestSuite, IMM) {
    uint8_t program[] = {Instruction::ADC_IMM, 0xFF};
    CPU cpu(program, sizeof(program));
    cpu.AC = 0xFF;
    cpu.Execute();
    check_registers(cpu, 2, sizeof(program), 1, 0);
    EXPECT_EQ(cpu.AC, 0xFE);
}

TEST(ADCTestSuite, ZP) {
    uint8_t program[] = {Instruction::ADC_ZP, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 3, sizeof(program));
    EXPECT_EQ(cpu.AC, 0x0);
}

TEST(ADCTestSuite, ZPX) {
    uint8_t program[] = {Instruction::ADC_ZPX, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.AC = 0xFF;
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program));
    EXPECT_EQ(cpu.AC, 0xFF);
}

TEST(ADCTestSuite, ABS) {
    uint8_t program[] = {Instruction::ADC_ABS, 0x00, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.AC = 0xFF;
    cpu.SR.C = 1;
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program), 1);
    EXPECT_EQ(cpu.AC, 0);
}

TEST(ADCTestSuite, ABSX) {
    uint8_t program[] = {Instruction::ADC_ABSX, 0x00, 0x02};
    CPU cpu(program, sizeof(program));
    cpu.AC = 0x7F;
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program), 0, 1);
    EXPECT_EQ(cpu.AC, 0xFC);
}

TEST(ADCTestSuite, ABSX_CROSS) {
    uint8_t program[] = {Instruction::ADC_ABSX, 0xFF, 0x44};
    CPU cpu(program, sizeof(program));
    cpu.X = 0x1;
    cpu.AC = 0x00;
    cpu.Execute();
    check_registers(cpu, 5, sizeof(program));
    EXPECT_EQ(cpu.AC, 0x00);
}

TEST(ADCTestSuite, ABSY) {
    uint8_t program[] = {Instruction::ADC_ABSY, 0x00, 0x02};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 4, sizeof(program));
    EXPECT_EQ(cpu.AC, Instruction::ADC_ABSY);
}

TEST(ADCTestSuite, INDX) {
    uint8_t program[] = {Instruction::ADC_INDX, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 6, sizeof(program));
    EXPECT_EQ(cpu.AC, 0);
}

TEST(ADCTestSuite, INDY) {
    uint8_t program[] = {Instruction::ADC_INDY, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 5, sizeof(program));
    EXPECT_EQ(cpu.AC, 0);
}