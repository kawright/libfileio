#include "fileio.h"

#include <core.h>
#include <err.h>
#include <mem.h>

#include <stdio.h>

U64 read_file_to_str(Ch *path, Ch **str, Err *err) {
    FILE        *fp;
    U64         sz;
    U64         bytes_read;
    fp = fopen(path, "r");
    if (fp == NIL) {
        THROW(err, ErrCode_IO, "Could not open file %s", path)
        return 0;
    }
    fseek(fp, 0L, SEEK_END); 
    sz = ftell(fp);
    rewind(fp);
    *str = alloc_mem(sz + 1, err);
    if (is_err(err))
        return 0;
    if ((bytes_read = fread(*str, 1, sz, fp)) != sz) {
        THROW(err, ErrCode_MEM, "Read %lld of %lld bytes from file %s", 
            bytes_read, sz, path)
        fclose(fp);
        free_mem(*str);
        return 0;
    }
    *str[sz] = '\0';
    return bytes_read;
}

Void write_str_to_file(Ch *path, Ch *str, Err *err) {
    FILE        *fp;
    fp = fopen(path, "w");
    if (fp == NIL) {
        THROW(err, ErrCode_IO, "Could not open file %s", path)
        return;
    }
    fprintf(fp, str);
    fclose(fp);
}

