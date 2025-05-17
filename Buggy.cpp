// File: Buggy.cpp
#include <zlib.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

std::vector<Bytef> compressData(const std::vector<Bytef>& input) {
    uLong sourceLen = input.size();
    if (sourceLen == 0) {
        return {}; // Handle empty input
    }

    uLong destLen = compressBound(sourceLen);
    std::vector<Bytef> dest(destLen);

    int result = compress(dest.data(), &destLen, input.data(), sourceLen);
    if (result != Z_OK) {
        throw std::runtime_error("Compression failed with error code: " + std::to_string(result));
    }

    dest.resize(destLen);
    return dest;
}

void processFile(std::istream& in, std::ostream& out) {
    constexpr size_t BUFFER_SIZE = 1024;
    std::vector<Bytef> buffer(BUFFER_SIZE);

    while (in) {
        in.read(reinterpret_cast<char*>(buffer.data()), BUFFER_SIZE);
        size_t bytesRead = in.gcount();

        if (bytesRead == 0) {
            break; // No more data to read
        }

        buffer.resize(bytesRead);
        std::vector<Bytef> compressed = compressData(buffer);
        out.write(reinterpret_cast<const char*>(compressed.data()), compressed.size());

        if (!out) {
            throw std::runtime_error("Failed to write to output file.");
        }

        buffer.resize(BUFFER_SIZE); // Reset buffer for next read
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input> <output>\n";
        return 1;
    }

    try {
        std::ifstream in(argv[1], std::ios::binary);
        if (!in) {
            throw std::runtime_error("Could not open input file: " + std::string(argv[1]));
        }

        std::ofstream out(argv[2], std::ios::binary);
        if (!out) {
            throw std::runtime_error("Could not open output file: " + std::string(argv[2]));
        }

        processFile(in, out);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}