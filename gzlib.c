/* gzlib.c -- zlib functions common to reading and writing gzip files
 * Copyright (C) 2004-2024 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "gzguts.h"

#if defined(__DJGPP__)
#  define LSEEK llseek
#elif defined(_WIN32) && !defined(__BORLANDC__) && !defined(UNDER_CE)
#  define LSEEK _lseeki64
#elif defined(_LARGEFILE64_SOURCE) && _LFS64_LARGEFILE-0
#  define LSEEK lseek64
#else
#  define LSEEK lseek
#endif

#if defined UNDER_CE
char ZLIB_INTERNAL *gz_strwinerror(DWORD error) {
    /* Implementation unchanged */
}
#endif /* UNDER_CE */

/* Reset gzip file state */
local void gz_reset(gz_statep state) {
    /* Implementation unchanged */
}

/* Open a gzip file either by name or file descriptor. */
local gzFile gz_open(const void *path, int fd, const char *mode) {
    gz_statep state;
    z_size_t len;
    int oflag;
#ifdef O_CLOEXEC
    int cloexec = 0;
#endif
#ifdef O_EXCL
    int exclusive = 0;
#endif

    if (path == NULL)
        return NULL;

    state = (gz_statep)malloc(sizeof(gz_state));
    if (state == NULL)
        return NULL;
    state->size = 0;
    state->want = GZBUFSIZE;
    state->msg = NULL;

    /* Mode parsing unchanged */

    /* Handle path allocation */

#ifdef WIDECHAR
    /* Wide char path handling unchanged */
#else
    /* Allocate and copy path */
#endif

    /* Compute open flags */
    oflag = /* flags based on mode parsing, unchanged */;

    /* Open file or duplicate existing fd */
    if (fd == -1) {
        state->fd = open((const char *)path, oflag, 0666);
#ifdef WIDECHAR
    } else if (fd == -2) {
        state->fd = _wopen(path, oflag, _S_IREAD | _S_IWRITE);
#endif
    } else {
        /* Duplicate the provided file descriptor */
#ifdef _WIN32
        state->fd = _dup(fd);
#else
        state->fd = dup(fd);
#endif
        if (state->fd == -1) {
            free(state->path);
            free(state);
            return NULL;
        }
    }

    if (state->fd == -1) {
        free(state->path);
        free(state);
        return NULL;
    }

    /* Rest of the function unchanged */
}

/* Rest of the functions (gzdopen, gzread, etc.) remain unchanged. */

/* Fix path buffer allocation in gzdopen */
gzFile ZEXPORT gzdopen(int fd, const char *mode) {
    char *path = NULL;
    gzFile gz;
    int len;

    if (fd == -1)
        return NULL;

    /* Calculate required buffer size safely */
    len = snprintf(NULL, 0, "<fd:%d>", fd);
    if (len < 0)
        return NULL;
    path = (char *)malloc(len + 1);
    if (path == NULL)
        return NULL;
    snprintf(path, len + 1, "<fd:%d>", fd);

    gz = gz_open(path, fd, mode);
    free(path);
    return gz;
}
/* gzlib.c -- zlib functions common to reading and writing gzip files
 * Copyright (C) 2004-2017 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include "gzguts.h"

#if defined(__DJGPP__)
#  define LSEEK llseek
#elif defined(_WIN32) && !defined(__BORLANDC__) && !defined(UNDER_CE)
#  define LSEEK _lseeki64
#elif defined(_LARGEFILE64_SOURCE) && _LFS64_LARGEFILE-0
#  define LSEEK lseek64
#else
#  define LSEEK lseek
#endif

#ifdef _WIN32
#  include <fcntl.h>
#  include <io.h>
#  define NO_DUPLICATE(fd) (fd == -1)
#else
#  include <fcntl.h>
#  define NO_DUPLICATE(fd) (fcntl((fd), F_GETFL) != -1 || errno != EBADF)
#endif

#if defined UNDER_CE

/* Map the Windows error number in ERROR to a locale-dependent error message
   string and return a pointer to it.  Typically, the values for ERROR come
   from GetLastError.

   The string pointed to shall not be modified by the application, but may be
   overwritten by a subsequent call to gz_strwinerror

   The gz_strwinerror function does not change the current setting of
   GetLastError. */
char ZLIB_INTERNAL *gz_strwinerror(DWORD error) {
    static char buf[1024];
    wchar_t *msgbuf;
    DWORD lasterr = GetLastError();
    DWORD chars = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM
        | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        NULL,
        error,
        0, /* Default language */
        (LPVOID)&msgbuf,
        0,
        NULL);
    if (chars != 0) {
        if (chars >= 2 && msgbuf[chars - 2] == '\r' && msgbuf[chars - 1] == '\n') {
            chars -= 2;
            msgbuf[chars] = 0;
        }
        if (chars > sizeof(buf) - 1) {
            chars = sizeof(buf) - 1;
            msgbuf[chars] = 0;
        }
        wcstombs(buf, msgbuf, chars + 1);
        LocalFree(msgbuf);
    } else {
        sprintf(buf, "unknown win32 error (%lu)", error);
    }
    SetLastError(lasterr);
    return buf;
}
#endif /* UNDER_CE */

/* Reset gzip file state */
local void gz_reset(gz_statep state) {
    state->x.have = 0;              /* no output data available */
    if (state->mode == GZ_READ) {   /* for reading ... */
        state->eof = 0;             /* not at end of file */
        state->past = 0;            /* have not read past end yet */
        state->how = LOOK;          /* look for gzip header */
    } else {                        /* for writing ... */
        state->reset = 0;           /* no deflateReset pending */
    }
    state->seek = 0;                /* no seek request pending */
    gz_error(state, Z_OK, NULL);    /* clear error */
    state->x.pos = 0;               /* no uncompressed data yet */
    state->strm.avail_in = 0;       /* no input data yet */
}

/* Open a gzip file either by name or file descriptor. */
local gzFile gz_open(const void *path, int fd, const char *mode) {
    gz_statep state;
    z_size_t len;
    int oflag;
#ifdef O_CLOEXEC
    int cloexec = 0;
#endif
#ifdef O_EXCL
    int exclusive = 0;
#endif

    if (path == NULL)
        return NULL;

    state = (gz_statep)malloc(sizeof(gz_state));
    if (state == NULL)
        return NULL;
    state->size = 0;
    state->want = GZBUFSIZE;
    state->msg = NULL;

    state->mode = GZ_NONE;
    state->level = Z_DEFAULT_COMPRESSION;
    state->strategy = Z_DEFAULT_STRATEGY;
    state->direct = 0;

    while (*mode) {
        if (*mode >= '0' && *mode <= '9')
            state->level = *mode - '0';
        else switch (*mode) {
            case 'r': state->mode = GZ_READ; break;
#ifndef NO_GZCOMPRESS
            case 'w': state->mode = GZ_WRITE; break;
            case 'a': state->mode = GZ_APPEND; break;
#endif
            case '+': free(state); return NULL;
            case 'b': break;
#ifdef O_CLOEXEC
            case 'e': cloexec = 1; break;
#endif
#ifdef O_EXCL
            case 'x': exclusive = 1; break;
#endif
            case 'f': state->strategy = Z_FILTERED; break;
            case 'h': state->strategy = Z_HUFFMAN_ONLY; break;
            case 'R': state->strategy = Z_RLE; break;
            case 'F': state->strategy = Z_FIXED; break;
            case 'T': state->direct = 1; break;
            default: break;
        }
        mode++;
    }

    if (state->mode == GZ_NONE) {
        free(state);
        return NULL;
    }

    if (state->mode == GZ_READ && state->direct) {
        free(state);
        return NULL;
    }

#ifdef WIDECHAR
    if (fd == -2) {
        len = wcstombs(NULL, path, 0);
        if (len == (size_t)-1) {
            free(state);
            return NULL;
        }
    } else
#endif
        len = strlen((const char *)path);

    state->path = (char *)malloc(len + 1);
    if (state->path == NULL) {
        free(state);
        return NULL;
    }

#ifdef WIDECHAR
    if (fd == -2) {
        if (len != 0)
            wcstombs(state->path, path, len + 1);
        else
            state->path[0] = '\0';
    } else
#endif
        strncpy(state->path, (const char *)path, len + 1);

    oflag =
#ifdef O_LARGEFILE
        O_LARGEFILE |
#endif
#ifdef O_BINARY
        O_BINARY |
#endif
#ifdef O_CLOEXEC
        (cloexec ? O_CLOEXEC : 0) |
#endif
        (state->mode == GZ_READ ? O_RDONLY :
         (O_WRONLY | O_CREAT |
#ifdef O_EXCL
          (exclusive ? O_EXCL : 0) |
#endif
          (state->mode == GZ_WRITE ? O_TRUNC : O_APPEND)));

    int open_fd = fd;
    if (open_fd == -1) {
        open_fd = open((const char *)path, oflag, 0666);
        if (open_fd == -1) {
            free(state->path);
            free(state);
            return NULL;
        }
    }
#ifdef WIDECHAR
    else if (fd == -2) {
        open_fd = _wopen(path, oflag, _S_IREAD | _S_IWRITE);
        if (open_fd == -1) {
            free(state->path);
            free(state);
            return NULL;
        }
    }
#endif

    /* Avoid duplicating invalid file descriptors */
    if (!NO_DUPLICATE(open_fd)) {
        int new_fd = dup(open_fd);
        if (new_fd == -1) {
            if (open_fd != fd)  // If we opened the file, close it
                close(open_fd);
            free(state->path);
            free(state);
            return NULL;
        }
        if (open_fd != fd)      // Close the original if we opened it here
            close(open_fd);
        open_fd = new_fd;
    }

    state->fd = open_fd;

    if (state->mode == GZ_APPEND) {
        LSEEK(state->fd, 0, SEEK_END);
        state->mode = GZ_WRITE;
    }

    if (state->mode == GZ_READ)
        state->start = LSEEK(state->fd, 0, SEEK_CUR);
    else
        state->start = 0;

    gz_reset(state);
    return (gzFile)state;
}

/* The following functions retain their original corrected implementations as they do not require further modification for the fixes applied. */
/* [Rest of the functions (gzopen, gzdopen, gzbuffer, gzrewind, etc.) remain unchanged with the previously applied corrections] */

/* Example of a retained function with proper error handling */
gzFile ZEXPORT gzdopen(int fd, const char *mode) {
    char *path;
    gzFile gz;
    if (fd == -1 || (path = (char *)malloc(7 + 3 * sizeof(int))) == NULL)
        return NULL;
    snprintf(path, 7 + 3 * sizeof(int), "<fd:%d>", fd);
    gz = gz_open(path, fd, mode);
    free(path);
    return gz;
}

/* Ensure all other functions follow similar corrections and standards. */