#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "combineRGBAndDepthOutput.h"


#include "AcquisitionSegment.h"

#include "imageProcessing.h"

float fx = 537.479600 , fy = 536.572920 , cx = 317.389787 ,cy = 236.118093;


int removeFloodFillBeforeProcessing(unsigned char * source , unsigned char * target , unsigned int width , unsigned int height , struct SegmentationFeaturesRGB * segConf  )
{
  if (segConf->floodErase.totalPoints==0) { return 0; }
  unsigned char sR , sG, sB ;

  int i=0;
  for (i=0; i<segConf->floodErase.totalPoints; i++)
  {
     if (segConf->floodErase.source)
     {

       unsigned char * srcColor = (unsigned char *) source  + ( (segConf->floodErase.pX[i]*3) + segConf->floodErase.pY[i] * width*3 );
       sR = *srcColor; ++srcColor;
       sG = *srcColor; ++srcColor;
       sB = *srcColor; ++srcColor;
       //fprintf(stderr,"Flood Filling Before %u  - %u,%u thresh(%u) \n",i,segConf->floodErase.pX[i],segConf->floodErase.pY[i],segConf->floodErase.threshold[i]);
       //fprintf(stderr,"Src Color %u,%u,%u \n",sR,sG,sB);

       floodFill(source , width, height ,
                 segConf->floodErase.pX[i],segConf->floodErase.pY[i],segConf->floodErase.threshold[i],
                 sR,sG,sB , 0 , 0 , 0    , 0 );
     }
     //fprintf(stderr,"Flood Filled Before %u  - %u,%u thresh(%u) \n",i,segConf->floodErase.pX[i],segConf->floodErase.pY[i],segConf->floodErase.threshold[i]);
  }

  return 1;
}



int removeDepthFloodFillBeforeProcessing(unsigned short * source , unsigned short * target , unsigned int width , unsigned int height , struct SegmentationFeaturesDepth * segConf  )
{
  if (segConf->floodErase.totalPoints==0) { return 0; }
  unsigned short sDepth ;

  int i=0;
  for (i=0; i<segConf->floodErase.totalPoints; i++)
  {
     if (segConf->floodErase.source)
     {

       unsigned short * srcDepth = (unsigned short *) source  + ( (segConf->floodErase.pX[i]) + segConf->floodErase.pY[i] * width );
       sDepth = *srcDepth; ++srcDepth;
       fprintf(stderr,"Flood Filling Before %u  - %u,%u thresh(%u) \n",i,segConf->floodErase.pX[i],segConf->floodErase.pY[i],segConf->floodErase.threshold[i]);
       fprintf(stderr,"Src Depth is %u \n",sDepth);

       floodFillUShort(source , width, height ,
                       segConf->floodErase.pX[i],segConf->floodErase.pY[i],segConf->floodErase.threshold[i],
                       sDepth, 0    , 0 );
     }
     //fprintf(stderr,"Flood Filled Before %u  - %u,%u thresh(%u) \n",i,segConf->floodErase.pX[i],segConf->floodErase.pY[i],segConf->floodErase.threshold[i]);
  }

  return 1;
}



unsigned char * selectSegmentationForRGBFrame(unsigned char * source , unsigned int width , unsigned int height , struct SegmentationFeaturesRGB * segConf)
{
 unsigned char * sourceCopy = (unsigned char *) malloc( width * height * 3 * sizeof( unsigned char));
 if ( sourceCopy == 0) { return 0; }
 memcpy(sourceCopy,source,width*height*3*sizeof(char));


 char * target = (char *) malloc( width * height * 3 * sizeof(char));
 if ( target == 0) {  free(sourceCopy); return 0; }
 memset(target,0,width*height*3*sizeof(char));

 removeFloodFillBeforeProcessing(sourceCopy,target,width,height,segConf);


 unsigned int posX = 0;
 unsigned int posY = 0;
 unsigned int sourceWidthStep = width * 3;

 char * sourcePixelsStart   = (char*) sourceCopy + ( (posX*3) + posY * sourceWidthStep );
 char * sourcePixelsLineEnd = sourcePixelsStart + (width*3);
 char * sourcePixelsEnd     = sourcePixelsLineEnd + ((height-1) * sourceWidthStep );
 char * sourcePixels = sourcePixelsStart;

 unsigned char * selectedRGB   = (unsigned char*) malloc(width*height*sizeof(unsigned char));
 if (selectedRGB==0) { fprintf(stderr,"Could not allocate memory for RGB Selection\n"); return 0; }
 memset(selectedRGB,0,width*height*sizeof(unsigned char));

 unsigned char * selectedPtr   = selectedRGB;

 unsigned int x=0 , y=0;
 unsigned char * R , * G , * B;
 while (sourcePixels<sourcePixelsEnd)
 {

   if ( (y<segConf->minY) || (y>segConf->maxY) )
   {
     sourcePixels+=sourceWidthStep;
     selectedPtr+=width;
   } else
   {
     x=0;
      while (sourcePixels<sourcePixelsLineEnd)
      {
        R = sourcePixels++;
        G = sourcePixels++;
        B = sourcePixels++;

       if  (
             (segConf->minR <= *R) && (*R <= segConf->maxR)  &&
              (segConf->minG <= *G) && (*G <= segConf->maxG)  &&
               (segConf->minB <= *B) && (*B <= segConf->maxB) &&

               (segConf->minX <= x) && ( x<= segConf->maxX)
           )
       {
          *selectedPtr=1;
       } else
       {
          *selectedPtr=0;
       }

        ++selectedPtr;
        ++x;
      }
   }
   sourcePixelsLineEnd+=sourceWidthStep;
   ++y;
 }


 free(sourceCopy);
 return selectedRGB;
}



unsigned char * selectSegmentationForDepthFrame(unsigned short * source , unsigned int width , unsigned int height , struct SegmentationFeaturesDepth * segConf)
{
 short * sourceCopy = (short *) malloc( width * height * sizeof(unsigned short));
 if ( sourceCopy == 0) { return 0; }
 memcpy(sourceCopy,source,width*height*sizeof(short));


 short * target = (short *) malloc( width * height * sizeof(short));
 if ( target == 0) { free(sourceCopy); return 0; }
 memset(target,0,width*height*sizeof(short));

 removeDepthFloodFillBeforeProcessing(sourceCopy,target,width,height,segConf);


 unsigned int sourceWidthStep = width;
 unsigned int targetWidthStep = width;
 unsigned int posX = segConf->minX;
 unsigned int posY = segConf->minY;
 width = segConf->maxX-segConf->minX;
 height = segConf->maxY-segConf->minY;

 short * sourcePixelsStart   = (short*) sourceCopy + ( (posX) + posY * sourceWidthStep );
 short * sourcePixelsLineEnd = sourcePixelsStart + (width);
 short * sourcePixelsEnd     = sourcePixelsLineEnd + ((height-1) * sourceWidthStep );
 short * sourcePixels = sourcePixelsStart;

 unsigned char * selectedDepth   = (unsigned char*) malloc(width*height*sizeof(unsigned char));
 if (selectedDepth==0) { fprintf(stderr,"Could not allocate memory for RGB Selection\n"); return 0; }
 memset(selectedDepth,0,width*height*sizeof(unsigned char));

 unsigned char * selectedPtr   = selectedDepth;

 unsigned int x =0;
 unsigned int y =0;

 /* Todo add accurate 3d point segmentation
 float x3D;
 float y3D;
 float z3D;
 acquisitionGetDepth3DPointAtXY(ModuleIdentifier moduleID,DeviceIdentifier devID,unsigned int x2d, unsigned int y2d , float *x3D, float *y3D , float *z3D  );
 */


 unsigned short * depth;
 while (sourcePixels<sourcePixelsEnd)
 {
   while (sourcePixels<sourcePixelsLineEnd)
    {
     depth = sourcePixels++;


     if (*depth != 0)
     { //If there is a depth given for point
       if  ( (segConf->minDepth <= *depth) && (*depth <= segConf->maxDepth) ) { *selectedPtr=1; } else
                                                                              { *selectedPtr=0; }
     }


     ++selectedPtr;
     ++x;
     if (x>=width) { x=0; ++y;}
    }
   sourcePixelsLineEnd+=sourceWidthStep;
 }


// -------------------------------------------------------------------------------------------------
// --------------------------------- BOUNDING BOX SEGMENTATION -------------------------------------
// -------------------------------------------------------------------------------------------------


if (segConf->enableBBox)
{
 fprintf(stderr,"Selecting Bounding Box %0.2f %0.2f %0.2f -> %0.2f %0.2f %0.2f  \n",segConf->bboxX1,segConf->bboxY1,segConf->bboxZ1,segConf->bboxX2,segConf->bboxY2,segConf->bboxZ2);

 selectedPtr   = selectedDepth;
 sourcePixels = sourcePixelsStart;
 sourcePixelsLineEnd = sourcePixelsStart + (width);
 x=0; y=0;
 while (sourcePixels<sourcePixelsEnd)
 {
   while (sourcePixels<sourcePixelsLineEnd)
    {
     depth = sourcePixels++;

     if ( (*selectedPtr) &&  (*depth != 0) )
     {
      double x3D = (x - cx) * (*depth) / fx;
      double y3D = (y - cy) * (*depth) / fy;
      double z3D = *depth;
      /*! TODO ADD MATRIX MULTIPLICATION WITH EXTRINSICS HERE!*/

       if (
           (segConf->bboxX1<x3D)&& (segConf->bboxX2>x3D) &&
           (segConf->bboxY1<y3D)&& (segConf->bboxY2>y3D) &&
           (segConf->bboxZ1<z3D)&& (segConf->bboxZ2>x3D)
          )
     {  /*If it was selected keep it selected*/ } else
     { *selectedPtr=0; } //Denied
     }//If it was selected and not null project it into 3d Space

     ++selectedPtr;
     ++x;
     if (x>=width) { x=0; ++y;}
    }
   sourcePixelsLineEnd+=sourceWidthStep;
 }
}
// -------------------------------------------------------------------------------------------------
// --------------------------------- BOUNDING BOX SEGMENTATION -------------------------------------
// -------------------------------------------------------------------------------------------------






 free(sourceCopy);
 return selectedDepth;
}


int   segmentRGBAndDepthFrame (    unsigned char * RGB ,
                                   unsigned short * Depth ,
                                   unsigned int width , unsigned int height ,
                                   struct SegmentationFeaturesRGB * segConfRGB ,
                                   struct SegmentationFeaturesDepth * segConfDepth,
                                   int combinationMode
                               )
{
  //We have some criteria for segmentation at segConfRGB , and segConfDepth
  //First of all we want to use them and the original frames (RGB,Depth) , and select ( on an abstract array )
  //the areas we want and the areas we don't want ..

  //We select the area for segmentation from RGB frame
  unsigned char * selectedRGB = selectSegmentationForRGBFrame(RGB , width , height , segConfRGB);

  //We select the area for segmentation from Depth frame
  unsigned char * selectedDepth = selectSegmentationForDepthFrame(Depth , width , height , segConfDepth);

  //We may chose to combine , or make a different selection for the RGB and Depth Frame
  if ( combinationMode == DONT_COMBINE )
  {
     //If we dont want to combine them we just execute the selection and RGB will
     //now have the segmented output frame
     executeSegmentationRGB(RGB,selectedRGB,width,height,segConfRGB);
     //The same goes for Depth
     executeSegmentationDepth(Depth,selectedDepth,width,height,segConfDepth);
  } else
  {
     //If we do want to combine the selections "Together" , and there are many ways to do that using
     //the combinationMode switch ( see enum combinationModesEnumerator ) , we first make a new selection
     //and then execute the segmentation using it
     unsigned char *  combinedSelection = combineRGBAndDepthToOutput(selectedRGB,selectedDepth,combinationMode,width,height);
     if (combinedSelection==0)
     {
       fprintf(stderr,"Failed to combine outputs using method %u\Cannot execute segmentation\n",combinationMode);
     } else
     {
      //We use the combinedSelection for both RGB and Depth
      executeSegmentationRGB(RGB,combinedSelection,width,height,segConfRGB);
      executeSegmentationDepth(Depth,combinedSelection,width,height,segConfDepth);

      //And we dont forget to free our memory
      if (combinedSelection!=0) { free(combinedSelection); combinedSelection=0; }
     }
  }


  //Free memory from selections..
  if (selectedRGB!=0) { free(selectedRGB); selectedRGB=0; }
  if (selectedDepth!=0) { free(selectedDepth); selectedDepth=0; }

  return 1;
}
