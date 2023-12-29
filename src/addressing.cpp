#include <addressing.h>

uint16_t swap_bytes(uint16_t value) {
    uint16_t low = value & 0xFF;
    value = value >> 8;
    low = low << 8;
    return low | value;
}

uint8_t ZeroPageAddress(CPU& cpu) {
    uint8_t zp_addr = cpu.Fetch();
    return cpu.mem_read(zp_addr);
}

uint8_t ZeroPageXAddress(CPU& cpu) {
    uint8_t base_addr = cpu.Fetch();
    return cpu.mem_read(base_addr, cpu.X);
}

uint8_t ZeroPageYAddress(CPU& cpu) {
    uint8_t base_addr = cpu.Fetch();
    return cpu.mem_read(base_addr, cpu.Y);
}

uint8_t AbsoluteAddress(CPU& cpu) {
    uint16_t addr = cpu.Fetch<uint16_t>();
    return cpu.mem_read(swap_bytes(addr));
}

uint8_t AbsoluteXAddress(CPU& cpu) {
    uint16_t base_addr = cpu.Fetch<uint16_t>();
    return cpu.mem_read(swap_bytes(base_addr), cpu.X);
}

uint8_t AbsoluteYAddress(CPU& cpu) {
    uint16_t base_addr = cpu.Fetch<uint16_t>();
    return cpu.mem_read(swap_bytes(base_addr), cpu.Y);
}

uint8_t IndexedIndirectAddress(CPU& cpu) { // Indirect X
    uint16_t base_addr = cpu.Fetch();
    base_addr =
        cpu.mem_read<uint8_t, uint16_t>(static_cast<uint8_t>(base_addr), cpu.X);
    return cpu.mem_read(swap_bytes(base_addr));
}

uint8_t IndirectIndexedAddress(CPU& cpu) { // Indirect X
    uint16_t base_addr = cpu.Fetch();
    base_addr = cpu.mem_read<uint16_t>(base_addr);
    return cpu.mem_read(swap_bytes(base_addr), cpu.Y);
}