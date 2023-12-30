#include <addressing.h>

static uint16_t address_from_bytes(uint8_t low, uint8_t high) {
    uint16_t address = high;
    address = address << 8;
    return address | low;
}

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

uint16_t AbsoluteXAddress(CPU& cpu) {
    uint8_t low = cpu.Fetch();
    uint8_t high = cpu.Fetch();
    return cpu.get_address(address_from_bytes(low, high), cpu.X);
}

uint16_t AbsoluteYAddress(CPU& cpu) {
    uint8_t low = cpu.Fetch();
    uint8_t high = cpu.Fetch();
    return cpu.get_address(address_from_bytes(low, high), cpu.Y);
}

uint16_t IndexedIndirectAddress(CPU& cpu) {
    // uint16_t base_addr = cpu.Fetch();
    // base_addr =
    //     cpu.mem_read<uint8_t, uint16_t>(static_cast<uint8_t>(base_addr),
    //     cpu.X);
    // return cpu.mem_read(swap_bytes(base_addr));
    uint16_t address = cpu.get_address(cpu.Fetch(), cpu.X);
    uint8_t low = cpu.mem_read(address++);
    uint8_t high = cpu.mem_read(address);

    return address_from_bytes(low, high);
}

uint16_t IndirectIndexedAddress(CPU& cpu) {
    // uint16_t base_addr = cpu.Fetch();
    // base_addr = cpu.mem_read<uint16_t>(base_addr);
    // return cpu.mem_read(swap_bytes(base_addr), cpu.Y);

    uint16_t address = cpu.Fetch();
    uint8_t low = cpu.mem_read(address++);
    uint8_t high = cpu.mem_read(address);
    address = address_from_bytes(low, high);
    return cpu.get_address(address_from_bytes(low, high), cpu.Y);
}