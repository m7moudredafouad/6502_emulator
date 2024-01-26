#include <addressing.h>
#include <utils.h>

uint16_t ADDR_IMM(CPU& cpu) { return cpu.PC++; }

uint16_t ADDR_ZP(CPU& cpu) { return cpu.Fetch(); }

uint16_t ADDR_ZPX(CPU& cpu) {
    ADD_CYCLE(cpu);
    uint16_t address = cpu.Fetch() + cpu.X;
    return address & 0x00FF;
}

uint16_t ADDR_ZPY(CPU& cpu) {
    ADD_CYCLE(cpu);
    uint16_t address = cpu.Fetch() + cpu.Y;
    return address & 0x00FF;
}

uint16_t ADDR_ABS(CPU& cpu) {
    uint8_t low = cpu.Fetch();
    uint8_t high = cpu.Fetch();
    return address_from_bytes(low, high);
}

uint16_t ADDR_ABSX(CPU& cpu, bool force_cycle) {
    uint8_t low = cpu.Fetch();
    uint8_t high = cpu.Fetch();

    uint16_t base_address = address_from_bytes(low, high);
    uint16_t address = base_address + cpu.X;

    if (force_cycle || ((address >> 8) != (base_address >> 8))) {
        ADD_CYCLE(cpu);
    }

    return address;
}

uint16_t ADDR_ABSY(CPU& cpu, bool force_cycle) {
    uint8_t low = cpu.Fetch();
    uint8_t high = cpu.Fetch();

    uint16_t base_address = address_from_bytes(low, high);
    uint16_t address = base_address + cpu.Y;

    if (force_cycle || ((address >> 8) != (base_address >> 8))) {
        ADD_CYCLE(cpu);
    }

    return address;
}

uint16_t ADDR_IND(CPU& cpu) {
    uint16_t abs_add = ADDR_ABS(cpu);
    uint8_t low = cpu.read(abs_add);
    uint8_t high = cpu.read(abs_add + 1);
    return address_from_bytes(low, high);
}

uint16_t ADDR_INDX(CPU& cpu) {
    uint16_t address = (uint16_t)cpu.Fetch() + (uint16_t)cpu.X;
    ADD_CYCLE(cpu);

    uint8_t low = cpu.read(address & 0x00FF);
    uint8_t high = cpu.read((address + 1) & 0x00FF);
    return address_from_bytes(low, high);
}

uint16_t ADDR_INDY(CPU& cpu, bool force_cycle) {
    uint16_t address = cpu.Fetch();
    uint8_t low = cpu.read(address & 0x00FF);
    uint8_t high = cpu.read((address + 1) & 0x00FF);

    uint16_t base_address = address_from_bytes(low, high);
    address = base_address + cpu.Y;

    if (force_cycle || ((address >> 8) != (base_address >> 8))) {
        ADD_CYCLE(cpu);
    }

    return address;
}