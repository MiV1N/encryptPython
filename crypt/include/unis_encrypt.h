
#include  <stdio.h> 
#include <stdint.h>

typedef struct {
    uint8_t * textBuffer;
    uint8_t * enBuffer;
    size_t  bufferSize;
} AES_ENCODE_DATA;


int unis_aes_encrypt(AES_ENCODE_DATA *plainTextData);
int unis_aes_decrypt(AES_ENCODE_DATA *plainTextData);