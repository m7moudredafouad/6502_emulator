#include <iostream>
#include <instructions.h>


/*
* Addressing Modes
* A			Accumulator				OPC A			operand is AC(implied single byte instruction)
* abs		absolute				OPC $LLHH		operand is address $HHLL*
* abs, X	absolute, X - indexed	OPC $LLHH, X	operand is address; effective address is address incremented by X with carry**
* abs, Y	absolute, Y - indexed	OPC $LLHH, Y	operand is address; effective address is address incremented by Y with carry**
* #			immediate				OPC #$BB		operand is byte BB
* impl		implied					OPC				operand implied
* ind		indirect				OPC($LLHH)		operand is address; effective address is contents of word at address : C.w($HHLL)
* X, ind	X - indexed, indirect	OPC($LL, X)		operand is zeropage address; effective address is word in(LL + X, LL + X + 1), inc.without carry : C.w($00LL + X)
* ind, Y	indirect, Y - indexed	OPC($LL), Y		operand is zeropage address; effective address is word in(LL, LL + 1) incremented by Y with carry : C.w($00LL) + Y
* rel		relative				OPC $BB			branch target is PC + signed offset BB * **
* zpg		zeropage				OPC $LL			operand is zeropage address(hi - byte is zero, address = $00LL)
* zpg, X	zeropage, X - indexed	OPC $LL, X		operand is zeropage address; effective address is address incremented by X without carry**
* zpg, Y	zeropage, Y - indexed	OPC $LL, Y		operand is zeropage address; effective address is address incremented by Y without carry**
*/

enum INSTRUCTIONS : uint8_t {
	LDA_IMM = 0xA9,
	LDA_ZP = 0xA5,
	LDA_ZPX = 0xB5,
	LDA_ABS = 0xAD,
	LDA_ABSX = 0xBD,
	LDA_ABSY = 0xB9,
	LDA_INDX = 0xA1,
	LDA_INDY = 0xB1,
};


void initialize_map(std::unordered_map<uint8_t, inst_func_t>& inst_map) {
	inst_map[LDA_IMM] = { LDA_INST, "lda_immediate", 2 };
	inst_map[LDA_ZP] = { LDA_INST, "lda_zeropage", 3 };
	inst_map[LDA_ZPX] = { LDA_INST, "lda_zeropage_x", 4 };
	inst_map[LDA_ABS] = { LDA_INST, "lda_absolute", 4 };
	inst_map[LDA_ABSX] = { LDA_INST, "lda_absolute_x", 4 };
	inst_map[LDA_ABSY] = { LDA_INST, "lda_absolute_y", 4 };
	inst_map[LDA_INDX] = { LDA_INST, "lda_indirect_x", 6 };
	inst_map[LDA_INDY] = { LDA_INST, "lda_indirect_y", 5 };
}

void LDA_INST(CPU& cpu, uint8_t op_code, std::string name) {
	switch (op_code) {
	case LDA_IMM: {
		cpu.AC = cpu.Fetch();
	} break;
	case LDA_ZP: {
		auto zp_addr = cpu.Fetch();
		cpu.AC = cpu.mem_read(zp_addr);
	} break;
	case LDA_ZPX: {
		auto zp_addr = cpu.Fetch();
		cpu.AC = cpu.mem_read(zp_addr, cpu.X);
	} break;
	case LDA_ABS: {
		uint16_t addr = cpu.Fetch();
		auto high = cpu.Fetch();
		addr |= (high << 8);
		cpu.AC = cpu.mem_read(addr);
	} break;
	default:
		exit(EXIT_FAILURE);
	}

	cpu.SR.N = ((cpu.AC & 0x80) > 0);
	cpu.SR.Z = cpu.AC == 0;
}