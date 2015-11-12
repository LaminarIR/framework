#ifndef __RTS_FILEIO_H__
#define __RTS_FILEIO_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

//void RTS_error(char *txt, ...);

/*
 * RTS file handlers
 */
FILE* fin;
void* read_buf;
int read_offset;
int numitems;
void RTS_filereader_open(const char* filename, const char* type);
void RTS_filereader_close(const char* type);
int RTS_filereader_readInt();
float RTS_filereader_readFloat();

#define FWRITE_BUF_ITEMS (1024*1024*16)
FILE* fout;
void* write_buf;
int write_buf_size;
void RTS_filewriter_open(const char* filename, const char* type);
void RTS_filewriter_close(const char* type);
void RTS_filewriter_writeInt(int val);
void RTS_filewriter_writeFloat(float val);
#endif
