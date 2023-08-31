#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "unis_encrypt.h"

#define MAX_PATH_LENGTH 256
#define MAX_TEXT_LENGTH 1000

void encrypt_file(const char* filename)
{
    FILE* pyc_file = fopen(filename, "rb");
    if (pyc_file == NULL) {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    // Get file size
    fseek(pyc_file, 0, SEEK_END);
    long file_size = ftell(pyc_file);
    fseek(pyc_file, 0, SEEK_SET);

    // Read file content
    char* buffer = (char*)malloc(file_size);
    fread(buffer, 1, file_size, pyc_file);
    fclose(pyc_file);

    // Encrypt the file content (excluding the first 16 bytes)
    AES_ENCODE_DATA test = {0};
    test.textBuffer = buffer + 16;  // Skip the first 16 bytes
    test.bufferSize = file_size - 16;  // Adjust the buffer size

    unis_aes_encrypt(&test);


    // Write the encrypted content to .pye file
    char pye_filename[MAX_PATH_LENGTH];
    strcpy(pye_filename, filename);
    strcpy(strrchr(pye_filename, '.'), ".pye"); // 将最后一个点之后的部分替换为.pye
    FILE* pye_file = fopen(pye_filename, "wb");
    if (pye_file == NULL) {
        printf("Failed to create .pye file: %s\n", pye_filename);
        free(buffer);
        return;
    }

    // Write header to the file
    const char pe_header[] = {0x2e, 0x70, 0x65, 0x00};
    fwrite(pe_header, 1, sizeof(pe_header), pye_file);

    // Write the skipped 16 bytes to the file
    fwrite(buffer, 1, 16, pye_file);

    // Write the encrypted content to the file
    fwrite(test.enBuffer, 1, test.bufferSize, pye_file);
    fclose(pye_file);

    free(buffer);
}

void scan_directory(const char* dirname)
{
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        printf("Failed to open directory: %s\n", dirname);
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            const char* filename = entry->d_name;
            size_t filename_length = strlen(filename);
            if (filename_length > 4 && strcmp(filename + filename_length - 4, ".pyc") == 0) {
                char full_path[MAX_PATH_LENGTH];
                strcpy(full_path, dirname);
                strcat(full_path, "/");
                strcat(full_path, filename);
                encrypt_file(full_path);
            }
        } else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            // Recursively scan subdirectories
            char subdirectory[MAX_PATH_LENGTH];
            strcpy(subdirectory, dirname);
            strcat(subdirectory, "/");
            strcat(subdirectory, entry->d_name);
            scan_directory(subdirectory);
        }
    }

    closedir(dir);
}


int main(int argc, char* argv[]) {
    const char* directory = "./"; // 默认为当前目录

    // 检查命令行参数是否提供了目录路径
    if (argc > 1) {
        directory = argv[1];
    }

    scan_directory(directory);

    return 0;
}