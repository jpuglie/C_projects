/* 
Jonathan Pugliese
ECE 222 
Programming Project 3
02/22/17
   
   This program asks the user for a hexadecimal input of up to 32 characters (128 bits), and then converts this hexadecimal number into a binary number. From there, huffman code is implemented in order to decode the binary into a word or phrase
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{

char hexString[33];
unsigned long long int maskValue;
unsigned long long int maskAdder = 1;
unsigned long long int huffmanCode[30];
unsigned int sizeBits[30];
char alphabet[] = "ETAOINSHRDLCUMWFGYPBVKJGXZ["; 
int hexLength;
int whichCode = 0;
int maskMover = 0;
int nibbleCounter = 0;
int match;
int again = 0;
int number_of_bits;
char *end = "quit";
unsigned long long int inputBinary;
unsigned long long int charCode;
int number_of_Nibbles;
while(again != 1)
{
	printf("\nEnter a Hexadecimal number with up to 32 characters or type quit: ");
	scanf("%s", hexString);

	sscanf(hexString, "%llx", &inputBinary);
	
		if( strcasecmp(hexString, end) == 0)
		{
			again = 1;
		}
		else
		{
			again = 0;
		}

        huffmanCode[0] = 0b011; /*E*/ sizeBits[0] = 3;		huffmanCode[15] = 0b11011; /*F*/ sizeBits[15] = 5;
	huffmanCode[1] = 0b111; /*T*/ sizeBits[1] = 3;		huffmanCode[16] = 0b001100; /*G*/ sizeBits[16] = 6;
	huffmanCode[2] = 0b0001; /*A*/ sizeBits[2] = 4;		huffmanCode[17] = 0b001101; /*Y*/ sizeBits[17] = 6;
	huffmanCode[3] = 0b0010; /*O*/ sizeBits[3] = 4;		huffmanCode[18] = 0b001110; /*P*/ sizeBits[18] = 6;
	huffmanCode[4] = 0b0100; /*I*/ sizeBits[4] = 4;		huffmanCode[19] = 0b001111; /*B*/ sizeBits[19] = 6;
	huffmanCode[5] = 0b0101; /*N*/ sizeBits[5] = 4;		huffmanCode[20] = 0b110011; /*V*/ sizeBits[20] = 6;
	huffmanCode[6] = 0b1000; /*S*/ sizeBits[6] = 4;		huffmanCode[21] = 0b1100100; /*K*/ sizeBits[21] = 7;
	huffmanCode[7] = 0b1001; /*H*/ sizeBits[7] = 4;		huffmanCode[22] = 0b11001011; /*J*/ sizeBits[22] = 8;
	huffmanCode[8] = 0b1010; /*R*/ sizeBits[8] = 4;		huffmanCode[23] = 0b1100101000; /*Q*/ sizeBits[23] = 10;
	huffmanCode[9] = 0b00000; /*D*/ sizeBits[9] = 5;	huffmanCode[24] = 0b1100101001; /*X*/ sizeBits[24] = 6;
	huffmanCode[10] = 0b00001; /*L*/ sizeBits[10] = 5;	huffmanCode[25] = 0b1100101010; /*Z*/ sizeBits[25] = 10;
	huffmanCode[11] = 0b10110; /*C*/ sizeBits[11] = 5;	huffmanCode[26] = 0b1100101011; /*EOT*/ sizeBits[26] = 10;
	huffmanCode[12] = 0b10111; /*U*/ sizeBits[12] = 5;
	huffmanCode[13] = 0b11000; /*M*/ sizeBits[13] = 5;
	huffmanCode[14] = 0b11010; /*W*/ sizeBits[14] = 5;


 	whichCode = 0;
	maskMover = 0;
	nibbleCounter = 0;	
	number_of_Nibbles = strlen(hexString);
	number_of_bits = number_of_Nibbles*4;			
	maskValue = 1;
	maskValue = maskValue<<63;
	maskAdder = maskAdder<<63;
while(nibbleCounter<number_of_Nibbles)
{
	while(whichCode<26)
        {
		maskValue = 1;
		if(huffmanCode[whichCode-1] == charCode)
		{
			maskValue = maskValue<<(64-sizeBits[whichCode-1]);
		}
		else
		{
			maskValue = maskValue<<63;
		}
	for(maskMover=0;maskMover<10;maskMover++)
	{	
		charCode = (inputBinary & maskValue);
					
															
		printf("\n whichCode = %d , maskMover = %d", whichCode, maskMover);
		printf("\ncharCode: %llu", charCode);
		printf("\nhuffmanCode: %llu", huffmanCode[whichCode]<<(64-sizeBits[whichCode]));
		printf("\ninputBinary : %llu", inputBinary<<(64-sizeBits[whichCode]));			

		if(huffmanCode[whichCode]<<(64-sizeBits[whichCode]) == charCode)
		{
			printf("%c", alphabet[whichCode]);
			inputBinary = inputBinary>>sizeBits[whichCode];
			break;
		}
		else
		{
			maskValue = maskValue>>1;
				maskValue = maskValue + maskAdder;
			}
		}
	if(whichCode>6)
	{
		break;
	}
	whichCode++;		
	}
nibbleCounter++;
}
}
return 0;
}
































