#include "fileio.h"

#include <core.h>
#include <err.h>
#include <mem.h>

#include <dirent.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

Void build_path(Ch *out, U16 max, Err *err, ...) {
    va_list     args;
    va_start(args, err);
    Ch *last_arg = va_arg(args, Ch*);
    U16 i = 0;
    while (last_arg != NIL) {
        U16 j = 0;
        Ch last_ch = last_arg[j];
        while (last_ch != '\0') {
            out[i] = last_ch;
            i++;
            if (i >= max) {
                THROW(err, ErrCode_BOUNDS, "Path exceeds max length of %d",
                    max)
                goto CLEANUP;
            }
            j++;
            last_ch = last_arg[j];
        }
        out[i] = '/';
        i++;
        if (i >= max) {
            THROW(err, ErrCode_BOUNDS, "Path exceeds max length of %d", max)
            goto CLEANUP;
        }
        last_arg = va_arg(args, Ch*);
    }

    CLEANUP:
    va_end(args);
}

U64 file_sz(Ch *path, Err *err) {
    FILE        *fp;
    U64         ret_data;
    fp = fopen(path, "r");
    if (fp == NIL) {
        THROW(err, ErrCode_IO, "Could not open file %s", path)
        return 0;
    }
    fseek(fp, 0L, SEEK_END); 
    ret_data = ftell(fp);
    fclose(fp);
    return ret_data;
}

Void free_file_list(FileList *list) {
	free_mem(list->contents);
	list->contents = NIL;
}

U16 get_dir_sz(Ch *path, Err *err) {
	DIR 				*dir_handle;
	struct dirent		*dir_obj;
	U16					ret_data;
	dir_handle 			= opendir(path);
	ret_data			= 0;
	if (dir_handle == NIL) {
		THROW(err, ErrCode_IO, "Could not query contents of dir %s", path)
		return 0;
	}
	while ((dir_obj = readdir(dir_handle)) != NIL) {
		ret_data++;
	}
	return ret_data;
}

FileEntry *get_file_list_entry(FileList *list, U16 index, Err *err) {
	if (index >= list->sz) {
		THROW(err, ErrCode_BOUNDS, 
			"Out-of-bounds file list access at index %ld", index)
		return NIL;
	}
	return (FileEntry *) list->contents + index;
}

Void init_file_entry(FileEntry *entry) {
	entry->type 			= FileEntryType_FILE;
	entry->name[0]			= '\0';
	entry->sz				= 0;
}

Void init_file_list(FileList *list) {
	list->contents			= NIL;
	list->sz				= 0;
}

Void ld_file_list(FileList *list, Ch *path, Err *err) {
	DIR 				*dir_handle;
	struct dirent		*dir_obj;
	U16					dir_sz;
	dir_sz = get_dir_sz(path, err);
	if (is_err(err))
		return;
	list->sz = dir_sz;
	list->contents = alloc_mem(dir_sz * sizeof(FileEntry), err);
	if (is_err(err))
		goto CLEANUP;
	dir_handle = opendir(path);
	if (dir_handle == NIL) {
		THROW(err, ErrCode_IO, "Could not query contents of dir %s", path)
		goto CLEANUP;
	}
	U16 index = 0;
	while ((dir_obj = readdir(dir_handle)) != NIL) {
		FileEntry *temp_entry = (FileEntry*) list->contents + index;
		switch (dir_obj->d_type) {
			case DT_DIR:
			temp_entry->type = FileEntryType_DIR;
			break;

			case DT_LNK:
			temp_entry->type = FileEntryType_LINK;
			break;

			case DT_REG:
			temp_entry->type = FileEntryType_FILE;
			break;

			default:
			temp_entry->type = FileEntryType_OTHER;
			break;
		}
		strcpy(temp_entry->name, dir_obj->d_name);
	    index++;
    }

	CLEANUP:
	if (is_err(err))
		free_file_list(list);
	return;
}

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
    Ch *temp_str = *str;
    temp_str[sz] = '\0';
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

Void write_fmt_to_file(Ch *path, Ch *fmt, Err *err, ...) {
    FILE        *fp;
    va_list     args;
    fp = fopen(path, "w");
    if (fp == NIL) {
        THROW(err, ErrCode_IO, "Could not open file %s", path)
        return;
    }
    va_start(args, err);
    vfprintf(fp, fmt, args);
    va_end(args);
    fclose(fp);
}
