#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <openssl/aes.h>

#define KEYSIZE 16


int compare(unsigned char one[], unsigned char two[]) {
	int j;
	for (j = 0; j < sizeof(one); j++) {
                //printf("%02X", key[i]);
		if (one[j] != two[j] ) {
			return -1;
		}
        }
	return 0;
}

int main() {

	int t = 1524013729; //result of date -d "2018-04-17 21:08:49" +%s
	char key[KEYSIZE];
	int i;

	unsigned char aes_input[] = {0x25,0x50,0x44,0x46,0x2d,0x31,0x2e,0x35,0x0a,0x25,0xd0,0xd4,0xc5,0xd8,0x0a,0x34};
	unsigned char aes_output[] = {0xd0,0x6b,0xf9,0xd0,0xda,0xb8,0xe8,0xef,0x88,0x06,0x60,0xd2,0xaf,0x65,0xaa,0x82};
	unsigned char iv[] = {0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00,0xA2,0xB2,0xC2,0xD2,0xE2,0xF2};
	unsigned char aes_result[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	
	while (compare(aes_output, aes_result) != 0) {
		srand(t);
		for (i=0; i < KEYSIZE; i++) {
			key[i] = rand()%256;
		}
		
		AES_KEY enc_key;
		AES_set_encrypt_key(key, KEYSIZE*8, &enc_key);
		AES_cbc_encrypt(aes_input, aes_result, sizeof(aes_input), &enc_key, iv, AES_ENCRYPT);
		t++;
		//printf("aes_output is %s, aes_result is %s, new time is %d\n", aes_output, aes_result, t);
		printf("%d\n",t);
		
		printf("Key: ");
        for (i = 0; i < KEYSIZE; i++) {
                //printf("%02X", key[i]);
                printf("%02x", key[i]);
        }
        printf("\naes_result: ");
        for (i = 0; i < sizeof(aes_result); i++) {
                //printf("%02X", key[i]);
                printf("%02x", aes_result[i]);
        }
        printf("\n");

		if (t > 1524013738) {
			printf("not found :(\n");
			break;
		}
	}
	//printf("Key: ");
	//for (i = 0; i < KEYSIZE; i++) {
	//	//printf("%02X", key[i]);
	//	printf("%x", key[i]);
	//}
	//printf("\naes_result: ");
	//for (i = 0; i < sizeof(aes_result); i++) {
        //        //printf("%02X", key[i]);
        //        printf("%x", aes_result[i]);
        //}
	//printf("\n");

	return 0;

}
