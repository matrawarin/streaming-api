#include <stdio.h>
#include <string.h>
#include <time.h>
#include "block.h"
#include "crypto.h"   // double_sha256
#include "db.h"       // write_chain()

int main() {
    char design[32];
    struct block_header genesis;
    printf("initialized .              normal\n");
    memset(&genesis, 0, sizeof(genesis));
    printf("initialized .              start\n");
    // Version
    genesis.version = 1;

    // prev_hash = 0 (genesis ไม่มี block ก่อนหน้า)
    memset(genesis.prev_hash, 0, 32);
    printf("initialized .              final\n");
    // merkle_root สมมุติ (ถ้าคุณมี tx จริง ให้คำนวณเอง)
    const char *data = "genesis block";
    double_sha256(genesis.merkle_root, (uint8_t*)data, strlen(data));
    
    //memcpy(design, genesis.merkle_root, 32);
    // timestamp
    genesis.timestamp = time(NULL);

    // difficulty target สมมุติ เช่น 0x1d00ffff (แบบ Bitcoin)
    genesis.bits = 0x1d00ffff;

    // nonce (ตั้งเป็น 0 ก่อน)
    genesis.nonce = 0;

    printf("initialized . \n");
    // -------------------------
    // เขียนลง chain.dat
    // -------------------------
    FILE *fp = fopen("chain.dat", "wb");
    if (!fp) {
        perror("fopen chain.dat");
        return 1;
    }

    fwrite(&genesis, sizeof(genesis), 1, fp);
    fclose(fp);

    printf("Genesis block created and saved to chain.dat\n");
    return 0;
}

