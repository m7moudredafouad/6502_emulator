#pragma once

#include <stdint.h>
#include <unordered_map>

#define MEM_SIZE (1024 * 64)

class Memory {
  public:
    void write(uint16_t address, uint8_t* data, uint16_t size) {
        for (int i = 0; i < size; i++) {
            m_data[address + i] = data[i];
        }
    }

    void write(uint16_t address, uint8_t data) { m_data[address] = data; }

    uint8_t read(uint16_t address) { return m_data[address]; }

  private:
    uint8_t m_data[MEM_SIZE] = {0};
};
