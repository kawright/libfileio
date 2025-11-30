/* File input/output */

#ifndef FILEIO_H
#define FILEIO_H

#include <core.h>
#include <err.h>

#include <stdarg.h>

#define FILE_ENTRY_NAME_BUF_SZ 256

/* Lists the type of file entries that a listing may contain. */
typedef enum ENUM_FILE_ENTRY_TYPE {
    FileEntryType_FILE,
    FileEntryType_DIR,
    FileEntryType_LINK,
    FileEntryType_OTHER
} FileEntryType;

/* Contains metadata about a single file system object (file, dir, link,
etc.) */
typedef struct STRUCT_FILE_ENTRY {
    FileEntryType       type;
    Ch                  name[FILE_ENTRY_NAME_BUF_SZ];
    U64                 sz;
} FileEntry;

/* A listing of information about files. */
typedef struct STRUCT_FILE_LIST {
    FileEntry           *contents;
    U16                 sz;
} FileList;

/* Build a path from multiple file parts, and write at most 'max' bytes of it
to 'out'. The final argument passed in should be a sentinal 'NIL'. Throws
BOUNDS. */
Void build_path(Ch *out, U16 max, Err *err, ...);

/* Get the size of a file, in bytes. Throws IO. */
U64 file_sz(Ch *path, Err *err);

/* Free heap data allocated to a 'FileList'. */
Void free_file_list(FileList *list);

/* Get the number of entries in a directory. Throws IO. */
U16 get_dir_sz(Ch *path, Err *err);

/* Get an entry from a file list by index. */
FileEntry *get_file_list_entry(FileList *list, U16 index, Err *err);

/* Initialize a 'FileEntry'. */
Void init_file_entry(FileEntry *entry);

/* Initialize a 'FileList'. */
Void init_file_list(FileList *list);

/* Load a 'FileList' from a given path. Throws IO, MEM. */
Void ld_file_list(FileList *list, Ch *path, Err *err);

/* Read a file into a destination buffer. Returns the number of bytes read, or
0 if there was an error. Throws IO and MEM. */
U64 read_file_to_str(Ch *path, Ch **dest, Err *err);

/* Write a string into a file. Throws IO. */
Void write_str_to_file(Ch *path, Ch *str, Err *err);

/* Write a formatted string into a file. Throws IO. */
Void write_fmt_to_file(Ch *path, Ch *fmt, Err *err, ...);

#endif

