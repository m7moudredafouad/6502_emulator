#include <CPU.h>
#include <assert.h>
#include <instructions.h>
#include <iomanip>
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
    auto op_code = m_memory.read(PC);

    ASSERT(isa_map.count(op_code) > 0,
           "Couldn't find op_code " << std::hex << int(op_code) << std::dec);

    cout << left << hex << uppercase;
    cout << "0x" << setw(4) << int(PC) << ": 0x" << int(op_code);
    cout << " " << setw(9) << ToString(static_cast<Instruction>(op_code));

    cout << "[A: 0x" << setw(2) << int(AC);
    cout << ", X: 0x" << setw(2) << int(X);
    cout << ", Y: 0x" << setw(2) << int(Y);
    cout << ", SP: 0x" << setw(3) << int(SP);
    cout << ", SR(NV_BDIZC): 0b" << int(SR.N) << int(SR.V) << 1 << int(SR.B)
         << int(SR.D) << int(SR.I) << int(SR.Z) << int(SR.C) << "]";
    cout << nouppercase << dec << internal;

    cout << " " << m_cycles << endl;
}

void CPU::Execute() {
    auto first_pc = PC;
    while (PC >= first_pc && (PC - first_pc) < m_program_size) {
        dump();

        uint8_t op_code = this->Fetch();
        ASSERT(isa_map.count(op_code) > 0, "Couldn't find op_code "
                                               << std::hex << int(op_code)
                                               << std::dec);

        isa_map[op_code](*this, op_code);
    }

    std::cout << m_cycles << " cycles were concumed." << std::endl;
}
