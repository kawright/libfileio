/* File input/output */

#ifndef FILEIO_H
#define FILEIO_H

#include <core.h>
#include <err.h>

/* Read a file into a destination buffer. Returns the number of bytes read, or
0 if there was an error. Throws IO and MEM. */
U64 read_file_to_str(Ch *path, Ch **dest, Err *err);

/* Write data into a file. Throws IO. */
Void write_file_to_str(Ch *path, Ch *data, U64 len, Err *err);

#endif
