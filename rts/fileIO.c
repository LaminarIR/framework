#include <stdarg.h>
#include <assert.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "fileIO.h"
/* 
 * RTS file handlers
 */

int getTypeSize(const char* type){
    int itemsize;
    if (strcmp(type, "int")){
        itemsize = sizeof(int);
    }else if(strcmp(type, "float")){
        itemsize = sizeof(float);
    }else{
        //RTS_error((char*)"wrong type given for file input, %s.\n", type);
    }
    return itemsize;
}

void RTS_filereader_open(const char* filename, const char* type){
    // Open file for input
    fin=fopen(filename, "rb");
    if (fin == NULL){
        //RTS_error((char*)"failed to open file %s.\n", filename);
    }
    fseek(fin, 0, SEEK_END);    // jump to the end of the file
    int filesize = ftell(fin);  // get the current byte offset in the file
    rewind(fin);                // reset the read offset to 0

    int itemsize = getTypeSize(type);

    if (filesize%itemsize != 0) {
        //RTS_error((char*)"filesize of %s does not fit in to the data type int.\n", filename);
    }
    numitems = filesize/itemsize;

    read_buf = malloc(filesize+1);
    int ret = fread(read_buf, itemsize, numitems, fin);
    if (ret == 0){
        //RTS_error((char*)"fread failed to read data from file %s.\n", filename);
    }
    read_offset = 0;
    return;
}

void RTS_filereader_close(const char* type) {
    fclose(fin);
}

int RTS_filereader_readInt(){
    int res = ((int *)read_buf)[read_offset];
    read_offset++;
    if (read_offset == numitems)
        read_offset=0;
    return res;
}

float RTS_filereader_readFloat(){
    float res = ((float *)read_buf)[read_offset];
    read_offset++;
    if (read_offset == numitems)
        read_offset=0;
    return res;
}

void RTS_filewriter_open(const char* filename, const char* type){
    // Open file for output
    fout=fopen(filename, "wb");
    if (fout == NULL){
        //RTS_error((char*)"failed to open file %s.\n", filename);
    }
    
    int itemsize = getTypeSize(type);
    write_buf = malloc(itemsize*FWRITE_BUF_ITEMS);
    write_buf_size = 0;
    return;
}

void RTS_filewriter_close(const char* type){
    // flush out remaining data in buffer
    fwrite(write_buf, sizeof(int), write_buf_size, fout);
    fclose(fout);
}

void RTS_filewriter_writeInt(int val){
    if (write_buf_size > FWRITE_BUF_ITEMS){
        fwrite(write_buf, sizeof(int), FWRITE_BUF_ITEMS, fout);
        write_buf_size = 0;
    }
    ((int *)write_buf)[write_buf_size] = val;
    write_buf_size++;
}

void RTS_filewriter_writeFloat(float val){
    if (write_buf_size > FWRITE_BUF_ITEMS){
        fwrite(write_buf, sizeof(float), FWRITE_BUF_ITEMS, fout);
        write_buf_size = 0;
    }
    ((float *)write_buf)[write_buf_size] = val;
    write_buf_size++;
}

