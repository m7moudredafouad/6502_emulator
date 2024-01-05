#include <CPU.h>
#include <gtest/gtest.h>
#include <instructions.h>


TEST(BITTestSuite, TEST_BIT_ZP) {
    uint8_t program[] = {Instruction::BIT_ZP, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    EXPECT_EQ(cpu.GetCycles(), 3);
}

TEST(BITTestSuite, TEST_BIT_ABS) {
    uint8_t program[] = {Instruction::BIT_ABS, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    EXPECT_EQ(cpu.GetCycles(), 4);
}
