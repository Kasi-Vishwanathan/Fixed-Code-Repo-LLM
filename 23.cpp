// 23.cpp

#include <zlib.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

struct GZCloser {
    void operator()(gzFile file) const {
        if (file) {
            gzclose(file);
        }
    }
};
using GZFilePtr = std::unique_ptr<gzFile_s, GZCloser>;

GZFilePtr openFileRead(const char* filename) {
    gzFile file = gzopen(filename, "rb");
    if (!file) {
        std::cerr << "Could not open " << filename << ": " << std::strerror(errno) << '\n';
        std::exit(EXIT_FAILURE);
    }
    return GZFilePtr(file);
}

void writePage(const char* filename, int page, const void* buffer, int len) {
    std::ostringstream tmpname_oss;
    tmpname_oss << filename << "." << page;
    std::string tmpname = tmpname_oss.str();

    std::unique_ptr<FILE, decltype(&fclose)> out(fopen(tmpname.c_str(), "wb"), &fclose);
    if (!out) {
        std::cerr << "Could not open " << tmpname << " for writing: " << std::strerror(errno) << '\n';
        return;
    }

    size_t written = fwrite(buffer, 1, len, out.get());
    if (written != static_cast<size_t>(len)) {
        std::cerr << "Error writing to " << tmpname << ": wrote " << written << " of " << len << " bytes\n";
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input.gz> <output_prefix>\n";
        return EXIT_FAILURE;
    }

    GZFilePtr file = openFileRead(argv[1]);
    std::array<char, 1024> buf{};
    int page = 0;
    int len;

    while ((len = gzread(file.get(), buf.data(), buf.size())) > 0) {
        writePage(argv[2], page++, buf.data(), len);
    }

    return EXIT_SUCCESS;
}
