#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *change_ext(char *buffer) {
    char *ext_start = strrchr(buffer, '.');
    
    int index = (int)(ext_start - buffer);
    
    char *new_name = NULL;
    new_name = malloc(index + 4);
    if (!new_name) {
        perror("Memory allocation failed");        
        return NULL;
        }
        
    int arr_count = (index + 4)/sizeof(new_name[0]);
    for (int i = 0; i < arr_count; i++) {
        new_name[i] = '\0';
        }
    int i = 0;
    const char *new_ext = ".comp";
    while(i < (index + strlen(new_ext))) {
        if (i < index) new_name[i] = buffer[i];
        else new_name[i] = new_ext[i - index];
        i++;
        }    
    return new_name;
    }

int main(int argc, char **argv) {
    FILE *f;
    f = fopen(argv[1], "rb");
    if (!f) {
        perror("Failed to open file");
        return -1;
        }
        
    uint32_t len = strlen(argv[1]);

    char *buffer = NULL;    
    buffer = malloc(len);
    
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(f);
        return -1;
        }    
    
    strcpy(buffer, argv[1]);
    char *file_name = change_ext(buffer);
    printf("%s \n", file_name);
    
    free(buffer);
    free(file_name);
    fclose(f);
    return 0;
    }

