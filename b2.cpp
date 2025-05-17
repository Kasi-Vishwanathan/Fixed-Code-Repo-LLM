// b2.cpp
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <stdexcept>

using namespace std;

void processFile(istream& input) {
    vector<unsigned char> buffer(1024);
    size_t total = 0;
    
    while (true) {
        input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        size_t bytesRead = input.gcount();
        
        if (input.bad()) {
            throw runtime_error("Error reading file");
        }

        for (size_t i = 0; i < bytesRead; ++i) {
            total += buffer[i];
        }

        if (bytesRead < buffer.size()) {
            if (input.eof()) {
                break;
            }
        }
    }

    cout << "Total: " << total << endl;
}

int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            processFile(cin);
        } else {
            for (int i = 1; i < argc; ++i) {
                ifstream file(argv[i], ios::binary);
                if (!file.is_open()) {
                    cerr << "Could not open file: " << argv[i] << endl;
                    return 1;
                }
                processFile(file);
            }
        }
        return 0;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
}
// File: b2.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>

using namespace std;

void processFile(istream& input) {
    vector<unsigned char> buffer(1024);
    size_t total = 0;
    
    while (true) {
        input.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        size_t bytesRead = input.gcount();
        
        if (input.bad()) {
            throw runtime_error("Error reading file");
        }
        
        if (input.fail() && !input.eof()) {
            throw runtime_error("Error reading file");
        }

        for (size_t i = 0; i < bytesRead; ++i) {
            total += buffer[i];
        }

        if (bytesRead < buffer.size()) {
            break;
        }
    }

    cout << "Total: " << total << endl;
}

int main(int argc, char** argv) {
    int exit_code = 0;
    try {
        if (argc < 2) {
            processFile(cin);
        } else {
            for (int i = 1; i < argc; ++i) {
                ifstream file(argv[i], ios::binary);
                if (!file) {
                    cerr << "Could not open file: " << argv[i] << endl;
                    exit_code = 1;
                    continue;
                }
                try {
                    processFile(file);
                } catch (const exception& e) {
                    cerr << "Error processing file " << argv[i] << ": " << e.what() << endl;
                    exit_code = 1;
                }
            }
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        exit_code = 1;
    }
    return exit_code;
}
// b2.cpp
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <stdexcept>

void process_block(const void* buf, size_t len) {
    // Use vector for automatic memory management
    std::vector<uint8_t> outbuf;
    try {
        outbuf.resize(len * 2);
    } catch (const std::bad_alloc&) {
        std::cerr << "Memory allocation failed\n";
        throw;
    }

    const uint8_t* input = static_cast<const uint8_t*>(buf);
    
    // Example processing: XOR each byte with 0x55
    for (size_t i = 0; i < len; ++i) {
        outbuf[i] = input[i] ^ 0x55;
    }

    // Simulated error check
    if (len > 0 && input[0] == 0xFF) {
        throw std::runtime_error("Invalid input data detected");
    }

    // Processed data remains in outbuf for further use
}

int main() {
    try {
        std::vector<uint8_t> data = {0x00, 0x01, 0x02, 0xFF};
        process_block(data.data(), data.size());
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
