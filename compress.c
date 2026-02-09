#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SYSTEM_ERROR -1
#define USER_ERROR 0
#define SUCCESS 1

char *change_ext(char *file_name, char *new_ext) { /* caller must free after use */
    uint32_t ext_len = strlen(new_ext);
    int stripped_name_len;
    
    char *ext_start = strrchr(file_name, '.');
    if (!ext_start) stripped_name_len = (int)strlen(file_name);
    else stripped_name_len = (int)(ext_start - file_name);
    
    char *new_name = NULL;
    int size = stripped_name_len + ext_len + 1;
    new_name = malloc(size);
    if (!new_name) {
        perror("Memory allocation failed");
        return NULL;
        }
        
    int i = 0;
    
    while (i < size) {
        if (i < stripped_name_len) new_name[i] = file_name[i];
        else new_name[i] = new_ext[i - stripped_name_len];
        i++;
        }
    new_name[size - 1] = '\0';
    return new_name;
    }

int compress(char *file_name) {
    int status;
    FILE *original, *compressed;
         
    original = fopen(file_name, "rb");    
    if (!original) {
        perror("Failed to open file");
        status = SYSTEM_ERROR;
        goto cleanup;
        }
        
    char *new_name = change_ext(file_name, ".rle");
    if (new_name == NULL) {
        printf("Program Error. \n");
        status = SYSTEM_ERROR;
        goto cleanup;
        }
    
    compressed = fopen(new_name, "wb");
    if (!compressed) {
        perror("Failed to open file");
        status = SYSTEM_ERROR;
        goto cleanup;
        }
    
    uint32_t len = strlen(file_name);
    fwrite(&len, sizeof(uint32_t), 1, compressed);
    fwrite(file_name, sizeof(char), len, compressed);
    
    int count, prev_char, curr_char;
    prev_char = fgetc(original);
    
    count = 1;    
    
    while (1) {
        curr_char = fgetc(original);
        if (curr_char == EOF) {
            fwrite(&count, sizeof(int), 1, compressed);
            fwrite(&prev_char, sizeof(char), 1, compressed);
            break;
            }
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
    
    status = SUCCESS;
    cleanup:
        if (new_name) free(new_name);
        if (original) fclose(original);
        if (compressed) fclose(compressed);
    
    return status;
    }

int view_encoding(char *file_name) {
    int status;
    FILE *f = fopen(file_name, "rb");
    if (!f) {
        perror("Failed to open file");
        status = SYSTEM_ERROR;
        goto cleanup;
        }
    const char *ext = ".rle";
    char *ext_check = strrchr(file_name, '.');
    if (strcmp(ext, ext_check) != 0) {
        printf("Invalid file format! Can only extract .rle files. \n");
        status = USER_ERROR;
        goto cleanup;
        }

    int count;
    char characters;

    fread(&count, sizeof(uint32_t), 1, f);
    char *original_name = malloc(count + 1);
    if (!original_name) {
        perror("Memory allocation failed");
        status = SYSTEM_ERROR;
        goto cleanup;
        }
    fread(original_name, sizeof(char), count, f);
    original_name[count] = '\0';
    printf("File name: %s\n", original_name);
    
    while (fread(&count, sizeof(int), 1, f)) {
        fread(&characters, sizeof(char), 1, f);
        printf("%d%c", count, characters);
        }    
    
    status = SUCCESS;
    
    cleanup:
        if (f) fclose(f);
        if (original_name) free(original_name); 
    
    return status;
    }

int extract(char *file_name) {
    int status;
    const char *ext = ".rle";
    char *ext_check = strrchr(file_name, '.');
    if (strcmp(ext, ext_check) != 0) {
        printf("Invalid file format! Can only view .rle files. \n");
        return USER_ERROR;
        }
    FILE *compressed, *extracted;
    compressed = fopen(file_name, "rb");
    if (!compressed) {
        perror("Failed to open file");
        status = SYSTEM_ERROR;
        goto cleanup;
        }
    
    uint32_t name_len;
    fread(&name_len, sizeof(uint32_t), 1, compressed);
    char *original_file = malloc(name_len + 1);
    if (!original_file) {
        perror("Memory allocation failed. \n");
        status = SYSTEM_ERROR;
        goto cleanup;
        }
    fread(original_file, sizeof(char), name_len, compressed);
    original_file[name_len] = '\0';
    extracted = fopen(original_file, "wb");
    
    int count;
    char c;
    while(fread(&count, sizeof(int), 1, compressed)) {
        fread(&c, sizeof(char), 1, compressed);
        while(count > 0) {
            fwrite(&c, sizeof(char), 1, extracted);
            count--;
            }
        }
    
    status = SUCCESS;
    
    cleanup:
        if (compressed) fclose(compressed);
        if (extracted) fclose(extracted);
        if (original_file) free(original_file);
    
    return status;
    }

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Program usage: %s <command> <file_name> \n", argv[0]);
        return USER_ERROR;
        }
    
    int compress_file, view_file, extract_file;
    if (strcmp(argv[1], "compress") == 0) {
        compress_file = compress(argv[2]);
        if (compress_file == SUCCESS) printf("Success! \n");
        else printf("Failure! \n");
        }

    else if (strcmp(argv[1], "view") == 0) {
        view_file = view_encoding(argv[2]);
        if (view_file != SUCCESS) printf("Failure! \n");
        }
    
    else if (strcmp(argv[1], "extract") == 0) {
        printf("Extracting... \n");
        extract_file = extract(argv[2]);
        if (extract_file == SUCCESS) printf("Success! \n");
        else if (extract_file == SYSTEM_ERROR) printf("Failure! \n");
        }
    
    return 0;
    }
