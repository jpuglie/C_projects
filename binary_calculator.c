/*Jonathan Pugliese
  ECE 2220
  Programming Assignment 1
  1/20/2017

  Program is a binary calculator. User enters two binary numbers and an 
  operand, and the program outputs the calculation. The binary numbers must each be 31 bits or less 
*/

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>

int main()
{

char operand1_string[55];
char *aPtr, *bPtr;
char operationChoice = ' ';
char operand2_string[55];
long int operand1_Decimal;
long int operand2_Decimal;
long int answer_Decimal;
unsigned char answer_Binary[64];
int operand1_Length;
int operand2_Length;
// This section of code takes in the user inputs and converts the binary strings into decimal integers

// Show the user the input format
printf("\nInput format (binary operands no more than 31 bits. Operations  +,-,x. or/):\nOperand1 Operation Operand2 [Enter]\n");

// Scan in operand1
scanf( "%s", operand1_string);


while( strcmp(operand1_string,"quit") || strcmp(operand1_string,"QUIT"))	// Keeps loop running if the user does not choose to quit
{

operand1_Length = strlen(operand1_string);
operand2_Length = strlen(operand2_string);

	
if(operand1_Length > 32 || operand2_Length > 32)
{
	printf("\n\n Error: The maximum capacity for each operand is 31 bits");
	printf("\n");
	scanf( "%s", operand1_string);   
}
if( !strcmp(operand1_string,"quit")|| !strcmp(operand1_string,"QUIT"))      // Check if user wants to quit
{
	printf("\nGood bye");
	printf("\n");
	return 0;
}

                              
// Scan in operation and operand2      
scanf(" %c",&operationChoice);
scanf("%s", operand2_string);

// Changes binary strings entered into a decimal integer				
operand1_Decimal = strtoul(operand1_string, &aPtr, 2);	     
operand2_Decimal = strtoul(operand2_string, &bPtr, 2);

// This section of code preforms the mathematical functions that the user desires

if( operationChoice == '+')
{
	answer_Decimal = operand1_Decimal + operand2_Decimal;
}

else if( operationChoice == '-')
{
	answer_Decimal = operand1_Decimal - operand2_Decimal;
}

// Note: all 3 symbols may possibly be entered (even though the lowercase x is stated to be correct)
else if( (operationChoice == 'x') || (operationChoice == 'X') || (operationChoice == '*')) 
{
	answer_Decimal = operand1_Decimal * operand2_Decimal;
}
else if( operationChoice == '/')
{
	answer_Decimal = operand1_Decimal / operand2_Decimal;
}
else
{
	printf("Please enter a valid operation. Your choices are +, -, x, or /");
}


// This section of code converts the decimal answer back to binary, then prints it for the user

char base_digits[2]= {'0','1'};
int index=0;
int base = 2;

while(answer_Decimal != 0)
{
        answer_Binary[index] = answer_Decimal % base;
        answer_Decimal = answer_Decimal / base;
        ++index;
}
--index;
printf("= ");
for( ; index>=0;index--)
{
        printf("%c", base_digits[answer_Binary[index]]);
}

printf("\n\n");
printf("Enter new operands or type 'quit'");
printf("\n");
scanf( "%s", operand1_string);   
}
return 0;
}

