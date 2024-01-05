#include <CPU.h>
#include <gtest/gtest.h>
#include <instructions.h>


TEST(StatusTestSuite, TEST_CLC) {
    uint8_t program[] = {Instruction::CLC};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    EXPECT_EQ(cpu.GetCycles(), 2);
    EXPECT_EQ(cpu.SR.C, 0);
}

TEST(StatusTestSuite, TEST_CLD) {
    uint8_t program[] = {Instruction::CLD};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    EXPECT_EQ(cpu.GetCycles(), 2);
    EXPECT_EQ(cpu.SR.D, 0);
}

TEST(StatusTestSuite, TEST_CLI) {
    uint8_t program[] = {Instruction::CLI};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    EXPECT_EQ(cpu.GetCycles(), 2);
    EXPECT_EQ(cpu.SR.I, 0);
}

TEST(StatusTestSuite, TEST_CLV) {
    uint8_t program[] = {Instruction::CLV};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    EXPECT_EQ(cpu.GetCycles(), 2);
    EXPECT_EQ(cpu.SR.V, 0);
}
