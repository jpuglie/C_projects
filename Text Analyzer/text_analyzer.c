/* Jonathan Pugliese
   04/07/2017

   The purpose of this program is a text analyzer that executes several different functions including
   word count, letter count, finding words with the string "ous", finding words
   that were not in the dictionary, finding words only made up of "aceslmtw", and counting the number of each 
   length of word. The program then prints the results to both an output file as well as on the screen.

   Assumptions: I made several assumptions when creating this program. For instance; 
   I was unable to create dynamically allocated arrays successfully, and thus as a work around I assumed that there would 
   Never be more than 300 addendum words. I also looked up how many different combonations of "aceslmtw" can be made into
   words, and the answer is 240,and thus I alotted that amount into my SpecialWord array in lieu of allocation.

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//Counts words by grabbing individual characters, and if the character is a space that means a word follows 
int wordCount(FILE *fp)
{

int count = 0;
int False = 0;
int True = 1;
int charFind = False;
char fileChar;

while(!feof(fp))
{
        fscanf(fp,"%c", &fileChar);

        if (fileChar == ' ')
        {
                charFind = False;
        }

        else
        {
                if (charFind == False)
                {
                        count++;
                        charFind = True;
                }
        }
}
rewind(fp);
return count;
}

// Finds words that contain the substring 'ous'
int findSubstring(FILE *fp, FILE *outputPtr, int i)
{
char subString[4] = "OUS" ;
char stringGrab[20];
char stringExist[400][20]; 
rewind(fp);
int stringLength = 5;
int j = 0;
int n = 0;
int k = 0;
int TRUE = 1;
int FALSE = 0;
int Exists = FALSE;
while(!feof(fp))
{
        fscanf(fp,"%s", stringGrab);
        while(stringGrab[n])
        {
                stringGrab[n] = toupper(stringGrab[n]);
                n++;
        }

        if(strstr(stringGrab, subString) != NULL)
        {
		stringLength = strlen(stringGrab);
		for(k = 0; k<stringLength; k++)
		{
			if(!isalpha(stringGrab[k]))
			{
				memmove(&stringGrab[k],&stringGrab[k+1],stringLength-k);
			}
		}
		while(j<300)
		{
			if(strcasecmp(stringExist[j],stringGrab) == 0)
			{
				Exists = TRUE;
			}
		j++;
		}	
		if(Exists == FALSE)
		{
			printf("\nSubStringWord[%d] = %s", i, stringGrab);                
                	fprintf(outputPtr,"\nSubStringWord[%d] = %s", i, stringGrab);
                	strcpy(stringExist[i], stringGrab);
			i++;
		}
        }

n = 0;
j = 0;
Exists = FALSE;
}
return i;
}

// Locates words with only the letters ACESLMTW
int findSpecialWords(FILE *fp, FILE *outputPtr, char title[20],int k)
{
FILE *tempfp;
char specialGrab[20];
char tempGrab[20];
char specialWord[240][20];
int specialCount = 0;
int TRUE = 1;
int FALSE = 0;
int Exists;
int specLength;
rewind(fp);
tempfp = fopen(title, "r");
int n = 0;
int i = 0;
while(!feof(fp))
{
        fscanf(fp,"%s ", specialGrab);
        while(specialGrab[n])
        {
                specialGrab[n] = toupper(specialGrab[n]);
		tempGrab[n] = toupper(tempGrab[n]);
                n++;
        }
        
        specLength = strlen(specialGrab);
        for(i = 0; i<specLength;i++)
        {
                if((strspn(specialGrab, "ACESLMTW") == strlen(specialGrab)))
                {
                        
                        Exists = TRUE;
			while(!feof(tempfp))
	        	{
                        	fscanf(tempfp, "%s", tempGrab);
                        	if(strcasecmp(tempGrab, specialGrab) == 0)
                        	{
                                	specialCount++;
                        	}
                	}

                }

            

                else
                {
                        Exists = FALSE;
                        break;
                }

        }
        n = 0;
        while(n<240)
        {
                if(strcasecmp(specialWord[n],specialGrab) == 0)
                {
                        
			Exists = FALSE;
                }
        n++;
        }
        
        if(Exists == TRUE)
        {
		
		n = 0;
		rewind(fp);
                fprintf(outputPtr, "\nSpecialWord[%d] = %s", k, specialGrab);
                printf("\nSpecialWord[%d] = %s", k, specialGrab);
		strcpy(specialWord[k], specialGrab);
                k++;
        }
                        
n = 0;
specialCount = 0;
}
return k;
}

// Determines word of the longest length
int findMaxLength(FILE *fp, int maxlength)
{
char string[35];
int stringLength;
rewind(fp);
int tempMax = 0;
fscanf(fp,"%s", string);
stringLength = strlen(string);
        while(!feof(fp))
        {
                fscanf(fp,"%s", string);
                stringLength = strlen(string);
                if(stringLength > tempMax)
                {
                        tempMax = stringLength;
                }

		if(tempMax > maxlength)
		{
        		maxlength = tempMax;
		}
	}
return maxlength;
}

int main(int argc, char **argv)
{
FILE *fp;
FILE *outputPtr;
int fileCounter;
static int totalAlphaCount[28];
long int totalLetters = 0;
char title[20];
char output[20];
char string[20];
char addendumWords[300][20];
int g = 0;
int x = 0;
int i=0;
int j=0;
int k=0;
int w=0;
static int lengthArray[20];
int count=0;
int maxlength = 0;
long int totalCount = 0;
int z = 0;
int alphaCount = 0;
char fileChar;
int letterCount = 0;
char alphabet[30] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


sprintf(title,"%s_1.txt",argv[1]);
sprintf(output, "%s.txt", argv[2]);
fp = fopen(title,"r");
outputPtr = fopen(output, "w");

while(fp != NULL)
{
sprintf(title,"%s_%d.txt",argv[1],(i+2));
fp = fopen(title,"r");
i++;
}

fileCounter = i;

for(i = 0; i<6; i++)
{

	for(j = 1; j<=fileCounter; j++)
	{
	sprintf(title, "%s_%d.txt", argv[1],j);
	fp = fopen(title, "r");
		
		if(i == 0)
		{
			count = wordCount(fp);
			totalCount += count;
		}
			
	
		else if(i == 1)
		{
	
		rewind(fp);
		while(!feof(fp))
		{
       		 fscanf(fp,"%c", &fileChar);
        		if(isalpha(fileChar))
        		{
                		letterCount++;
        		}

		}
		rewind(fp);
		for(z = 0; z<26; z++)
		{

        		while(!feof(fp))
        		{

        		fscanf(fp,"%c",&fileChar);

                		if(isalpha(fileChar))
                		{
                        		fileChar = toupper(fileChar);
                        		if(fileChar == alphabet[z])
                        		{
                
						alphaCount++;
                        		}

                		}


        		}
			totalAlphaCount[z] += alphaCount;
			alphaCount = 0;
			rewind(fp);
		}
		
		for(z = 0; z<26; z++)
                {
                        totalLetters += totalAlphaCount[z];
                }
                

		}
		
		if(i == 2)
		{
			x = findSubstring(fp, outputPtr, x);
		}
			
		if( i == 3)
		{
			g = findSpecialWords(fp, outputPtr, title,g);
		}
		
		if(i == 4)
		{
			rewind(fp);			
			int lengthCounter = 0;
			maxlength = findMaxLength(fp, maxlength);		
		rewind(fp);
		for(w = 1; w<=maxlength; w++)
		{
        		while(!feof(fp))
        		{
                		fscanf(fp,"%s", string);
                		int stringLength = strlen(string);

                		if(stringLength == w)
                		{
                        	lengthCounter++;
                		}
				

        		}
		lengthArray[w] += lengthCounter;
		rewind(fp);
		lengthCounter = 0;
		}	
		}
		
		if(i == 5)
		{

			FILE *dictPtr;
			int TRUE = 1;
			int FALSE = 0;
			int Exists = FALSE;
			int i = 0;
			int f = 0;
			char fileString[30];
			char dictString[30];
			dictPtr = fopen("dictionary.txt", "r");
			rewind(fp);

			int count = wordCount(fp);
			while(!feof(fp))
			{
        			fscanf(fp,"%s",fileString);
        
        			while(fileString[i])
        			{
                			fileString[i] = toupper(fileString[i]);
                			i++;
        			}
        			while(!feof(dictPtr))
        			{
                		fscanf(dictPtr, "%s", dictString);
                
                			while(dictString[f])
                			{
                        			dictString[f] = toupper(dictString[f]);
                        			f++;
                			}
                                
                			if(strcmp(dictString,fileString) == 0)
                			{
                        			count--;                
                			}

        				f = 0;
        			}       
			i = 0;
			rewind(dictPtr);
			}
			rewind(fp);
			char string[30];
			int stringLength;
			fscanf(fp,"%s", string);
			int maxLength = strlen(string);

        		while(!feof(fp))
        		{
                		fscanf(fp,"%s", string);
                		stringLength = strlen(string);

                		if(stringLength > maxLength)
                		{
                        		maxLength = stringLength;
                		}
        		}
			rewind(fp);
			rewind(dictPtr);
i = 0;
f = 0;
int n = 0;
while(!feof(fp))
{
        fscanf(fp,"%s",fileString);

        while(fileString[i])
        {
                fileString[i] = toupper(fileString[i]);
                i++;
        }
        
        stringLength = strlen(fileString);

        while(!feof(dictPtr))
        {
                for(n = 0; n < stringLength; n++)
                {
                        if((!isalpha(fileString[n]) && (!isalpha(fileString[n+1]))))
                        {
                                Exists = TRUE;
                                break;
                        }
                        else if(!isalpha(fileString[n]))
                        {
                                memmove(&fileString[n],&fileString[n+1],stringLength-n);
                        }
                        
                }
                fscanf(dictPtr, "%s", dictString);

                while(dictString[f])
                {
                        dictString[f] = toupper(dictString[f]);
                        f++;
                }

                
                if(strcasecmp(dictString,fileString) == 0)
                {
                        Exists = TRUE;
                        break;       
                }

        }
	f = 0;
	while(f<250)
	{
		if(strcasecmp(addendumWords[f],fileString) == 0)
		{
			Exists = TRUE;
		}
		f++;
	}

	if(Exists == FALSE)
	{
        printf("\nWord[%d] = %s", k, fileString);
        fprintf(outputPtr,"\nWord[%d] = %s",k, fileString);
        strcpy(addendumWords[k],fileString);
        k++;
	}
i = 0;
f = 0;
Exists = FALSE;
rewind(dictPtr);
}

		}
	

	
	}	
}
fprintf(outputPtr,"\nWord Count: %ld\n", totalCount);
printf("\nWord Count: %ld\n", totalCount);
printf("\n");
for(i = 0; i<26; i++)
                {
                        printf( "\nTotal Letters %c = %d",alphabet[i],totalAlphaCount[i]);
			fprintf(outputPtr,"\nTotal Letters %c = %d", alphabet[i], totalAlphaCount[i]);
                }
printf("\n");
fprintf(outputPtr, "\n");
i = 0;
for(i = 1; i <= maxlength; i++)
{
		fprintf(outputPtr,"\nTotal Words of Length %d = %d", i, lengthArray[i]);
                printf("\nTotal words of Length %d = %d", i, lengthArray[i]);
}
printf("\n");

return 0;
}

