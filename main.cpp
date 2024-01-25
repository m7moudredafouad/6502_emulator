#include <CPU.h>
#include <fstream>
#include <iterator>
#include <vector>

int main(int argc, char** argv) {
    if (argc != 2) {
        ASSERT(0, "The emulator expects 1 bin file")
    }

    std::ifstream input(argv[1], std::ios::binary);

    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input),
                                      {});

    std::cout << std::hex;
    int i = 0;
    for (auto b : buffer) {
        i++;
        std::cout << int(b) << " ";
        if (i % 16 == 0)
            std::cout << "\n";
    }
    std::cout << "\n" << std::dec;

    CPU cpu(&buffer[0], buffer.size()); //, 0xFFFF - buffer.size());
    cpu.Execute();
    return 0;
}
