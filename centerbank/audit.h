#ifndef AUDIT_H
#define AUDIT_H

#include <time.h>

/* -----------------------------------------
   STRUCT : ข้อมูลบันทึก Audit Log
   ----------------------------------------- */
typedef struct {
    int id;                        // เลขลำดับ audit log
    char action[128];              // ประเภทเหตุการณ์ เช่น CREATE_ACCOUNT, TRANSFER
    char user_id[64];              // ผู้กระทำ (ระบบ/พนักงาน)
    char details[256];             // รายละเอียดเหตุการณ์ เช่น account_id, amount ฯลฯ
    time_t timestamp;              // เวลา
    unsigned char hash[32];        // SHA-256 hash
    unsigned char prev_hash[32];   // hash ก่อนหน้า (สร้าง chain)
    unsigned char signature[256];  // Digital signature (RSA)
} AuditLog;

/* -----------------------------------------
   ฟังก์ชั่นบันทึกเหตุการณ์
   ----------------------------------------- */
int audit_init();  // เตรียม DB audit
int audit_log_event(const char *action,
                    const char *user_id,
                    const char *details);

/* -----------------------------------------
   ฟังก์ชั่นอ่าน Audit Log
   ----------------------------------------- */
int audit_get_by_id(int id, AuditLog *out);
int audit_list_all();  // print ออกหน้าจอ

/* -----------------------------------------
   ฟังก์ชั่นตรวจสอบความถูกต้อง (Integrity Check)
   ----------------------------------------- */
int audit_verify_chain();   // ตรวจ hash chain
int audit_verify_signature(int id); // ตรวจลายเซ็นดิจิทัล

#endif

