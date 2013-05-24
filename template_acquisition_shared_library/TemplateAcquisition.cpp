#include "TemplateAcquisition.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned int templateWIDTH=640;
unsigned int templateHEIGHT=480;
char * templateColorFrame = 0;
short * templateDepthFrame = 0;

#define MAX_DIR_PATH 1024
char readFromDir[MAX_DIR_PATH]={0}; // <- this sucks i know :P
unsigned short cycle=0;


#define PPMREADBUFLEN 256



int FileExists(char * filename)
{
 FILE *fp = fopen(filename,"r");
 if( fp ) { /* exists */
            fclose(fp);
            return 1;
          }
 /* doesnt exist */
 return 0;
}


char * ReadPPM(char * filename,unsigned int *width,unsigned int *height)
{
    fprintf(stderr,"Reading template file %s \n",filename);
    char * pixels=0;
    FILE *pf=0;
    pf = fopen(filename,"rb");

    if (pf!=0 )
    {
        char buf[PPMREADBUFLEN], *t;
        unsigned int w=0, h=0, d=0;
        int r=0;

        t = fgets(buf, PPMREADBUFLEN, pf);
        if ( (t == 0) || ( strncmp(buf, "P6\n", 3) != 0 ) ) { fprintf(stderr,"ReadPPM only undertsands P6 format\n"); fclose(pf); return 0; }
        do
        { /* Px formats can have # comments after first line */
           t = fgets(buf, PPMREADBUFLEN, pf);
           if ( t == 0 ) { fclose(pf); return 0; }
        } while ( strncmp(buf, "#", 1) == 0 );
        r = sscanf(buf, "%u %u", &w, &h);
        if ( r < 2 ) { fclose(pf); fprintf(stderr,"Incoherent dimensions received %ux%u \n",w,h); return 0; }
        // The program fails if the first byte of the image is equal to 32. because
        // the fscanf eats the space and the image is read with some bit less
        r = fscanf(pf, "%u\n", &d);
        if ( (r < 1) || ( d != 255 ) ) { fprintf(stderr,"Incoherent payload received %u bits per pixel \n",d); fclose(pf); return 0; }


        *width=w;
        *height=h;
        pixels= (char*) malloc(w*h*3*sizeof(char));

        if ( pixels != 0 )
        {
          size_t rd = fread(pixels,3, w*h, pf);
          if (rd < w*h )
             {
               fprintf(stderr,"Note : Incomplete read while reading file %s (%u instead of %u)\n",filename,(unsigned int) rd, w*h);
             }

          fclose(pf);
          //if ( rd < w*h ) { return 0; }
          return pixels;
        } else
        {
            fprintf(stderr,"Could not Allocate enough memory for file %s \n",filename);
        }
        fclose(pf);
    } else
    {
      fprintf(stderr,"File %s does not exist \n",filename);
    }

  return 0;
}




short * ReadPPMD(char * filename,unsigned int *width,unsigned int *height)
{
    fprintf(stderr,"Reading template file %s \n",filename);
    short * pixels=0;
    FILE *pf=0;
    pf = fopen(filename,"rb");

    if (pf!=0 )
    {
        char buf[PPMREADBUFLEN], *t;
        unsigned int w=0, h=0, d=0;
        int r=0;

        t = fgets(buf, PPMREADBUFLEN, pf);
        if ( (t == 0) || ( strncmp(buf, "P5\n", 3) != 0 ) ) { fclose(pf); return 0; }
        do
        { /* Px formats can have # comments after first line */
           t = fgets(buf, PPMREADBUFLEN, pf);
           if ( t == 0 ) { fclose(pf); return 0; }
        } while ( strncmp(buf, "#", 1) == 0 );
        r = sscanf(buf, "%u %u", &w, &h);
        if ( r < 2 ) { fclose(pf); return 0; }
        // The program fails if the first byte of the image is equal to 32. because
        // the fscanf eats the space and the image is read with some bit less
        r = fscanf(pf, "%u\n", &d);
        if ( (r < 1) /*|| ( d != 255 )*/ ) { fclose(pf); return 0; }


        *width=w;
        *height=h;
        pixels= (short*) malloc(w*h*3*sizeof(short));

        if ( pixels != 0 )
        {
          size_t rd = fread(pixels,sizeof(short), w*h, pf);
          if (rd < w*h) { fprintf(stderr,"Note : Incomplete read while reading file %s (%u instead of %u)\n",filename,(unsigned int) rd,w*h);  }

          fclose(pf);
          return pixels;
        } else
        {
            fprintf(stderr,"Could not Allocate enough memory for file %s \n",filename);
        }
        fclose(pf);
    } else
    {
      fprintf(stderr,"File %s does not exist \n",filename);
    }

  return 0;
}



int startTemplate(unsigned int max_devs,char * settings)
{
    if (settings==0) { strcpy(readFromDir,""); }
       else
     {
       if (strlen(settings)==0)  { strcpy(readFromDir,""); } else
                                 { strncpy(readFromDir,settings,MAX_DIR_PATH);  }
     }
    return 1;
}
int getTemplateNumberOfDevices() { return 1; }

int stopTemplate()
{
   if (templateColorFrame!=0) { free(templateColorFrame); templateColorFrame=0; }
   if (templateDepthFrame!=0) { free(templateDepthFrame); templateDepthFrame=0; }
   return 1;
}

int createTemplateDevice(int devID,unsigned int width,unsigned int height,unsigned int framerate)
{
  if ( ( templateWIDTH < width ) &&  ( templateHEIGHT < height ) )
   {
        templateHEIGHT=height;
        templateWIDTH=width;
   }

  unsigned int widthInternal; unsigned int heightInternal;

  char file_name_test[1024];
  sprintf(file_name_test,"frames/%s/colorFrame_%u_%05u.pnm",readFromDir,devID,0);
  char * tmpColor = ReadPPM(file_name_test,&widthInternal,&heightInternal);
  if ( (widthInternal!=width) || (heightInternal!=height) )
   { fprintf(stderr,"Please note that the templateColor.pnm file has %ux%u resolution and the createTemplateDevice asked for %ux%u \n",widthInternal,heightInternal,width,height); }

  if (tmpColor!=0) { templateColorFrame=tmpColor; } else
  {
   // if templateColorFrame is zero the next function behaves like a malloc
   templateColorFrame= (char*) realloc(templateColorFrame,templateWIDTH*templateHEIGHT*3*sizeof(char));
  }


  sprintf(file_name_test,"frames/%s/depthFrame_%u_%05u.pnm",readFromDir,devID,0);
  short * tmpDepth = ReadPPMD(file_name_test,&widthInternal,&heightInternal);
  if ( (widthInternal!=width) || (heightInternal!=height) )
   { fprintf(stderr,"Please note that the templateColor.pnm file has %ux%u resolution and the createTemplateDevice asked for %ux%u \n",widthInternal,heightInternal,width,height); }

  if (tmpDepth!=0) { templateDepthFrame=tmpDepth; } else
  {
   // if templateDepthFrame is zero the next function behaves like a malloc
   templateDepthFrame= (short*) realloc(templateDepthFrame,templateWIDTH*templateHEIGHT*1*sizeof(short));
  }

  return ((templateColorFrame!=0)&&(templateDepthFrame!=0));
}



int destroyTemplateDevice(int devID)
{
  return 1;
}


int seekTemplateFrame(int devID,unsigned int seekFrame)
{
  cycle = seekFrame;
}

int snapTemplateFrames(int devID)
{
    //TODO HERE MAYBE LOAD NEW BUFFERS
    int found_frames = 0;

    unsigned int widthInternal=0; unsigned int heightInternal=0;
    char * file_name_test = (char* ) malloc(2048 * sizeof(char));
    if (file_name_test==0) { fprintf(stderr,"Could not snap frame , no space for string\n"); return 0; }

    sprintf(file_name_test,"frames/%s/colorFrame_%u_%05u.pnm",readFromDir,devID,cycle);
    //fprintf(stderr,"Snap color %s",file_name_test);
    if (FileExists(file_name_test))
     {
       if (templateColorFrame!=0) { free(templateColorFrame); }
       templateColorFrame = ReadPPM(file_name_test,&widthInternal,&heightInternal);
       ++found_frames;
     }

    sprintf(file_name_test,"frames/%s/depthFrame_%u_%05u.pnm",readFromDir,devID,cycle);
    //fprintf(stderr,"Snap depth %s",file_name_test);
    if (FileExists(file_name_test))
     {
      if (templateDepthFrame!=0) { free(templateDepthFrame); }
      templateDepthFrame = ReadPPMD(file_name_test,&widthInternal,&heightInternal);
      ++found_frames;
     }

  free(file_name_test);
  file_name_test=0;

  ++cycle;
  if (cycle>65534) { cycle=0; }
  if (found_frames!=2) { cycle = 0; }

  return 1;
}

//Color Frame getters
int getTemplateColorWidth(int devID)        { return templateWIDTH; }
int getTemplateColorHeight(int devID)       { return templateHEIGHT; }
int getTemplateColorDataSize(int devID)     { return templateHEIGHT*templateWIDTH * 3; }
int getTemplateColorChannels(int devID)     { return 3; }
int getTemplateColorBitsPerPixel(int devID) { return 8; }

// Frame Grabber should call this function for color frames
char * getTemplateColorPixels(int devID)    { return templateColorFrame; }

double getTemplateColorFocalLength(int devID) { return 120.0; }
double getTemplateColorPixelSize(int devID)   { return 0.1052; }


   //Depth Frame getters
int getTemplateDepthWidth(int devID)    { return templateWIDTH; }
int getTemplateDepthHeight(int devID)   { return templateHEIGHT; }
int getTemplateDepthDataSize(int devID) { return templateWIDTH*templateHEIGHT; }
int getTemplateDepthChannels(int devID)     { return 1; }
int getTemplateDepthBitsPerPixel(int devID) { return 16; }

// Frame Grabber should call this function for depth frames
char * getTemplateDepthPixels(int devID) { return (char *) templateDepthFrame; }

double getTemplateDepthFocalLength(int devID) { return 120.0; }
double getTemplateDepthPixelSize(int devID) { return 0.1052; }
