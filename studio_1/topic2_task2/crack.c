#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define KEYSIZE 16

void main()
{
	//int t = 1524013729; //result of date -d "2018-04-17 21:08:49" +%s
	int t = 1524010000;
	int i;

	FILE *fp;	
	fp = fopen( "keys.txt", "w" );

	while (t < 1524030000 ) {
	//while (t < 1524020929 + 1) {
		srand (t);
		
		for(i=0; i < KEYSIZE; i++) {
			fprintf (fp, "%02x", rand() % 256);	
		}
		fputs ("\n", fp);
		t++;
	}
}

