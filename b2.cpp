#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

using uchar = unsigned char;

int b2GenSeq(uchar* table, int offs, int s, int n) {
    int j, cnt;
    if (!table)
        table = new (std::nothrow) uchar[257]();
    if (!table) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    for (j = 0, cnt = 0; s; j = (j << 1) | (s & 1), s >>= 1) {
        if (j >= 256)
            continue;
        table[j + offs] = static_cast<uchar>(n);
        cnt++;
    }
    return cnt;
}

std::unique_ptr<uchar[]> gen_codes() {
    auto htable = std::unique_ptr<uchar[]>(new (std::nothrow) uchar[65536]());
    if (!htable) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    constexpr int n = 5;
    uchar* table = nullptr;
    for (int i = 0; i < 3; i++) {
        int offs = (i == 1) ? 1 : 0;
        int s = 1 << n;
        b2GenSeq(table, offs, s, n);
    }

    return htable;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    auto htable = gen_codes();

    std::string out_filename = std::string(argv[1]) + ".out";
    FILE* f = std::fopen(out_filename.c_str(), "wb");
    if (!f) {
        perror(out_filename.c_str());
        return EXIT_FAILURE;
    }
    std::fwrite(htable.get(), 1, 65536, f);
    std::fclose(f);

    return EXIT_SUCCESS;
}
