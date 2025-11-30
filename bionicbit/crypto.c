
#include "crypto.h"
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/pem.h>
#include <string.h>
#include <stdlib.h>

void double_sha256(const uint8_t *data, size_t len, uint8_t out[32]){
    uint8_t buf[32];
    SHA256(data, len, buf);
    SHA256(buf, 32, out);
}

void hash160(const uint8_t *data, size_t len, uint8_t out20[20]){
    uint8_t sha[32];
    SHA256(data, len, sha);
    RIPEMD160(sha, 32, out20);
}

int ecdsa_generate_key(unsigned char **priv, size_t *priv_len, unsigned char **pub, size_t *pub_len){
    int ret = 0;
    EC_KEY *eck = EC_KEY_new_by_curve_name(NID_secp256k1);
    if(!eck) return 0;
    if(!EC_KEY_generate_key(eck)) goto end;

    // private key to DER
    *priv_len = i2d_ECPrivateKey(eck, NULL);
    *priv = malloc(*priv_len);
    unsigned char *p = *priv;
    i2d_ECPrivateKey(eck, &p);

    // public key (compressed)
    const EC_POINT *pt = EC_KEY_get0_public_key(eck);
    const EC_GROUP *gr = EC_KEY_get0_group(eck);
    *pub_len = EC_POINT_point2oct(gr, pt, POINT_CONVERSION_COMPRESSED, NULL, 0, NULL);
    *pub = malloc(*pub_len);
    EC_POINT_point2oct(gr, pt, POINT_CONVERSION_COMPRESSED, *pub, *pub_len, NULL);

    ret = 1;
end:
    EC_KEY_free(eck);
    return ret;
}
