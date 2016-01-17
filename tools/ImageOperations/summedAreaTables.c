#include "summedAreaTables.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int * generateSummedAreaTableRGB(unsigned char * source,  unsigned int sourceWidth , unsigned int sourceHeight )
{
 fprintf(stderr,"generateSummedAreaTable(%p , %u , %u )\n",source,sourceWidth , sourceHeight);
 if ( (sourceWidth>4000 ) && (sourceHeight>4000) )
 {
   fprintf(stderr,"generateSummedAreaTable cannot contain so big frame sizes \n");
   return 0;
 }

 unsigned int *  sat = (unsigned int * ) malloc(sizeof(unsigned int) * sourceWidth * sourceHeight*3 );
 if (sat==0) { fprintf(stderr,"generateSummedAreaTable could not allocate table\n"); return 0; }
 unsigned int * satOut = sat;

 unsigned int memplace=0;
 unsigned int nextLineOffset = (sourceWidth*3);

 unsigned char * inPtr = source;
 unsigned char * inLineLimit = inPtr + nextLineOffset;
 unsigned char * inLimit = inPtr + (sourceWidth*sourceHeight*3) ;

 //First horizontal line is special since it has no vertical additions
 unsigned int pSumR=0,pSumG=0,pSumB=0;
 while (inPtr<inLineLimit)
 {
   *satOut = (unsigned int) pSumR + (*inPtr); pSumR=*satOut; ++inPtr; ++satOut;
   *satOut = (unsigned int) pSumG + (*inPtr); pSumG=*satOut; ++inPtr; ++satOut;
   *satOut = (unsigned int) pSumB + (*inPtr); pSumB=*satOut; ++inPtr; ++satOut;
 }
 inLineLimit+=nextLineOffset;

 unsigned int *outUpPtr=sat , *outUpLeftPtr=sat , *outLeftPtr=satOut;
 //Ready for the main loop
 while (inPtr<inLimit)
 {
    outLeftPtr=satOut;
    *satOut = (unsigned int) (*inPtr) + (*outUpPtr); ++inPtr; ++outUpPtr; ++satOut;
    *satOut = (unsigned int) (*inPtr) + (*outUpPtr); ++inPtr; ++outUpPtr; ++satOut;
    *satOut = (unsigned int) (*inPtr) + (*outUpPtr); ++inPtr; ++outUpPtr; ++satOut;

    while (inPtr<inLineLimit)
    {
      *satOut = (unsigned int) (*inPtr) + (*outLeftPtr) +  (*outUpPtr);
      *satOut -= (*outUpLeftPtr);
      ++inPtr; ++outUpPtr; ++outUpLeftPtr; ++outLeftPtr; ++satOut;

      *satOut = (unsigned int) (*inPtr) + (*outLeftPtr) +  (*outUpPtr);
      *satOut -= (*outUpLeftPtr);
      ++inPtr; ++outUpPtr; ++outUpLeftPtr; ++outLeftPtr; ++satOut;


      *satOut = (unsigned int) (*inPtr) + (*outLeftPtr) +  (*outUpPtr);
      *satOut -= (*outUpLeftPtr);
      ++inPtr; ++outUpPtr; ++outUpLeftPtr; ++outLeftPtr; ++satOut;

    }
    inLineLimit+=nextLineOffset;
    if (inLineLimit>inLimit) { /* fprintf(stderr,"Border case\n"); */ inLineLimit=inLimit; }
    outUpLeftPtr=outUpPtr;
 }


 return sat;
}



int summedAreaTableTest()
{
  fprintf(stderr,"summedAreaTableTest()\n");
  unsigned char sourceRGB[]={
                             1,0,2 , 1,0,2 , 1,0,2 , 1,0,2 , 1,0,2 ,
                             1,0,2 , 1,0,2 , 1,0,2 , 1,0,2 , 1,0,2 ,
                             1,0,2 , 1,0,2 , 1,0,2 , 1,0,2 , 1,0,2 ,
                             1,0,2 , 1,0,2 , 1,0,2 , 1,0,2 , 1,0,2 ,
                             1,0,2 , 1,0,2 , 1,0,2 , 1,0,2 , 1,0,2 ,
                             1,0,2 , 1,0,2 , 1,0,2 , 1,0,2 , 1,0,2
                          } ;

 unsigned int * output = generateSummedAreaTableRGB(sourceRGB ,  5 , 5 );

 unsigned int x,y,z=0;

 fprintf(stderr,"Input\n");
 for (y=0; y<5; y++)
 { for (x=0; x<5; x++)
   {
    fprintf(stderr,"%u %u %u ",(unsigned int) sourceRGB[z],(unsigned int) sourceRGB[z+1],(unsigned int) sourceRGB[z+2]);
    z+=3;
   }
   fprintf(stderr,"\n");
 }
 fprintf(stderr,"\n\n\n\n\n\n");

 fprintf(stderr,"Output\n");
 z=0;
 for (y=0; y<5; y++)
 { for (x=0; x<5; x++)
   {
    fprintf(stderr,"%u %u %u ",output[z],output[z+1],output[z+2]);
    z+=3;
   }
   fprintf(stderr,"\n");
 }
 fprintf(stderr,"\n\n\n\n\n\n");


 return 1;
}
