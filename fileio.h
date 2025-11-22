/* File input/output */

#ifndef FILEIO_H
#define FILEIO_H

#include <core.h>
#include <err.h>

#include <stdarg.h>

/* Read a file into a destination buffer. Returns the number of bytes read, or
0 if there was an error. Throws IO and MEM. */
U64 read_file_to_str(Ch *path, Ch **dest, Err *err);

/* Write a string into a file. Throws IO. */
Void write_str_to_file(Ch *path, Ch *str, Err *err);

/* Write a formatted string into a file. Throws IO. */
Void write_fmt_to_file(Ch *path, Ch *fmt, Err *err, ...);

#endif
