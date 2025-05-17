// File: dac.cpp

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>

struct DACSetup {
    int dac_ID;
    const char* user_code;
    void (*send_function)(const char*, int);

    DACSetup(int ID, const char* code, void (*sf)(const char*, int) = nullptr)
        : dac_ID(ID), user_code(code), send_function(sf) {}
};

void InitializeDAC(DACSetup* dac) {
    if (!dac) return;

    std::string user_code = dac->user_code ? dac->user_code : "";
    // Truncate if necessary to fit buffer
    if (user_code.length() >= 256) {
        user_code.resize(255);
    }

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%s", user_code.c_str());
    
    std::printf("DAC initialized with code: %s\n", buffer);
    
    if (dac->send_function) {
        dac->send_function(buffer, dac->dac_ID);
    }
}

int main() {
    auto dac = std::make_unique<DACSetup>(42, "TEST");
    InitializeDAC(dac.get());
    
    // Example of setting a send function
    // dac->send_function = [](const char* data, int id) { /* implementation */ };
    
    return 0;
}
// dac.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <zlib.h>

constexpr int MAX_MEM_LEVEL = 8;
constexpr int BUFSIZE = 1024;

// Read the zlib header from an input stream
int readZlibHeader(std::istream& is) {
    int cmf = is.get();
    int flags = is.get();

    if (cmf == EOF || flags == EOF) {
        std::cerr << "Error: Failed to read zlib header." << std::endl;
        return 1;
    }

    if ((cmf & 0x0F) != Z_DEFLATED) {
        std::cerr << "Error: Invalid compression method." << std::endl;
        return 1;
    }

    if (((cmf << 8) + flags) % 31 != 0) {
        std::cerr << "Error: Bad FCHECK value." << std::endl;
        return 1;
    }

    return 0;
}

// Compress data from input stream and write to output file
void TestCompress(std::istream& in, int level, int strategy) {
    z_stream strm{};
    int ret = deflateInit2(&strm, level, Z_DEFLATED, MAX_WBITS, MAX_MEM_LEVEL, strategy);
    if (ret != Z_OK) {
        std::cerr << "deflateInit2 error: " << ret << std::endl;
        return;
    }

    std::vector<Byte> in_buf(BUFSIZE);
    std::vector<Byte> out_buf(BUFSIZE);
    std::ofstream outfile("test.z", std::ios::binary);

    if (!outfile) {
        std::cerr << "Error opening output file." << std::endl;
        deflateEnd(&strm);
        return;
    }

    int flush = Z_NO_FLUSH;
    do {
        in.read(reinterpret_cast<char*>(in_buf.data()), in_buf.size());
        strm.avail_in = static_cast<uInt>(in.gcount());
        flush = in.eof() ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in_buf.data();

        do {
            strm.avail_out = out_buf.size();
            strm.next_out = out_buf.data();

            ret = deflate(&strm, flush);
            if (ret == Z_STREAM_ERROR) {
                std::cerr << "deflate error: " << ret << std::endl;
                deflateEnd(&strm);
                return;
            }

            size_t have = out_buf.size() - strm.avail_out;
            outfile.write(reinterpret_cast<const char*>(out_buf.data()), have);
        } while (strm.avail_out == 0);
    } while (flush != Z_FINISH);

    deflateEnd(&strm);

    if (ret != Z_STREAM_END) {
        std::cerr << "Compression incomplete. Error: " << ret << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ifstream in(argv[1], std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    if (readZlibHeader(in) != 0) {
        return 1;
    }

    TestCompress(in, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);

    return 0;
}
