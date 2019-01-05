#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint32_t read_int(FILE *fp, int *reached_end) {
    uint8_t data[4];
    if(fread(data, 1, 4, fp) != 4) {
        *reached_end = 1;
        return 0;
    }
    return ((uint32_t)data[0] << 24) | ((uint32_t)data[1] << 16) | ((uint32_t)data[2] << 8) | data[3];
}

void read_chunk_type(FILE *fp, char *data, int *reached_end) {
    if(fread(data, 1, 4, fp) != 4) {
        *reached_end = 1;
    }
}

char *chunk_type_name(char *chunk_type) {
    if(strcmp(chunk_type, "IHDR") == 0) {
        return "image header";
    } else if(strcmp(chunk_type, "PLTE") == 0) {
        return "palette";
    } else if(strcmp(chunk_type, "IDAT") == 0) {
        return "image data";
    } else if(strcmp(chunk_type, "IEND") == 0) {
        return "image trailer";
    } else if(strcmp(chunk_type, "cHRM") == 0) {
        return "primary chromaticities and white point";
    } else if(strcmp(chunk_type, "gAMA") == 0) {
        return "gamma";
    } else if(strcmp(chunk_type, "iCCP") == 0) {
        return "embedded ICC profile";
    } else if(strcmp(chunk_type, "sBIT") == 0) {
        return "significant bits";
    } else if(strcmp(chunk_type, "sRGB") == 0) {
        return "standard RGB color space";
    } else if(strcmp(chunk_type, "bKGD") == 0) {
        return "background color";
    } else if(strcmp(chunk_type, "hIST") == 0) {
        return "image histogram";
    } else if(strcmp(chunk_type, "tRNS") == 0) {
        return "transparency information";
    } else if(strcmp(chunk_type, "pHYs") == 0) {
        return "physical pixel dimensions";
    } else if(strcmp(chunk_type, "sPLT") == 0) {
        return "suggested palette";
    } else if(strcmp(chunk_type, "tIME") == 0) {
        return "last modification time";
    } else if(strcmp(chunk_type, "iTXt") == 0) {
        return "international textual data";
    } else if(strcmp(chunk_type, "tEXt") == 0) {
        return "textual data";
    } else if(strcmp(chunk_type, "zTXt") == 0) {
        return "compressed textual data";
    } else {
        return "unknown chunk type";
    }
}

int main(int argc, char **argv) {
    if(argc == 1) {
        printf("usage: %s <input file>\n", argv[0]);
        return 1;
    }

    char *input_path = argv[1];
    FILE *fp = fopen(input_path, "r");
    if(fp == NULL) {
        printf("\"%s\" does not exist\n", input_path);
        return 1;
    }

    fseek(fp, 0, SEEK_SET);

    uint8_t pnghdr[8];
    uint8_t pnghdr_expected[] = {137, 80, 78, 71, 13, 10, 26, 10};
    if(fread(pnghdr, 1, 8, fp) != 8) {
        fclose(fp);
        puts("not enough space for valid header");
        return 1;
    }

    if(memcmp(pnghdr, pnghdr_expected, 8) != 0) {
        fclose(fp);
        puts("invalid header");
        return 1;
    }

    int reached_eof = 0;
    int reached_iend = 0;
    while(!reached_eof) {
        uint32_t length = read_int(fp, &reached_eof);
        if(reached_eof && !reached_iend) {
            puts("truncated chunk");
            continue;
        }
        if(reached_iend) {
            break;
        }

        char chunk_type[5];
        read_chunk_type(fp, chunk_type, &reached_eof);
        chunk_type[4] = '\0';
        if(reached_eof) {
            puts("truncated chunk");
            continue;
        }

        printf("%s (%s), %d bytes\n", chunk_type, chunk_type_name(chunk_type), length);

        fseek(fp, length, SEEK_CUR);

        uint32_t crc = read_int(fp, &reached_eof);
        if(reached_eof) {
            puts("truncated chunk");
        }

        if(strcmp(chunk_type, "IEND") == 0) {
            reached_iend = 1;
        }
    }

    fclose(fp);

    return 0;
}
