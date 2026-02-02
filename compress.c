#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SYSTEM_ERROR 0
#define SUCCESS 1

char *change_ext(char *buffer) {
    const char *new_ext = ".comp";
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
    char *new_name = change_ext(file_name);
    
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
        if ((curr_char == prev_char)) {
            prev_char = curr_char;
            count++;
            continue;
            }        
        
        count = 1;
        break;
        }
    free(new_name);
    fclose(original);
    return SUCCESS;
    }

int main(int argc, char **argv) {
    int compress_file = compress(argv[1]);
    if (compress_file) printf("Success! \n");
    return 0;
    }

