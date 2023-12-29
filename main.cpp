#include <CPU.h>

int main() {
	uint8_t program[] = {	0xA9, 0x00,	// LDA_IMM
							0xA9, 0x80,	// LDA_IMM
							0xA5, 0x01,	// LDA_ZP
							0xB5, 0x00,	// LDA_ZPX
							0xAD, 0x00, 0x02, // LDA_ABS
						};

	CPU cpu(program, sizeof(program));
	cpu.Execute();
	return 0;
}