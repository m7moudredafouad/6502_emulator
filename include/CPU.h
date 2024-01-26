#pragma once

#include <Memory.h>
#include <cstring>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <utils.h>

#define ADD_CYCLE(cpu) cpu.read(0)

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

        void Set(uint8_t val) {
            N = GET_BIT(val, 7);
            V = GET_BIT(val, 6);
            B = GET_BIT(val, 4);
            D = GET_BIT(val, 3);
            I = GET_BIT(val, 2);
            Z = GET_BIT(val, 1);
            C = GET_BIT(val, 0);
        }
    } SR; // status register[NV - BDIZC](8 bit)

    uint8_t SP; // stack pointer (8 bit)

  public:
    CPU(uint8_t* program, uint16_t size);

    /**
     * @brief CPU gives a signal to read from the bus
     * */
    uint8_t read(uint16_t address) {
        m_cycles++;
        return m_memory.read(address);
    }

    /**
     * @brief CPU gives a signal to write to the bus
     * */
    void write(uint16_t address, uint8_t data) {
        m_cycles++;
        return m_memory.write(address, data);
    }

  public:
    uint16_t GetCycles() { return m_cycles; }

    /**
     * @brief Pushing bytes to the stack causes the stack pointer to be
     * decremented.
     * */
    void PUSH(uint8_t val) { write(0x100 + (SP--), val); }

    /**
     * @brief Pulling bytes from stack causes it to be incremented.
     * */
    uint8_t POP() { return read(0x100 + (++SP)); }

    /**
     * @brief Fetch the next byte.
     * */
    uint8_t Fetch() { return read(PC++); }

    void Execute();

  private:
    Memory m_memory;
    std::unordered_map<uint8_t, inst_func_t> isa_map;
    uint16_t m_program_size, m_cycles;
    void dump();
};