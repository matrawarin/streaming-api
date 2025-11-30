#ifndef KYC_H
#define KYC_H

typedef struct {
    int account_id;
    char full_name[100];
    char id_number[20];      // เลขบัตรประชาชน
    char dob[15];            // วันเกิด (YYYY-MM-DD)
    char address[200];
    int verified;            // 0 = not verified, 1 = verified
} KYCInfo;

void kyc_register(int account_id);
void kyc_verify(int account_id);
void kyc_show(int account_id);

#endif

