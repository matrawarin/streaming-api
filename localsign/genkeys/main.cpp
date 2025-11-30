#include <stdio.h>
#include <stdlib.h>

#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509v3.h>
#ifndef OPENSSL_NO_ENGINE
#include <openssl/engine.h>
#endif

#include <openssl/x509.h>

#define MS_CALLBACK __cdecl
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
static void callback(int p, int n, void *arg);	
static int MS_CALLBACK dh_cb(int p, int n, BN_GENCB *cb);

int main(int argc, char *argv[]) {
	
	RSA      *rsa=NULL;
	DH       *dh = NULL;
	DSA      *dsa= NULL;
	EVP_PKEY *pk=NULL;
	EC_KEY   *eckey=NULL;
	EC_GROUP *group = NULL;
	BIO      *bio_err;
	BN_GENCB cb;
	int      nid;
	
	BIO      *keys = BIO_new_file("certs\\localsign.key", "w");
	
	CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);

	bio_err=BIO_new_fp(stderr, BIO_NOCLOSE);
	
	if ((pk=EVP_PKEY_new()) == NULL){
		return 0;
	}
	rsa=RSA_generate_key(2048,RSA_F4,callback,NULL);
	if (!EVP_PKEY_assign_RSA(pk,rsa))
		{
		abort();
		return 0;
		}
	rsa=NULL;
	
	RSA_print_fp(stdout,pk->pkey.rsa,0);
	PEM_write_bio_RSAPrivateKey(bio_err, pk->pkey.rsa, NULL, NULL, 0, NULL, NULL);
	
	BN_GENCB_set(&cb, dh_cb, bio_err);
	if(((dh = DH_new()) == NULL) || !DH_generate_parameters_ex(dh, 512, 5, &cb))
	   return 0;
	
	DH_generate_key(dh);

	/*if (!EVP_PKEY_assign_DH(pk,dh))
	{
		abort();
		return 0;
	}
	*/	
	DHparams_print(bio_err, dh);	
	PEM_write_bio_DHparams(bio_err, dh);	
    
    int ret=-1;
    unsigned long ref = 0;
	////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////	
	////////////////////////////////////////////////////////////////////////////////////////
	if(((dsa = DSA_new())==NULL )|| 
	    !DSA_generate_parameters_ex(dsa, 2048, (unsigned char *)"xx", 2, &ret, &ref, &cb ) ){
	   printf("ERROR: xxx..... \n");    	
	}
	DSA_generate_key(dsa);
	/*
	if (!EVP_PKEY_assign_DSA(pk,dsa))
	{
		abort();
		return 0;
	}
	*/	
	
	DSAparams_print(bio_err, dsa);
	DSA_print(bio_err, dsa, -1);
	PEM_write_bio_DSAPrivateKey(bio_err, dsa, NULL, NULL, 0, NULL ,NULL);
	
	/////////////////////////////////////////////////////////////////////////////////////////
	
	printf("----------------------------------------------------------------------\n");
	if(((eckey=EC_KEY_new())==NULL))
	  printf("ERROR: ccc............\n");
	nid = NID_X9_62_prime192v1;
	group = EC_GROUP_new_by_curve_name(nid);
	if (group == NULL)
	{
		printf("[ERR] new group name 'EC_KEY' \n");
	}	
	EC_GROUP_set_asn1_flag(group, 0); // OPENSSL_EC_NAMED_CURVE
	EC_GROUP_set_point_conversion_form(group, POINT_CONVERSION_COMPRESSED); 
		
	if (EC_KEY_set_group(eckey, group) == 0)
	{
		printf("[ERR] set group key.. 'EC_KEY'\n");
	}  
	if(!EC_KEY_generate_key(eckey)){
		printf("[ERR] generate key.. 'EC KEY'\n");
	}
	/*
	if (!EVP_PKEY_assign_EC_KEY(pk,eckey))
	{
		abort();
		return 0;
	}
	*/
	ECParameters_print(bio_err , eckey);
	EC_KEY_print(bio_err , eckey, 0);
	PEM_write_bio_ECPrivateKey(bio_err, eckey, NULL,
				NULL, 0, NULL, NULL);
	
	printf("----------------------------------------------------------------------\n");
	PEM_write_bio_PrivateKey(bio_err, pk, NULL,NULL,0, NULL,NULL);
	PEM_write_bio_PrivateKey(keys, pk , NULL, NULL, 0, NULL, NULL);
	
	BIO_free(keys);
	
	#ifndef OPENSSL_NO_ENGINE
	ENGINE_cleanup();
#endif
	CRYPTO_cleanup_all_ex_data();
    
	CRYPTO_mem_leaks(bio_err);
	BIO_free(bio_err);
	system("pause");
	return 0;
}

static void callback(int p, int n, void *arg)
	{
	char c='B';

	if (p == 0) c='.';
	if (p == 1) c='+';
	if (p == 2) c='*';
	if (p == 3) c='\n';
	fputc(c,stderr);
	}
	
static int MS_CALLBACK dh_cb(int p, int n, BN_GENCB *cb)
	{
	char c='*';

	if (p == 0) c='.';
	if (p == 1) c='+';
	if (p == 2) c='*';
	if (p == 3) c='\n';
	BIO_write((BIO *)cb->arg,&c,1);
	(void)BIO_flush((BIO *)cb->arg);
#ifdef LINT
	p=n;
#endif
	return 1;
	}

