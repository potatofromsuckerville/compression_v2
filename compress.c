#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SYSTEM_ERROR -1
#define ERROR 0
#define SUCCESS 1

char *change_ext(char *buffer, char *new_ext) {
    uint32_t ext_len = strlen(new_ext);
    
    char *ext_start = strrchr(buffer, '.');
    
    int index = (int)(ext_start - buffer);
    
    char *new_name = NULL;
    int size = index + ext_len;
    new_name = malloc(size);
    if (!new_name) {
        perror("Memory allocation failed");        
        return NULL;
        }
        
    int arr_count = (size)/sizeof(new_name[0]);
    for (int i = 0; i < arr_count; i++) {
        new_name[i] = '\0';
        }
    int i = 0;
    
    while (i < (index + ext_len)) {
        if (i < index) new_name[i] = buffer[i];
        else new_name[i] = new_ext[i - index];
        i++;
        }    
    return new_name;
    }

int compress(char *file_name) {
    FILE *original, *compressed;
    
    original = fopen(file_name, "rb");    
    if (!original) {
        perror("Failed to open file");
        return SYSTEM_ERROR;
        }
    char *new_name = change_ext(file_name, ".rle");
    if (new_name == NULL) {
        printf("Program Error. \n");
        return SYSTEM_ERROR;
        }
    
    compressed = fopen(new_name, "wb");
    if (!compressed) {
        perror("Failed to open file");
        return SYSTEM_ERROR;
        }
    
    char prev_char, curr_char;
    prev_char = fgetc(original);
    int count;
    count = 1;
    
    
    while (1) {
        curr_char = fgetc(original);
        if (curr_char == EOF) break;
        if ((curr_char == prev_char)) {
            prev_char = curr_char;
            count++;
            }
        else {
            fwrite(&count, sizeof(int), 1, compressed);
            fwrite(&prev_char, sizeof(char), 1, compressed);
            prev_char = curr_char;
            count = 1;
            }
        }
    
    free(new_name);
    fclose(original);
    return SUCCESS;
    }

int view(char *file_name) {
    FILE *f = fopen(file_name, "rb");
    if (!f) {
        perror("Failed to open file");
        return SYSTEM_ERROR;
        }
    int count;
    char characters;
    
    while (fread(&count, sizeof(int), 1, f)) {
        fread(&characters, sizeof(char), 1, f);
        printf("%d%c", count, characters);
        }
    printf("\n");
    return SUCCESS;
    }

int main(int argc, char **argv) {
    int compress_file, view_file;
    if (strcmp(argv[1], "compress") == 0) {
        compress_file = compress(argv[2]);
        if (compress_file) printf("Success! \n");
        else printf("Failure! \n");
        }

    else if (strcmp(argv[1], "view") == 0) {
        view_file = view(argv[2]);
        if (view_file) printf("Success! \n");
        else printf("Failure! \n");        
        }
    
    return 0;
    }
