#pragma once

#include<stdint.h>
#include<string>
#include <cstring>
#include<unordered_map>
#include<Memory.h>

class CPU;

typedef struct {
	void (*function)(CPU&, uint8_t, std::string);
	std::string name;
	uint8_t cycles;
} inst_func_t;


class CPU {
	std::unordered_map<uint8_t, inst_func_t> isa_map;
	Memory m_memory;
	uint16_t m_program_size, m_cycles;
	void dump();
public:
	CPU(uint8_t* program, uint16_t size);
	uint8_t mem_read(uint16_t address);
	uint8_t mem_read(uint16_t address, uint8_t offset);
	uint8_t Fetch();
	void Execute();

	uint16_t get_cycles() { return m_cycles; }
	uint16_t get_pc() { return PC; }
	uint8_t get_ac() { return AC; }
	uint8_t get_x() { return X; }
	uint8_t get_y() { return Y; }
	uint8_t get_sr() {
		uint8_t val;
		std::memcpy(&val, &SR, sizeof(val));
		return val;
	}
	uint8_t get_sp() { return SP; }

	// Registers
	uint16_t PC;	// PC	program counter(16 bit)
	uint8_t AC;		// AC	accumulator(8 bit)
	uint8_t X;		// X register	(8 bit)
	uint8_t Y;		// Y register	(8 bit)
	struct {
		uint8_t N : 1;	// Negative
		uint8_t V : 1;	// Overflow
		uint8_t _ignore_ : 1;  // ignored
		uint8_t B : 1;	// Break
		uint8_t D : 1;	// Decimal(use BCD for arithmetics)
		uint8_t I : 1;	// Interrupt(IRQ disable)
		uint8_t Z : 1;	// Zero
		uint8_t C : 1;	// Carry
	} SR; // status register[NV - BDIZC](8 bit)
	uint16_t SP;		// stack pointer (8 bit)
};