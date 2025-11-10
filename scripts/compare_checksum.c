#include <stdlib.h>
#include <stdio.h>

#define READ_BYTE_ONLY              "rb"
#define WRITE_ONLY                  "w"

const char * WORKING_BIN = "../bin/fw_funkar.bin";
const char * NOT_WORKING_BIN = "../bin/axels_funkar_inte.bin";
const char * DIFF_TEXT_FILE = "../bin/diff_text.txt";
int counter = 0;
int offset = 0;


int main (void)
{
    unsigned char working_buffer;
    unsigned char not_working_buffer;

    FILE * working = fopen(WORKING_BIN, READ_BYTE_ONLY);
    FILE * not_working = fopen(NOT_WORKING_BIN, READ_BYTE_ONLY);

    if(!working && !not_working)
    {
        return 1;
    }

    size_t not_working_byte = fread(&not_working_buffer, 1, 1, not_working);
    size_t working_byte = fread(&working_buffer, 1, 1, working);

    while(working_byte != 0 && not_working_byte != 0)
    {
        
        if(working_buffer != not_working_buffer)
        {
            // save to textfile
            printf("%d : Offset --> %d\n", counter, offset);
            counter++;
        }
        offset++;
        not_working_byte = fread(&not_working_buffer, 1, 1, not_working);
        working_byte = fread(&working_buffer, 1, 1, working);
    }

    fclose(not_working);
    fclose(working);

    return 0;
}