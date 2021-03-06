#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Enable ECB, CTR and CBC mode. Note this can be done before including aes.h or at compile-time.
// E.g. with GCC by using the -D flag: gcc -c aes.c -DCBC=0 -DCTR=1 -DECB=1
#define CBC 1
#define CTR 1
#define ECB 1

#include "aes.h"


static void phex(uint8_t* str);
static void test_encrypt_ecb_verbose(void);
void read(uint8_t key[], uint8_t plain_text[]);
void read2(uint8_t key[], uint8_t plain_text[]);
static void test_decrypt_ecb_verbose(void);

int main(void)
{

#ifdef AES128
	printf("\nTesting AES128\n\n");
#elif defined(AES192)
	printf("\nTesting AES192\n\n");
#elif defined(AES256)
	printf("\nTesting AES256\n\n");
#else
	printf("You need to specify a symbol between AES128, AES192 or AES256. Exiting");
	return 0;
#endif
	
	srand(time(NULL));
	test_encrypt_ecb_verbose();
	//test_decrypt_ecb_verbose();
	return 0;
}


// prints string as hex
static void phex(uint8_t* str)
{

#ifdef AES128
	uint8_t len = 16;
#elif defined(AES192)
	uint8_t len = 24;
#elif defined(AES256)
	uint8_t len = 32;
#endif

	unsigned char i;
	for (i = 0; i < len; ++i)
		printf("%.2x ", str[i]);
	printf("\n");
}

void read(uint8_t key[], uint8_t plain_text[])
{
	freopen("encrypt.inp", "r", stdin);
	freopen("encrypt.txt", "w", stdout);
	//generate key//
	for (int i = 0; i < 16; i++)
	{
		key[i] = rand() % 16;
	}

	char pt[64];
	gets(pt);
	

	for (int i = 0; i < 64; i++)
	{
		if (pt[i] == '\0') break;
		plain_text[i] = (uint8_t)pt[i];
	}
}


static void test_encrypt_ecb_verbose(void)
{
	// Example of more verbose verification

	uint8_t i, buf[64], buf2[64];

	// 128bit key
	uint8_t key[16] = { (uint8_t)0x2b, (uint8_t)0x7e, (uint8_t)0x15, (uint8_t)0x16, (uint8_t)0x28, (uint8_t)0xae, (uint8_t)0xd2, (uint8_t)0xa6, (uint8_t)0xab, (uint8_t)0xf7, (uint8_t)0x15, (uint8_t)0x88, (uint8_t)0x09, (uint8_t)0xcf, (uint8_t)0x4f, (uint8_t)0x3c };
	// 512bit text
	uint8_t plain_text[64] = { (uint8_t)0x6b, (uint8_t)0xc1, (uint8_t)0xbe, (uint8_t)0xe2, (uint8_t)0x2e, (uint8_t)0x40, (uint8_t)0x9f, (uint8_t)0x96, (uint8_t)0xe9, (uint8_t)0x3d, (uint8_t)0x7e, (uint8_t)0x11, (uint8_t)0x73, (uint8_t)0x93, (uint8_t)0x17, (uint8_t)0x2a,
		(uint8_t)0xae, (uint8_t)0x2d, (uint8_t)0x8a, (uint8_t)0x57, (uint8_t)0x1e, (uint8_t)0x03, (uint8_t)0xac, (uint8_t)0x9c, (uint8_t)0x9e, (uint8_t)0xb7, (uint8_t)0x6f, (uint8_t)0xac, (uint8_t)0x45, (uint8_t)0xaf, (uint8_t)0x8e, (uint8_t)0x51,
		(uint8_t)0x30, (uint8_t)0xc8, (uint8_t)0x1c, (uint8_t)0x46, (uint8_t)0xa3, (uint8_t)0x5c, (uint8_t)0xe4, (uint8_t)0x11, (uint8_t)0xe5, (uint8_t)0xfb, (uint8_t)0xc1, (uint8_t)0x19, (uint8_t)0x1a, (uint8_t)0x0a, (uint8_t)0x52, (uint8_t)0xef,
		(uint8_t)0xf6, (uint8_t)0x9f, (uint8_t)0x24, (uint8_t)0x45, (uint8_t)0xdf, (uint8_t)0x4f, (uint8_t)0x9b, (uint8_t)0x17, (uint8_t)0xad, (uint8_t)0x2b, (uint8_t)0x41, (uint8_t)0x7b, (uint8_t)0xe6, (uint8_t)0x6c, (uint8_t)0x37, (uint8_t)0x10 };
	memset(plain_text, 0, 64);
	memset(key, 0, 16);
	read(key, plain_text);
	memset(buf, 0, 64);
	memset(buf2, 0, 64);

	int len = 16;
	//phex(key);
	FILE* f = fopen("key.txt", "w");
	for (i = 0; i < len; ++i)
		fprintf(f, "%.2x", key[i]);
	fclose(f);


	struct AES_ctx ctx;
	AES_init_ctx(&ctx, key);

	for (i = 0; i < 4; ++i)
	{
		AES_ECB_encrypt(&ctx, plain_text + (i * 16));
		phex(plain_text + (i * 16));
	}

}

static void test_decrypt_ecb_verbose(void){
	// Example of more verbose verification

	uint8_t i, buf[64], buf2[64];

	// 128bit key
	uint8_t key[16] = { (uint8_t)0x2b, (uint8_t)0x7e, (uint8_t)0x15, (uint8_t)0x16, (uint8_t)0x28, (uint8_t)0xae, (uint8_t)0xd2, (uint8_t)0xa6, (uint8_t)0xab, (uint8_t)0xf7, (uint8_t)0x15, (uint8_t)0x88, (uint8_t)0x09, (uint8_t)0xcf, (uint8_t)0x4f, (uint8_t)0x3c };
	// 512bit text
	uint8_t plain_text[64] = { (uint8_t)0x6b, (uint8_t)0xc1, (uint8_t)0xbe, (uint8_t)0xe2, (uint8_t)0x2e, (uint8_t)0x40, (uint8_t)0x9f, (uint8_t)0x96, (uint8_t)0xe9, (uint8_t)0x3d, (uint8_t)0x7e, (uint8_t)0x11, (uint8_t)0x73, (uint8_t)0x93, (uint8_t)0x17, (uint8_t)0x2a,
		(uint8_t)0xae, (uint8_t)0x2d, (uint8_t)0x8a, (uint8_t)0x57, (uint8_t)0x1e, (uint8_t)0x03, (uint8_t)0xac, (uint8_t)0x9c, (uint8_t)0x9e, (uint8_t)0xb7, (uint8_t)0x6f, (uint8_t)0xac, (uint8_t)0x45, (uint8_t)0xaf, (uint8_t)0x8e, (uint8_t)0x51,
		(uint8_t)0x30, (uint8_t)0xc8, (uint8_t)0x1c, (uint8_t)0x46, (uint8_t)0xa3, (uint8_t)0x5c, (uint8_t)0xe4, (uint8_t)0x11, (uint8_t)0xe5, (uint8_t)0xfb, (uint8_t)0xc1, (uint8_t)0x19, (uint8_t)0x1a, (uint8_t)0x0a, (uint8_t)0x52, (uint8_t)0xef,
		(uint8_t)0xf6, (uint8_t)0x9f, (uint8_t)0x24, (uint8_t)0x45, (uint8_t)0xdf, (uint8_t)0x4f, (uint8_t)0x9b, (uint8_t)0x17, (uint8_t)0xad, (uint8_t)0x2b, (uint8_t)0x41, (uint8_t)0x7b, (uint8_t)0xe6, (uint8_t)0x6c, (uint8_t)0x37, (uint8_t)0x10 };
	memset(plain_text, 0, 64);
	memset(key, 0, 16);
	read2(key, plain_text);
	memset(buf, 0, 64);
	memset(buf2, 0, 64);

	// print text to encrypt, key and IV
	//printf("key:\n");
	phex(key);

	// print the resulting cipher as 4 x 16 byte strings
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, key);
	//printf("decrypt text:\n");
	for (i = 0; i < 4; ++i)
	{
		AES_ECB_decrypt(&ctx, plain_text + (i * 16));
		
	}

	for (int i = 0; i < 64; i++) printf("%c", plain_text[i]);
}

