#include "crypto.h"
#include <openssl/sha.h>
#include <stdio.h>

void sha256(const char *str, char output[65]) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)str, strlen(str), hash);

    for (int i=0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(output + (i*2), "%02x", hash[i]);

    output[64] = 0;
}

