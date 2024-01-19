#pragma once

#include <Memory.h>
#include <cstring>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <utils.h>

class CPU;

using inst_func_t = void (*)(CPU&, uint8_t);

class CPU {
  public:
    // Registers
    uint16_t PC; // PC	program counter(16 bit)
    uint8_t AC;  // AC	accumulator(8 bit)
    uint8_t X;   // X register	(8 bit)
    uint8_t Y;   // Y register	(8 bit)

    struct {
        uint8_t N : 1;        // Negative
        uint8_t V : 1;        // Overflow
        uint8_t _ignore_ : 1; // ignored
        uint8_t B : 1;        // Break
        uint8_t D : 1;        // Decimal(use BCD for arithmetics)
        uint8_t I : 1;        // Interrupt(IRQ disable)
        uint8_t Z : 1;        // Zero
        uint8_t C : 1;        // Carry

        uint8_t Value() { return N << V << 1 << B << D << I << Z << C; }
    } SR; // status register[NV - BDIZC](8 bit)

    uint16_t SP; // stack pointer (8 bit)

  public:
    CPU(uint8_t* program, uint16_t size);

    uint8_t mem_read(uint16_t address) {
        m_cycles++;
        return m_memory.read(address);
    }

    void mem_write(uint16_t address, uint8_t data) {
        m_cycles++;
        return m_memory.write(address, data);
    }

    template <typename address_t>
    uint16_t get_address(address_t base_address, uint8_t offset,
                         bool force_cycle = false) {
        uint16_t address = base_address + offset;

        // FIXME: Not sure why, but it's needed for ZPX
        if constexpr (std::is_same_v<address_t, uint8_t>) {
            m_cycles++;
            address &= 0xFF;
        } else if constexpr (std::is_same_v<address_t, uint16_t>) {
            if (force_cycle || ((address >> 8) != (base_address >> 8))) {
                m_cycles++;
            }
        } else {
            ASSERT(0, "get_address address_t not compatible");
        }

        return address;
    }

  public:
    uint16_t GetCycles() { return m_cycles; }

    uint8_t GetSR() {
        uint8_t val = 0, tmp;
        std::memcpy(&tmp, &SR, sizeof(tmp));

        for (int i = 0; i < 8; i++) {
            val = (val << 1);
            val |= (tmp & 0x1);
            tmp = (tmp >> 1);
        }
        return val;
    }

  public:
    uint8_t Fetch();
    void Execute();

  private:
    Memory m_memory;
    std::unordered_map<uint8_t, inst_func_t> isa_map;
    uint16_t m_program_size, m_cycles;
    void dump();
};