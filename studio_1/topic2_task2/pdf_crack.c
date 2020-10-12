//OpenSSL resource: http://www.firmcodes.com/how-do-aes-128-bit-cbc-mode-encryption-c-programming-code-openssl/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <openssl/aes.h>

#define KEYSIZE 16


//Prints a byte array nicely in one line
int printArray(unsigned char array[], int size) {
	int j;
	for (j = 0; j < size; j++) {
                printf("%02x", array[j]);
        }
	printf("\n");
	return 0;
}

//Compares two byte arrays of size size
//Returns 0 if byte arrays are the same
//Returns -1 if byte arrays are different
int compare(unsigned char one[], unsigned char two[], int size) {
        int j; 
        for (j = 0; j < size; j++) {
                if (one[j] != two[j] ) {
                        return -1;
                }
        }
        return 0;
}


int main() {
	//printf("%lld\n", (long long) time(NULL));
	int t = 1524013729; //result of date -d "2018-04-17 21:08:49" +%s
	char key[KEYSIZE];
	int i;

	unsigned char aes_input[] = {0x25,0x50,0x44,0x46,0x2d,0x31,0x2e,0x35,0x0a,0x25,0xd0,0xd4,0xc5,0xd8,0x0a,0x34};
	unsigned char aes_output[] = {0xd0,0x6b,0xf9,0xd0,0xda,0xb8,0xe8,0xef,0x88,0x06,0x60,0xd2,0xaf,0x65,0xaa,0x82};
	unsigned char iv[] = {0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00,0xA2,0xB2,0xC2,0xD2,0xE2,0xF2};
	unsigned char aes_result[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

	//Test to make sure compare() works (it does)
	//unsigned char one[] = {0x00, 0x01, 0x02, 0x03};
	//unsigned char two[] = {0x00, 0x01, 0x02, 0x03};
	//printf("comparison: %d\n", compare(one, two, 4));

	
	while ( compare (aes_output, aes_result, KEYSIZE) != 0) {
		srand(t);
		for (i=0; i < KEYSIZE; i++) {
			key[i] = rand()%256;
		}
		
		//Encrypt aes_input with key that was seeded by srand(t)
		AES_KEY enc_key;
		AES_set_encrypt_key(key, KEYSIZE*8, &enc_key);
		AES_cbc_encrypt(aes_input, aes_result, sizeof(aes_input), &enc_key, iv, AES_ENCRYPT);
		t++;

		//Uncomment to print result of encryption every time
		//printf("aes_result: ");
		//printArray(aes_result, KEYSIZE);
		
		if (t > 1524020929) {
			printf("not found :(\n");
			return -1;
		}
	}

	printf("found key: ");
	printArray(key, KEYSIZE);

	return 0;

}
