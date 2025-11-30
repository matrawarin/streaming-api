#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CHAIN_FILE "chain.dat"

int append_block_header(const void *hdr, size_t len){
    FILE *f = fopen(CHAIN_FILE, "ab"); if(!f) return 0;
    fwrite(hdr, 1, len, f); fclose(f); return 1;
}

size_t load_chain(void **out){
    FILE *f = fopen(CHAIN_FILE, "rb"); if(!f) { *out = NULL; return 0; }
    fseek(f, 0, SEEK_END); long sz = ftell(f); fseek(f,0,SEEK_SET);
    void *buf = malloc(sz);
    fread(buf,1,sz,f); fclose(f);
    *out = buf; return (size_t)sz;
}
