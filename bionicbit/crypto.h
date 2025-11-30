#ifndef CRYPTO_H
#define CRYPTO_H


#include <openssl/sha.h>
#include <stdint.h>
#include <string.h>

void double_sha256(const uint8_t *data, size_t len, uint8_t out[32]) ;
void hash160(const uint8_t *data, size_t len, uint8_t out20[20]);
int ecdsa_generate_key(unsigned char **priv, size_t *priv_len, unsigned char **pub, size_t *pub_len);


#endif
