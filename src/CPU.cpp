#include <CPU.h>
#include <assert.h>
#include <instructions.h>
#include <iostream>

/* CPU */
CPU::CPU(uint8_t* program, uint16_t size)
    : PC(0xFCFF), AC(0), X(0), Y(0), SR({0, 0, 0, 0, 0, 0, 0, 0}), SP(0x0100),
      m_program_size(size), m_cycles(0) {
    initialize_map(isa_map);
    PC = 0x0200; // FIXME: Program start in memory
    m_memory.write(PC, program, m_program_size);
}

void CPU::dump() {
    using namespace std;
    cout << hex;
    cout << "PC: 0x" << int(PC);
    cout << ", AC: 0x" << int(AC);
    cout << ", X: 0x" << int(X);
    cout << ", Y: 0x" << int(Y);
    cout << ", SP: 0x" << int(SP);
    cout << ", SR(NV_BDIZC): 0b" << int(SR.N) << int(SR.V) << 0 << int(SR.B)
         << int(SR.D) << int(SR.I) << int(SR.Z) << int(SR.C);
    cout << dec;
    cout << endl;
}

void CPU::Execute() {
    while ((PC - 0x0200) < m_program_size) {

        auto old_cycles = m_cycles;

        uint8_t op_code = this->Fetch();
        assert(isa_map.count(op_code) > 0);

        isa_map[op_code](*this, op_code);

        std::cout << ToString(static_cast<Instruction>(op_code)) << ":\t";
        std::cout << std::hex << "OpCode: 0x" << int(op_code) << std::dec;
        std::cout << ", Cycles: " << (m_cycles - old_cycles) << std::endl
                  << "\t\t";
        dump();
    }

    std::cout << m_cycles << " cycles were concumed." << std::endl;
}
