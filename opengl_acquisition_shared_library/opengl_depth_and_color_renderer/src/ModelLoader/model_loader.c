
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <math.h>

#include <GL/gl.h>
#include <GL/glx.h>    /* this includes the necessary X headers */

#include "model_loader.h"
#include "model_loader_hardcoded.h"
#include "model_loader_obj.h"
#include "model_loader_tri.h"
#include "../tools.h"

#define DISABLE_GL_CALL_LIST 0
#if DISABLE_GL_CALL_LIST
 #warning "Please note that glCallList is disabled and that has a really bad effect on graphics card performance"
#endif // DISABLE_GL_CALL_LIST

#define SPHERE_QUALITY 10 /*100 is good quality*/

#define PIE 3.14159265358979323846
#define degreeToRadOLD(deg) (deg)*(PIE/180)



#define USE_QUESTIONMARK_FOR_FAILED_LOADED_MODELS 1


#define NORMAL   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */

const GLfloat defaultAmbient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat defaultDiffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat defaultSpecular[]   = { 0.1f, 0.1f, 0.1f, 1.0f };
const GLfloat defaultShininess[] = { 5.0f };




int growModelList(struct ModelList * modelStorage,unsigned int framesToAdd)
{/*
  if (framesToAdd == 0) { return 0 ; }
  if (streamObj == 0) { fprintf(stderr,"Given an empty stream to grow \n"); return 0 ; }


  struct KeyFrame * new_frame;
  new_frame = (struct KeyFrame *) realloc( streamObj->frame, sizeof(struct KeyFrame)*( streamObj->MAX_numberOfFrames+framesToAdd ));

   if (new_frame == 0 )
    {
       fprintf(stderr,"Cannot add %u frames to our currently %u sized frame buffer\n",framesToAdd,streamObj->MAX_numberOfFrames);
       return 0;
    } else
     {
      //Clean up all new object types allocated
      void * clear_from_here  =  new_frame+streamObj->MAX_numberOfFrames;
      memset(clear_from_here,0,framesToAdd * sizeof(struct KeyFrame));
    }

   streamObj->MAX_numberOfFrames+=framesToAdd;
   streamObj->frame = new_frame ;*/
  return 1;
}


struct ModelList *  allocateModelList(unsigned int initialSpace)
{
  struct ModelList * newModelList = (struct ModelList * ) malloc(sizeof(struct ModelList));

  if (newModelList!=0)
  {
    newModelList->currentNumberOfModels = 0;
    newModelList->MAXNumberOfModels = 0;


    newModelList->models = (struct Model * ) malloc( initialSpace * sizeof(struct Model));

    if (newModelList->models!=0)
    {
      newModelList->MAXNumberOfModels = initialSpace;
      memset(newModelList->models,0,initialSpace * sizeof(struct Model));
    }
  }
  return newModelList;
}

int deallocateModelList(struct ModelList* modelStorage)
{
  fprintf(stderr,"Deallocation does not work yet todo : \n");

  if (modelStorage==0) { return 1; }
  if (modelStorage->models!=0)
    {
       unsigned int i=0;

       for (i=0; i<modelStorage->currentNumberOfModels; i++)
       {
           unloadModel(&modelStorage->models[i]);
       }
       free(modelStorage->models);
    }
  free(modelStorage);
  return 0;
}



int printModelList(struct ModelList* modelStorage)
{
  fprintf(stderr,"Model List ___________________________________________\n");
  unsigned int i=0;
  for (i=0; i<modelStorage->currentNumberOfModels; i++)
  {
    fprintf(stderr,"%03u | " ,i);
    fprintf(stderr,YELLOW "%s" NORMAL , modelTypeNames[ modelStorage->models[i].type ]);
    fprintf(stderr," %s - %u joints \n" ,
            modelStorage->models[i].pathOfModel ,
            modelStorage->models[i].numberOfBones);
  }
  fprintf(stderr,"______________________________________________________\n");
 return 1;
}

int modelHasASkinTransformation(struct Model * model,float* joints)
{
  if (model==0) {return 0; }
 return  ( (joints!=0) && (model->type==TRI_MODEL) );
}

unsigned int updateModelPosition(struct Model * model,float * position)
{
     // return 0;

      if (model==0) { return 0; }
      if (position==0) { return 0; }

      GLint viewport[4];
      GLdouble modelview[16];
      GLdouble projection[16];

      GLdouble winX, winY, winZ=0.0;

      #warning "It is inneficient to query all the tables for each position update..!"
      //fprintf(stderr,"This should not work it should only be called when the draw operation is ready , otherwise the matrices received here are irrelevant\n");
      glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
      glGetDoublev( GL_PROJECTION_MATRIX, projection );
      glGetIntegerv( GL_VIEWPORT, viewport );

      GLdouble posX=0.0,posY=0.0,posZ=0.0;

    if (model->type==OBJ_MODEL)
     {
      GLdouble posX=position[0],posY=position[1],posZ=position[2];
      struct OBJ_Model * modelOBJ = (struct OBJ_Model *) model->modelInternalData;

      posX=position[0] - modelOBJ->boundBox.min.x;
      posY=position[1] - modelOBJ->boundBox.min.y;
      posZ=position[2] - modelOBJ->boundBox.min.z;
      gluProject( posX * model->scaleX , posY* model->scaleY , posZ * model->scaleZ , modelview, projection, viewport, &winX, &winY, &winZ);
      model->bbox2D[0]=winX;
      model->bbox2D[1]=winY;

      posX=position[0] + modelOBJ->boundBox.max.x;
      posY=position[1] + modelOBJ->boundBox.max.y;
      posZ=position[2] + modelOBJ->boundBox.max.z;
      gluProject( posX* model->scaleX , posY* model->scaleY , posZ * model->scaleZ , modelview, projection, viewport, &winX, &winY, &winZ);
      model->bbox2D[2]=winX;
      model->bbox2D[3]=winY;

      return 1;
     }

 return 0;
}


unsigned int findModel(struct ModelList * modelStorage , char * directory,char * modelname ,int * found )
{
  *found = 0;
  char tmpPathOfModel[MAX_MODEL_PATHS]={0};
  snprintf(tmpPathOfModel,MAX_MODEL_PATHS,"%s/%s",directory,modelname);

  unsigned int i=0;
  for (i=0; i<modelStorage->currentNumberOfModels; i++)
  {
   if (&modelStorage->models[i]==0)
   {

   } else
   if (modelStorage->models[i].pathOfModel!=0)
   {
    if ( strcmp(tmpPathOfModel,modelStorage->models[i].pathOfModel)==0)
    {
      *found=1;
      fprintf(stderr,GREEN "model ( %s ) is already loaded , no need to reload it \n" NORMAL,tmpPathOfModel);
      return i;
    }
   }
  }

return 0;
}



unsigned int loadModel(struct ModelList* modelStorage , unsigned int whereToLoadModel , const char * directory,const char * modelname , const char * extension )
{
  fprintf(stderr,"loadModel ..  \n");
  if ( (directory==0) || (modelname==0) )
  {
    fprintf(stderr,RED "loadModel failing , no modelname given \n" NORMAL );
    return 0;
  }

  char fullPathToFile[MAX_MODEL_PATHS]={0};
  snprintf(fullPathToFile, MAX_MODEL_PATHS , "%s/%s.%s" , directory , modelname , extension);


  struct Model * mod =  &modelStorage->models[whereToLoadModel];
  if ( mod == 0 )  { fprintf(stderr,"Could not allocate enough space for model %s \n",modelname);  return 0; }
  memset(mod , 0 , sizeof(struct Model));


  snprintf(mod->pathOfModel,MAX_MODEL_PATHS,"%s",fullPathToFile);

  unsigned int unableToLoad=1;
  unsigned int checkForHardcodedReturn=0;
  unsigned int modType = isModelnameAHardcodedModel(modelname,&checkForHardcodedReturn);

  if (checkForHardcodedReturn)
  {
      mod->type = modType;
      mod->modelInternalData = 0;
      unableToLoad=0;
  } else
 if ( strcmp(extension,"tri") == 0 )
    {
      mod->type = TRI_MODEL;
      fprintf(stderr,YELLOW "loading tri model \n" NORMAL);

      struct TRI_Model * triModel = allocateModelTri();
      if (triModel !=0 )
         {
          if ( loadModelTri(fullPathToFile, triModel) )
            {
             mod->numberOfBones = triModel->header.numberOfBones;
             mod->modelInternalData=(void * ) triModel;
             unableToLoad=0;
             //colorCodeBones(triModel); //enable this to color code bones..
             fprintf(stderr,GREEN " success \n" NORMAL);
            } else
            { fprintf(stderr,RED " unable to load TRI model \n" NORMAL);
              freeModelTri(triModel);
            }
         } else { fprintf(stderr,RED " unable to allocate model \n" NORMAL); }
    } else
 if ( strcmp(extension,"ply") == 0 )
    {
      mod->type = OBJ_ASSIMP_MODEL;
      fprintf(stderr,RED "TODO : loading ply model \n" NORMAL);
    } else
  if ( strcmp(extension,"obj") == 0 )
    {
      mod->type = OBJ_MODEL;
      struct  OBJ_Model *  newObj = (struct  OBJ_Model * ) loadObj(directory,modelname,1);
      mod->modelInternalData = newObj;//(struct  OBJ_Model * ) loadObj(directory,modelname);
      if (mod->modelInternalData !=0 )
         {
             unableToLoad=0;
             //Populate 3D bounding box data
             mod->minX = newObj->minX; mod->minY = newObj->minY;  mod->minZ = newObj->minZ;
             mod->minX = newObj->maxX; mod->maxY = newObj->maxY;  mod->maxZ = newObj->maxZ;
             fprintf(stderr,"new obj : min %0.2f %0.2f %0.2f  max %0.2f %0.2f %0.2f \n",newObj->minX,newObj->minY,newObj->minZ,newObj->maxX,newObj->maxY,newObj->maxZ);
         }
    }


  if (unableToLoad)
    {
      fprintf(stderr,"Could not load object %s \n",modelname);
      fprintf(stderr,"Searched in directory %s \n",directory);
      fprintf(stderr,"Object %s was also not one of the hardcoded shapes\n",modelname);
      if (mod->modelInternalData==0 )
         {
          #if USE_QUESTIONMARK_FOR_FAILED_LOADED_MODELS
              mod->type = OBJ_QUESTION;
              mod->modelInternalData = 0;
              fprintf(stderr,RED "Failed to load object %s , will pretend it got loaded and use a fake object question mark instead\n" NORMAL,modelname);
          #else
            free(mod);
            return 0 ;
          #endif // USE_QUESTIONMARK_FOR_FAILED_LOADED_MODELS
         }
    }

  return 1;
}

void unloadModel(struct Model * mod)
{
   if (mod == 0 ) { return ; }

    switch ( mod->type )
    {
      case TRI_MODEL :
          freeModelTri( (struct TRI_Model *) mod->modelInternalData);
      case OBJ_MODEL :
          unloadObj( (struct  OBJ_Model * ) mod->modelInternalData);
      break;
    };
}


int loadModelToModelList(struct ModelList* modelStorage,const char * modelDirectory,const char * modelName , const char * modelExtension , unsigned int * whereModelWasLoaded)
{
  fprintf(stderr,"loadModelToModelList called .. \n");
  if (modelStorage==0) { return 0; }

  int foundAlreadyExistingModel=0;
  unsigned int modelLocation = findModel(modelStorage->models,modelDirectory,modelName, &foundAlreadyExistingModel);
 // fprintf(stderr,"findModel survived .. \n");

  if (!foundAlreadyExistingModel)
   { //If we can't find an already loaded version of the mesh we are looking for
     unsigned int whereToLoadModel=modelStorage->currentNumberOfModels;
     fprintf(stderr,"before LoadModel.. \n");
     if (loadModel(modelStorage,whereToLoadModel,modelDirectory,modelName,modelExtension))
      {
        fprintf(stderr,GREEN "Model %s is now loaded as model[%u] \n" NORMAL,modelName , whereToLoadModel );
        *whereModelWasLoaded=whereToLoadModel;
        ++modelStorage->currentNumberOfModels;
        return 1;
      } else
      { fprintf(stderr,RED "Failed loading new model %s ( %u ) \n" NORMAL,modelName, whereToLoadModel );        return 0; }
    } else
    {
     *whereModelWasLoaded=modelLocation;
     fprintf(stderr,GREEN "Model %s found already loaded \n" NORMAL,modelName);
     return 1;
    }
 return 0;
}

int drawModelAt(struct Model * mod,float x,float y,float z,float heading,float pitch,float roll)
{
 if (mod==0)
  {
    fprintf(stderr,"Cannot draw model at position %0.2f %0.2f %0.2f , it doesnt exist \n",x,y,z);
    return 0;
  }

  if (checkOpenGLError(__FILE__, __LINE__)) { fprintf(stderr,"drawModelAt called while on an erroneous state :(\n"); }
  glPushMatrix();
  //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

   if (mod->wireframe)  { glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); } else
                        { glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); }
  //glEnable(GL_NORMALIZE);
  /*If scale factors other than 1 are applied to the modelview matrix
    and lighting is enabled, lighting often appears wrong.
    In that case, enable automatic normalization of normals by
    calling glEnable with the argument GL_NORMALIZE.*/

  if (mod->nocull) { glDisable(GL_CULL_FACE); }
  glTranslatef(x,y,z);
  if ( roll!=0 )    { glRotatef(roll,0.0,0.0,1.0); }
  if ( heading!=0 ) { glRotatef(heading,0.0,1.0,0.0); }
  if ( pitch!=0 )   { glRotatef(pitch,1.0,0.0,0.0); }

  if ( (mod->scaleX!=1.0) ||
       (mod->scaleY!=1.0) ||
       (mod->scaleZ!=1.0) )
                       {
                         glScalef( mod->scaleX , mod->scaleY , mod->scaleZ );
                         if (checkOpenGLError(__FILE__, __LINE__)) { fprintf(stderr,"Could not scale :(\n"); }
                         //fprintf(stderr,"Scaling model by %f %f %f\n",mod->scaleX,mod->scaleY,mod->scaleZ);
                       }



  if (checkOpenGLError(__FILE__, __LINE__)) { fprintf(stderr,"drawModelAt error after specifying dimensions \n"); }

       // MAGIC NO COLOR VALUE :P MEANS NO COLOR SELECTION
      if (mod->nocolor!=0)  { glDisable(GL_COLOR_MATERIAL);   } else
      {//We Have a color to set
        //glEnable(GL_COLOR); <- not even an opengl command :P
        if (checkOpenGLError(__FILE__, __LINE__)) { fprintf(stderr,"drawModelAt error after enabling color\n"); }
        glEnable(GL_COLOR_MATERIAL);
        if (checkOpenGLError(__FILE__, __LINE__)) { fprintf(stderr,"drawModelAt error after enabling color material\n"); }

        GLenum faces=GL_FRONT;//GL_FRONT_AND_BACK;
        glMaterialfv(faces, GL_AMBIENT,    defaultAmbient);
        if (checkOpenGLError(__FILE__, __LINE__)) { fprintf(stderr,"drawModelAt error regarding color/materials\n"); }
        glMaterialfv(faces, GL_DIFFUSE,    defaultDiffuse);
        if (checkOpenGLError(__FILE__, __LINE__)) { fprintf(stderr,"drawModelAt error regarding color/materials\n"); }
        glMaterialfv(faces, GL_SPECULAR,   defaultSpecular);
        if (checkOpenGLError(__FILE__, __LINE__)) { fprintf(stderr,"drawModelAt error regarding color/materials\n"); }
        glMaterialfv(faces, GL_SHININESS,   defaultShininess);
        if (checkOpenGLError(__FILE__, __LINE__)) { fprintf(stderr,"drawModelAt error regarding color/materials\n"); }

        if (mod->transparency==0.0)
        {
          //fprintf(stderr,"SET RGB(%0.2f,%0.2f,%0.2f)\n",mod->colorR, mod->colorG, mod->colorB);
          glColor3f(mod->colorR,mod->colorG,mod->colorB);
        }
        else
        { glEnable(GL_BLEND);			// Turn Blending On
          glBlendFunc(GL_SRC_ALPHA, GL_ONE);
          //fprintf(stderr,"SET RGBA(%0.2f,%0.2f,%0.2f,%0.2f)\n",mod->colorR, mod->colorG, mod->colorB, mod->transparency);
          glColor4f(mod->colorR,mod->colorG,mod->colorB,mod->transparency);
        }
      }

  if (checkOpenGLError(__FILE__, __LINE__)) { fprintf(stderr,"drawModelAt error after specifying color/materials\n"); }

    //fprintf(stderr,"Drawing RGB(%0.2f,%0.2f,%0.2f) , Transparency %0.2f , ColorDisabled %u\n",mod->colorR, mod->colorG, mod->colorB, mod->transparency,mod->nocolor );

  //We have all the files
  float position[7]={0};
  updateModelPosition(mod,position);


      if (mod->type==TRI_MODEL)
      {
         //fprintf(stderr,"drawing TRI model\n");
         struct TRI_Model * tri = (struct TRI_Model *) mod->modelInternalData;
          doOGLGenericDrawCalllist(
                                     tri->vertices ,       tri->header.numberOfVertices ,
                                     tri->normal ,         tri->header.numberOfNormals ,
                                     tri->textureCoords ,  tri->header.numberOfTextureCoords ,
                                     tri->colors ,         tri->header.numberOfColors ,
                                     tri->indices ,        tri->header.numberOfIndices
                             );

      } else
      if (mod->type==OBJ_ASSIMP_MODEL )
      {
         fprintf(stderr,"TODO : draw assimp model here.. \n");
      } else
      if (mod->type==OBJ_MODEL)
      {
        //fprintf(stderr,"drawing OBJ model\n");
        if (mod->modelInternalData!=0)
         {
           if (mod->highlight)
           {
            struct  OBJ_Model *  drawOBJ = (struct  OBJ_Model * ) mod->modelInternalData;
            drawBoundingBox(0,0,0,drawOBJ->minX,drawOBJ->minY,drawOBJ->minZ,drawOBJ->maxX,drawOBJ->maxY,drawOBJ->maxZ);
           }

           //A model has been created , and it can be served
           GLuint objlist  =  getObjOGLList( ( struct OBJ_Model * ) mod->modelInternalData);
           if (checkOpenGLError(__FILE__, __LINE__)) { fprintf(stderr,"OpenGL error after getObjOGLList\n"); }

           if ( (objlist!=0) && (!DISABLE_GL_CALL_LIST) )
             { //We have compiled a list of the triangles for better performance
               glCallList(objlist);
               if (checkOpenGLError(__FILE__, __LINE__)) { fprintf(stderr,"drawModelAt error after drawing glCallList(%u)\n",objlist); }
             }  else
             { //Just feed the triangles to open gl one by one ( slow )
               drawOBJMesh( ( struct OBJ_Model * ) mod->modelInternalData);
               if (checkOpenGLError(__FILE__, __LINE__)) { fprintf(stderr,"drawModelAt error after drawing all the triangles\n"); }
             }
         } else
         { fprintf(stderr,"Could not draw unspecified model\n"); }
         glDisable(GL_TEXTURE_2D); //TODO : <-- change drawOBJMesh , Calllist so that they dont leave textures on! :P

      } else
      if (drawHardcodedModel(mod->type))
      {
        //Success drawing hardcoded model
        //fprintf(stderr,"drawing hardcoded model\n");
      } else
      {
         fprintf(stderr, "Cannot draw model , unknown type %u\n",mod->type );
      }


  if (checkOpenGLError(__FILE__, __LINE__)) { fprintf(stderr,"drawModelAt error after drawing geometry\n"); }

  if (mod->transparency!=0.0) {glDisable(GL_BLEND);  }
  if (mod->nocolor) {glEnable(GL_COLOR); glEnable(GL_COLOR_MATERIAL); }
  if (mod->nocull)  {glEnable(GL_CULL_FACE); }

  glTranslatef(-x,-y,-z);
  //glDisable(GL_NORMALIZE);
  glPopMatrix();
 return 1;
}

int drawModel(struct Model * mod)
{
    if (mod == 0) { fprintf(stderr,"Cannot draw model , it doesnt exist \n"); return 0; } //If mod = 0 accesing the fields below will lead in crashing..
    return drawModelAt(mod,mod->x,mod->y,mod->z,mod->heading,mod->pitch,mod->roll);
}

int addToModelCoordinates(struct Model * mod,float x,float y,float z,float heading,float pitch,float roll)
{
  if (mod==0) { return 0; }
  mod->x+=x; mod->y+=y; mod->z+=z;

  mod->heading+=heading; mod->pitch+=pitch; mod->roll+=roll;
  fprintf(stderr,"Model(%0.2f %0.2f %0.2f - %0.4f %0.4f %0.4f)\n",mod->x,mod->y,mod->z,mod->heading,mod->pitch,mod->roll);
  return 1;
}

int addToModelCoordinatesNoSTACK(struct Model * mod,float *x,float *y,float *z,float *heading,float *pitch,float *roll)
{
  if (mod==0) { return 0; }
  mod->x+=*x; mod->y+=*y; mod->z+=*z;

  mod->heading+=*heading; mod->pitch+=*pitch; mod->roll+=*roll;
  fprintf(stderr,"Model(%0.2f %0.2f %0.2f - %0.4f %0.4f %0.4f)\n",mod->x,mod->y,mod->z,mod->heading,mod->pitch,mod->roll);
  return 1;
}

int setModelCoordinates(struct Model * mod,float x,float y,float z,float heading,float pitch,float roll)
{
  if (mod==0) { return 0; }
  fprintf(stderr,"Model SET Got params(%0.2f %0.2f %0.2f - %0.4f %0.4f %0.4f)\n",x,y,z,heading,pitch,roll);

  mod->x=x; mod->y=y; mod->z=z;

  mod->heading=heading; mod->pitch=pitch; mod->roll=roll;
  fprintf(stderr,"Model SET (%0.2f %0.2f %0.2f - %0.4f %0.4f %0.4f)\n",mod->x,mod->y,mod->z,mod->heading,mod->pitch,mod->roll);
  return 1;
}

int setModelCoordinatesNoSTACK(struct Model * mod,float * x,float* y,float *z,float *heading,float *pitch,float* roll)
{
  if (mod==0) { return 0; }
  //fprintf(stderr,"Model SET NoSTACK Got params(%0.2f %0.2f %0.2f - %0.4f %0.4f %0.4f)\n",*x,*y,*z,*heading,*pitch,*roll);

  mod->x=*x; mod->y=*y; mod->z=*z;

  mod->heading=*heading; mod->pitch=*pitch; mod->roll=*roll;
  //fprintf(stderr,"Model SET NoSTACK (%0.2f %0.2f %0.2f - %0.4f %0.4f %0.4f)\n",mod->x,mod->y,mod->z,mod->heading,mod->pitch,mod->roll);
  return 1;
}

int setModelColor(struct Model * mod,float *R,float *G,float *B,float *transparency,unsigned char * noColor)
{
 if (mod==0) { return 0; }

 //fprintf(stderr,"Seting color to  %0.2f %0.2f %0.2f trans %0.2f \n",*R,*G,*B,*transparency);
 mod->colorR = *R;
 mod->colorG = *G;
 mod->colorB = *B;
 mod->transparency = *transparency;
 mod->nocolor = *noColor;
 //fprintf(stderr,"Seting color to  %0.2f %0.2f %0.2f trans %0.2f \n",mod->colorR,mod->colorG,mod->colorB,mod->transparency);
 return 1;
}

int getModelBBox(struct Model *mod , float * minX,  float * minY , float * minZ , float * maxX , float * maxY , float * maxZ)
{
 struct OBJ_Model * objMod = (struct OBJ_Model * ) mod->modelInternalData;
 * minX = objMod->boundBox.min.x;
 * minY = objMod->boundBox.min.y;
 * minZ = objMod->boundBox.min.z;
 * maxX = objMod->boundBox.max.x;
 * maxY = objMod->boundBox.max.y;
 * maxZ = objMod->boundBox.max.z;
 return 1;
}

int getModel3dSize(struct Model *mod , float * sizeX , float * sizeY , float * sizeZ )
{
  float minX,minY,minZ,maxX,maxY,maxZ;
  getModelBBox(mod,&minX,&minY,&minZ,&maxX,&maxY,&maxZ);

  *sizeX = maxX - minX;
  *sizeY = maxY - minY;
  *sizeZ = maxZ - minZ;

 return 1;
}


int getModelListBoneNumber(struct ModelList * modelStorage,unsigned int modelNumber)
{
  fprintf(stderr,"getModelListBoneNumber(modelStorage,modelNumber=%u)\n",modelNumber);
  if (modelNumber>=modelStorage->currentNumberOfModels)
  {
    fprintf(stderr," modelNumber is out of bounds\n");
    return 0;
  }

  return modelStorage->models[modelNumber].numberOfBones;
}

int getModelBoneIDFromBoneName(struct Model *mod,char * boneName,int * found)
{
 fprintf(stderr,"Searching model %s for a bone named %s \n",mod->pathOfModel , boneName);
 *found=0;
 if (mod->type==TRI_MODEL)
 {
  struct TRI_Model * triM = (struct TRI_Model * ) mod->modelInternalData;
  if (triM!=0)
   {
     unsigned int i=0;
     unsigned int numberOfBones=mod->numberOfBones;

     for (i=0; i<numberOfBones; i++)
     {
       if (strcmp( triM->bones[i].boneName , boneName) == 0 )
       {
         fprintf(stderr,"found it , it is joint # %u \n" , i);
         *found=1;
         return i;
       }
     }
   }
 }
 return 0;
}
