#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t BYTE;

int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    char *name = malloc(8); // 8 bytes per jpg string
    BYTE *buf = malloc(BLOCK_SIZE); // 512 bytes per block
    int jpgnum = 0;

    FILE *jpg; // declare outside otherwise can't close last one

    while (fread(buf, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        if (buf[0] == 0xff && buf[1] == 0xd8 && buf[2] == 0xff && ((buf[3] & 0xf0) == 0xe0))
        {
            if (jpgnum != 0)
            {
                fclose(jpg); // don't close jpg 0, we just started!
            }
            sprintf(name, "%03i.jpg", jpgnum);
            jpg = fopen(name, "w");
            jpgnum++; // increments after we find and open jpg 0
        }
        if (jpgnum != 0) // don't write until we find start of jpg
        {
            fwrite(buf, BLOCK_SIZE, 1, jpg);
        }
    }
    fclose(jpg);
    fclose(raw_file);
    free(buf);
    free(name);
    return 0;
}