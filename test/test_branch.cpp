#include <CPU.h>
#include <gtest/gtest.h>
#include <instructions.h>

static void check_registers(CPU& cpu, uint16_t cycles, uint16_t PC) {
    EXPECT_EQ(cpu.GetCycles(), cycles);
    EXPECT_EQ(cpu.PC, PC);
}

TEST(BranchTestSuite, BCCNoBranch) {
    uint8_t program[] = {Instruction::BCC, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.SR.C = 1;
    cpu.Execute();
    check_registers(cpu, 2, 0x200 + sizeof(program));
}

TEST(BranchTestSuite, BCCSamePage) {
    uint8_t program[] = {Instruction::BCC, 0x08};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 3, 0x200 + 0x08 + 0x2);
}

TEST(BranchTestSuite, BCCOtherPage) {
    uint8_t program[] = {Instruction::BCC, 0xFF};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 4, 0x200 + 0xFF + 0x2);
}


TEST(BranchTestSuite, BCSNoBranch) {
    uint8_t program[] = {Instruction::BCS, 0x00};
    CPU cpu(program, sizeof(program));
    cpu.Execute();
    check_registers(cpu, 2, 0x200 + sizeof(program));
}

TEST(BranchTestSuite, BCSSamePage) {
    uint8_t program[] = {Instruction::BCS, 0x08};
    CPU cpu(program, sizeof(program));
    cpu.SR.C = 1;
    cpu.Execute();
    check_registers(cpu, 3, 0x200 + 0x08 + 0x2);
}

TEST(BranchTestSuite, BCSOtherPage) {
    uint8_t program[] = {Instruction::BCS, 0xFF};
    CPU cpu(program, sizeof(program));
    cpu.SR.C = 1;
    cpu.Execute();
    check_registers(cpu, 4, 0x200 + 0xFF + 0x2);
}
