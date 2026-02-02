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

void compress(char *file_name) {
    FILE *f;
    f = fopen(file_name, "rb");
    if (!f) {
        perror("Failed to open file");
        return;
        }
    char *new_name = change_ext(file_name);
    printf("%s \n", new_name);
    
    free(new_name);
    fclose(f);
    }

int main(int argc, char **argv) {
    compress(argv[1]);
    return 0;
    }

