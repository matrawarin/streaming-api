#ifndef KYC_H
#define KYC_H

#include <stddef.h>

int kyc_init();
int kyc_add(const char *acc_number, const char *first_name, const char *last_name,
            const char *dob, const char *id_number, const char *address,
            const char *phone, const char *email);
int kyc_add_face(const char *acc_number, const unsigned char *face_data, size_t face_size);
void kyc_show(const char *acc_number);

#endif

