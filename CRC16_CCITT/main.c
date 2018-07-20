
#define CRC16			0xA001


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

unsigned int DEVICE_CRC16 = 0x00;


unsigned int calcCRC16r(unsigned int crc, unsigned int *c, unsigned int mask)
{
  unsigned char i;
  for(i=0;i<8;i++)
  {
    if((crc ^ *c) & 1) 
	{ 
		crc=(crc>>1)^mask; 
	}
    else 
    {
    	crc>>=1;	
	}
    *c>>=1;
  }
  return (crc);
}

void crc_update_str(unsigned char *str)
{
	unsigned int s = 0;
	while(*str)
	{
		s = *str++;
	 	DEVICE_CRC16 = calcCRC16r(DEVICE_CRC16,&s,CRC16);	
	}	
}


unsigned char UserIn;

int main(int argc, char *argv[]) 
{
	printf("*******************************************\n");
	printf("*	CRC16 CCITT from String\n");
	printf("*	Build.: 15.11.2016 | made by Hm(C)\n");
	printf("*******************************************\n");
	
	printf("CRC16 from String.:\n\n");
	printf("-> ");
	gets(&UserIn);
	
	crc_update_str(&UserIn);
	
	printf("****************************\n");
	printf("*	CRC -> %X\n",DEVICE_CRC16);
	printf("****************************\n\n");	
		
	system("pause");	
}



