#include <gtest/gtest.h>
#include <CPU.h>

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(TestLDAInstruction, LDA_IMM) {
    {

        uint8_t program[] = {0xA9, 0x00};
        CPU cpu(program, sizeof(program));
        cpu.Execute();
        EXPECT_EQ(cpu.get_cycles(), 2);
        EXPECT_EQ(cpu.get_pc(), 0x202);
        EXPECT_EQ(cpu.get_sr(), 0b00000010);
    }

    {
        uint8_t program[] = {0xA9, 0x80};
        CPU cpu(program, sizeof(program));
        cpu.Execute();
        EXPECT_EQ(cpu.get_cycles(), 2);
        EXPECT_EQ(cpu.get_pc(), 0x202);
        EXPECT_EQ(cpu.get_sr(), 0b10000000);
    }
}

TEST(TestLDAInstruction, LDA_ZP) {
    uint8_t program[] = {0xA5, 0x01};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    EXPECT_EQ(cpu.get_cycles(), 3);
}

TEST(TestLDAInstruction, LDA_ZPX) {
    uint8_t program[] = {0xB5, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    EXPECT_EQ(cpu.get_cycles(), 4);
}

TEST(TestLDAInstruction, LDA_ABS) {
    uint8_t program[] = {0xAD, 0x00, 0x02};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    EXPECT_EQ(cpu.get_cycles(), 4);
}