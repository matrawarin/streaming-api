#ifndef CRYPTO_H
#define CRYPTO_H

#include <stddef.h>

// SHA256 hash function
void crypto_sha256(const unsigned char *data, size_t data_len,
                   unsigned char output[32]);

// RSA Sign (private key)
// signature is allocated inside and must be freed by caller
int crypto_sign(const unsigned char *data, size_t data_len,
                unsigned char **signature, size_t *sig_len,
                const char *private_key_path);

// RSA Verify (public key)
// return 1 = valid, 0 = invalid
int crypto_verify(const unsigned char *data, size_t data_len,
                  const unsigned char *signature, size_t sig_len,
                  const char *public_key_path);

// Load PEM private key file
void *crypto_load_private_key(const char *path);

// Load PEM public key file
void *crypto_load_public_key(const char *path);

#endif

