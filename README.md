# Build:
gcc -o compress.c compress

# Program usage:
./compress <command> <file_name>

# Available commands:
- compress
- extract
- view

# Example usage using dummy files "file.txt" and "no_ext"
- file.txt
1. Compress file: ./compress compress file.txt
2. View encoding of compressed file (only works for files compressed using the algorithm): ./compress view file.rle
3. Extract original: ./compress extract file.rle
 
 - no_ext
1. Compress file: ./compress compress no_ext
2. View encoding: ./compress view no_ext.rle
3. Extract original: ./compress extract no_ext.rle
 
