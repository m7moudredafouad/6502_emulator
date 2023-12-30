#include <addressing.h>

static uint16_t address_from_bytes(uint8_t low, uint8_t high) {
    uint16_t address = high;
    address = address << 8;
    return address | low;
}

uint16_t ImmediateAddress(CPU& cpu) { return cpu.PC++; }

uint16_t ZeroPageAddress(CPU& cpu) { return cpu.Fetch(); }

uint16_t ZeroPageXAddress(CPU& cpu) {
    return cpu.get_address(cpu.Fetch(), cpu.X);
}

uint16_t ZeroPageYAddress(CPU& cpu) {
    return cpu.get_address(cpu.Fetch(), cpu.Y);
}

uint16_t AbsoluteAddress(CPU& cpu) {
    uint8_t low = cpu.Fetch();
    uint8_t high = cpu.Fetch();
    return address_from_bytes(low, high);
}

uint16_t AbsoluteXAddress(CPU& cpu, bool force_cycle) {
    uint8_t low = cpu.Fetch();
    uint8_t high = cpu.Fetch();
    return cpu.get_address(address_from_bytes(low, high), cpu.X, force_cycle);
}

uint16_t AbsoluteYAddress(CPU& cpu, bool force_cycle) {
    uint8_t low = cpu.Fetch();
    uint8_t high = cpu.Fetch();
    return cpu.get_address(address_from_bytes(low, high), cpu.Y, force_cycle);
}

uint16_t IndexedIndirectAddress(CPU& cpu) {
    uint16_t address = cpu.get_address(cpu.Fetch(), cpu.X);
    uint8_t low = cpu.mem_read(address++);
    uint8_t high = cpu.mem_read(address);

    return address_from_bytes(low, high);
}

uint16_t IndirectIndexedAddress(CPU& cpu) {
    uint16_t address = cpu.Fetch();
    uint8_t low = cpu.mem_read(address++);
    uint8_t high = cpu.mem_read(address);
    address = address_from_bytes(low, high);
    return cpu.get_address(address_from_bytes(low, high), cpu.Y);
}