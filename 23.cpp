// File: 23.cpp
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Assume necessary zlib types are defined (e.g., typedefs for uLong, voidpf, ZCALLBACK)
typedef void* voidpf;
typedef unsigned long uLong;
#define ZCALLBACK

struct zlib_filefunc64_def {
    voidpf (ZCALLBACK *open64_file)(voidpf, const char*, int);
    int (ZCALLBACK *close_file)(voidpf, voidpf);
    uLong (ZCALLBACK *read_file)(voidpf, voidpf, voidpf, uLong);
    uLong (ZCALLBACK *write_file)(voidpf, voidpf, const voidpf, uLong);
    long (ZCALLBACK *tell64_file)(voidpf, voidpf);
    long (ZCALLBACK *seek64_file)(voidpf, voidpf, uLong, int);
    int (ZCALLBACK *test_error)(voidpf, voidpf);
    voidpf opaque;
};

static voidpf ZCALLBACK fopen64_file_func(const char* filename, const char* mode) {
    return std::fopen(filename, mode);
}

static int ZCALLBACK fclose_file_func(voidpf opaque, voidpf stream) {
    return std::fclose(static_cast<FILE*>(stream));
}

static uLong ZCALLBACK fread_file_func(voidpf opaque, voidpf stream, void* buf, uLong size) {
    size_t read = std::fread(buf, 1, size, static_cast<FILE*>(stream));
    return static_cast<uLong>(read);
}

static uLong ZCALLBACK fwrite_file_func(voidpf opaque, voidpf stream, const void* buf, uLong size) {
    size_t written = std::fwrite(buf, 1, size, static_cast<FILE*>(stream));
    return static_cast<uLong>(written);
}

static long ZCALLBACK ftell64_file_func(voidpf opaque, voidpf stream) {
    return static_cast<long>(std::ftell(static_cast<FILE*>(stream)));
}

static long ZCALLBACK fseek64_file_func(voidpf opaque, voidpf stream, uLong offset, int origin) {
    int seekOrigin = SEEK_SET;
    switch (origin) {
        case 1: seekOrigin = SEEK_CUR; break;
        case 2: seekOrigin = SEEK_END; break;
    }
    return std::fseek(static_cast<FILE*>(stream), static_cast<long>(offset), seekOrigin);
}

static int ZCALLBACK ferror_file_func(voidpf opaque, voidpf stream) {
    return std::ferror(static_cast<FILE*>(stream));
}

void fill_fopen64_filefunc(zlib_filefunc64_def* pzlib_filefunc_def) {
    // Initialize all fields to zero by value-initializing the struct
    *pzlib_filefunc_def = zlib_filefunc64_def{};
    pzlib_filefunc_def->open64_file = fopen64_file_func;
    pzlib_filefunc_def->close_file = fclose_file_func;
    pzlib_filefunc_def->read_file = fread_file_func;
    pzlib_filefunc_def->write_file = fwrite_file_func;
    pzlib_filefunc_def->tell64_file = ftell64_file_func;
    pzlib_filefunc_def->seek64_file = fseek64_file_func;
    pzlib_filefunc_def->test_error = ferror_file_func;
}
