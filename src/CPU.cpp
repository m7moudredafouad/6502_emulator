#include <CPU.h>
#include <assert.h>
#include <instructions.h>
#include <iostream>

/* CPU */
CPU::CPU(uint8_t* program, uint16_t size)
    : PC(0), AC(0), X(0), Y(0), SR({0, 0, 0, 0, 0, 0, 0, 0}), SP(0x01FF),
      m_program_size(size), m_cycles(0) {
    initialize_map(isa_map);
    uint16_t start_address = 0x8000;
    m_memory.write(start_address, program, m_program_size);
    m_memory.write(0xFFFC, 0x00);
    m_memory.write(0xFFFE, 0x80);
    PC = m_memory.read(0xFFFE);
    PC = (PC << 8) | m_memory.read(0xFFFC);
}

void CPU::dump() {
    using namespace std;
    cout << hex << uppercase;
    cout << "PC: 0x" << int(PC);
    cout << ", AC: 0x" << int(AC);
    cout << ", X: 0x" << int(X);
    cout << ", Y: 0x" << int(Y);
    cout << ", SP: 0x" << int(SP);
    cout << ", SR(NV_BDIZC): 0b" << int(SR.N) << int(SR.V) << 1 << int(SR.B)
         << int(SR.D) << int(SR.I) << int(SR.Z) << int(SR.C);
    cout << nouppercase << dec;
    cout << ", Cycles: " << m_cycles << endl;
}

void CPU::PUSH(uint8_t val) {
    // Pushing bytes to the stack causes the stack pointer to be decremented.
    mem_write(SP--, val);
}

uint8_t CPU::POP() {
    // pulling bytes causes it to be incremented.
    return mem_read(++SP);
}

uint8_t CPU::Fetch() { return this->mem_read(PC++); }

void CPU::Execute() {
    auto first_pc = PC;
    while (PC >= first_pc && (PC - first_pc) < m_program_size) {

        auto old_cycles = m_cycles;

        dump();

        uint8_t op_code = this->Fetch();
        ASSERT(isa_map.count(op_code) > 0, "Couldn't find op_code "
                                               << std::hex << int(op_code)
                                               << std::dec);

        isa_map[op_code](*this, op_code);

        std::cout << "\t\t---> ";
        std::cout << ToString(static_cast<Instruction>(op_code));
        std::cout << std::hex << std::uppercase << " [0x" << int(op_code) << "]"
                  << std::nouppercase << std::dec << std::endl;
    }

    std::cout << m_cycles << " cycles were concumed." << std::endl;
}
