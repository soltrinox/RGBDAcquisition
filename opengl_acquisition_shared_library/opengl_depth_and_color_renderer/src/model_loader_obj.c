#include "model_loader_obj.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "TextureLoader/texture_loader.h"

#define reallocationStep 500



void calcFaceNormal(Normal *nrm,Vertex v1,Vertex v2,Vertex v3,int normalized)
{
  /* Calculate cross product of vectors to calculate face normal (normalized) */
  Normal ret;
  float length;
  Vertex v4,v5;

  v4.x = v2.x-v1.x;
  v4.y = v2.y-v1.y;
  v4.z = v2.z-v1.z;
  v5.x = v3.x-v1.x;
  v5.y = v3.y-v1.y;
  v5.z = v3.z-v1.z;

  ret.n1 = v4.y*v5.z - v4.z*v5.y;
  ret.n2 = v4.z*v5.x - v4.x*v5.z;
  ret.n3 = v4.x*v5.y - v4.y*v5.x;

  if(normalized==0)
  {
	  length = (float)sqrt((ret.n1*ret.n1)+(ret.n2*ret.n2)+(ret.n3*ret.n3));
	  if(length==0.0f)
		  length=1.0f;
	  ret.n1=(GLfloat)(ret.n1/(GLfloat)length);
	  ret.n2=(GLfloat)(ret.n2/(GLfloat)length);
	  ret.n3=(GLfloat)(ret.n3/(GLfloat)length);
  }

  nrm->n1=ret.n1;
  nrm->n2=ret.n2;
  nrm->n3=ret.n3;
}









int VerticesComp(Vertex v1, Vertex v2)
{
	 return (v1.x==v2.x && v1.y==v2.y && v1.z==v2.z);
}








void Transpose( MATRIX a, MATRIX b)
{
	int i, j;
	for(i = 0; i<4; i++){
		for(j=0; j<4; j++){
			b[j+i*4] = a[i+j*4];
		}
	}

}





float findMax(struct OBJ_Model * obj)
{
    	/* Compares and finds the maximum dimension of bounding box */
	GLfloat w,h,d,xmin,xmax,ymin,ymax,zmin,zmax;


	xmin=obj->boundBox.min.x;
	xmax=obj->boundBox.max.x;
	ymin=obj->boundBox.min.y;
	ymax=obj->boundBox.max.y;
	zmin=obj->boundBox.min.z;
	zmax=obj->boundBox.max.z;
		if(obj->boundBox.min.x<xmin) xmin=obj->boundBox.min.x;
		if(obj->boundBox.min.y<ymin) ymin=obj->boundBox.min.y;
		if(obj->boundBox.min.z<zmin) zmin=obj->boundBox.min.z;
		if(obj->boundBox.max.x>xmax) xmax=obj->boundBox.max.x;
		if(obj->boundBox.max.y>ymax) ymax=obj->boundBox.max.y;
		if(obj->boundBox.max.z>zmax) zmax=obj->boundBox.max.z;
	w = xmax-xmin;
    h = ymax-ymin;
    d = zmax-zmin;
	if(w>h)
	  {
		if(w>d) return w;
		else    return d;
	  }
	  else
	  {
		if(h>d) return h;
		else return d;
	  }
   return 0;
}

unsigned int FindMaterial(struct OBJ_Model * obj,char *name)
{
  /* Find material id NAME in MODEL */
  GLuint i;

  for (i = 0; i<obj->numMaterials; i++)
  {
    if (!strcmp(obj->matList[i].name, name)) { return i; }

  }

  /* didn't find the name, so set it as the default material */
  printf("FindMaterial():  TODO NOT RETURN 0 can't find material \"%s\".\n", name);
  return 0;
}

signed int FindGroup(struct OBJ_Model * obj, char *name)
{
  /* Find group id NAME in MODEL */
  GLuint i;
  for(i=0; i<obj->numGroups; i++)
  {
    if (strcmp(name, obj->groups[i].name)==0)
	{
		return i;
	}
  }

  return -1;
}



unsigned int AddGroup(struct OBJ_Model * obj, char *name)
{
  /* Add group NAME to the MODEL structure */
  int find;
  find = FindGroup( obj, name);
  if (find==-1) {
                  strcpy(obj->groups[obj->numGroups].name,name);
                  obj->groups[obj->numGroups].numFaces = 0;
	              obj->groups[obj->numGroups].hasNormals = 0;
	              obj->groups[obj->numGroups].hasTex = 0;
	              obj->numGroups++;
	              return obj->numGroups-1;
                }
  else
	  return find;
}

void AddFacetoG(Group *g,long unsigned int fc)
{
	/* Add face FC to group's G facelist */
	if(g->numFaces==0)
	{
		g->faceList=(long unsigned int*)malloc(sizeof(long unsigned int)*reallocationStep);
		g->malloced=reallocationStep;
	}
	if(g->malloced<=g->numFaces)
	{
		fprintf(stderr,"New Reallocation code..\n");
	    g->malloced+=reallocationStep;
	    g->faceList=(long unsigned int*) realloc(g->faceList, sizeof(long unsigned int)*(g->malloced));
		fprintf(stderr,"New Reallocation code survived..\n");
	}
	g->faceList[g->numFaces]=fc;
	g->numFaces++;
}




int loadMTL(struct OBJ_Model * obj,char *filename)
{

  FILE *file;
  char buf[128];
  char buf1[128];
  char fname[60];
  GLuint tex_id;
  GLuint mat_num;
  float   r,g,b;
 int i;

  strcpy(fname,filename);


  mat_num = 1;

  if((file=fopen(fname,"r"))==0) { printf("File %s is corrupt or does not exist.\n",filename); return 0; }

  rewind(file);
	//1st pass - count materials

	while(!feof(file))
	{
		buf[0] = 0;// ? NULL;
		fscanf(file,"%s", buf);

		if (!strcmp(buf,"newmtl")) { mat_num ++; } else
		                           { fgets(buf, sizeof(buf), file); }
	}

	if (mat_num == 0) mat_num = 1;
	obj->matList=(Material*)malloc(sizeof(Material)*(mat_num));
    if (obj->matList==0) { fprintf(stderr,"Could not make enough space for %u materials \n",mat_num); }

    obj->numMaterials = mat_num;
  /* set the default material */

  for(i = 0; i<mat_num; i++)
  {
	obj->matList[i].shine = 0;
	obj->matList[i].diffuse[0] = 0.8;
	obj->matList[i].diffuse[1] = 0.8;
	obj->matList[i].diffuse[2] = 0.8;
	obj->matList[i].diffuse[3] = 1.0;
	obj->matList[i].ambient[0] = 0.2;
	obj->matList[i].ambient[1] = 0.2;
	obj->matList[i].ambient[2] = 0.2;
	obj->matList[i].ambient[3] = 1.0;
	obj->matList[i].specular[0] = 0.0;
	obj->matList[i].specular[1] = 0.0;
	obj->matList[i].specular[2] = 0.0;
	obj->matList[i].specular[3] = 1.0;
	obj->matList[i].ldText = 0;
	obj->matList[i].hasTex = 0;
  }//give default values
  strcpy(obj->matList[0].name,"default");
  strcpy(obj->matList[0].texture,"");

  rewind(file);

  mat_num = 0;

  while(!feof(file))
	{
		buf[0] = 0;//? NULL;
		fscanf(file,"%s", buf);

		if (!strcmp(buf,"newmtl"))
		{
			fscanf(file,"%s",buf1);
			mat_num ++;

			strcpy(obj->matList[mat_num].name, buf1);
			obj->matList[mat_num].hasTex =  0;
		} else
		if (!strcmp(buf,"Ka"))
		{
			obj->matList[mat_num].ambient[3] =  1;
			fscanf(file,"%f %f %f",&obj->matList[mat_num].ambient[0], &obj->matList[mat_num].ambient[1],&obj->matList[mat_num].ambient[2]);
		} else
		if (!strcmp(buf,"Kd"))
		{
			fscanf(file,"%f %f %f",&r,&g,&b);
			obj->matList[mat_num].diffuse[0] =  r;
			obj->matList[mat_num].diffuse[1] =  g;
			obj->matList[mat_num].diffuse[2] =  b;
			obj->matList[mat_num].diffuse[3] =  1;
		} else
		if (!strcmp(buf,"Ks"))
		{
			fscanf(file,"%f %f %f",&r,&g,&b);
			obj->matList[mat_num].specular[0] =  r;
			obj->matList[mat_num].specular[1] =  g;
			obj->matList[mat_num].specular[2] =  b;
			obj->matList[mat_num].specular[3] =  1;
		} else
		if (!strcmp(buf,"Ns"))
		{
			fscanf(file,"%f",&r);
			obj->matList[mat_num].shine =  r;
		} else
		if(!strcmp(buf, "map_Kd"))
		{
			fscanf(file,"%s",obj->matList[mat_num].texture);
			obj->matList[mat_num].hasTex =  1;
		    obj->matList[mat_num].ldText = loadTexture(GL_LINEAR, obj->matList[mat_num].texture);
			printf("%d \t   \n\n", obj->matList[mat_num].ldText);
			printf("%s \t   \n\n", obj->matList[mat_num].texture);
		} else
		if (!strcmp(buf,"#"))
			fgets(buf,100,file);

	}

	if (file) fclose(file);
	return 1;
}


int calculateBoundingBox(struct OBJ_Model * obj)
{
  /* Scan all model vertices to find the maximum and minimum coordinates of each dimension */
  long unsigned int i;

	  obj->boundBox.min.x=obj->vertexList[1].x;
	  obj->boundBox.min.y=obj->vertexList[1].y;
	  obj->boundBox.min.z=obj->vertexList[1].z;
	  obj->boundBox.max.x=obj->vertexList[1].x;
	  obj->boundBox.max.y=obj->vertexList[1].y;
	  obj->boundBox.max.z=obj->vertexList[1].z;
	  for(i=1; i<=obj->numVertices; i++)
	  {
		if(obj->vertexList[i].x < obj->boundBox.min.x) obj->boundBox.min.x=obj->vertexList[i].x;
		if(obj->vertexList[i].y < obj->boundBox.min.y) obj->boundBox.min.y=obj->vertexList[i].y;
		if(obj->vertexList[i].z < obj->boundBox.min.z) obj->boundBox.min.z=obj->vertexList[i].z;
		if(obj->vertexList[i].x > obj->boundBox.max.x) obj->boundBox.max.x=obj->vertexList[i].x;
		if(obj->vertexList[i].y > obj->boundBox.max.y) obj->boundBox.max.y=obj->vertexList[i].y;
		if(obj->vertexList[i].z > obj->boundBox.max.z) obj->boundBox.max.z=obj->vertexList[i].z;
	  }
	  obj->center[0] = (float)((obj->boundBox.max.x+obj->boundBox.min.x)/2.0);
	  obj->center[1] = (float)((obj->boundBox.max.y+obj->boundBox.min.y)/2.0);
	  obj->center[2] = (float)((obj->boundBox.max.z+obj->boundBox.min.z)/2.0);

 return 1;
}


int prepareObject(struct OBJ_Model * obj)
{
	long unsigned int i;
	Normal tmpnrm;
	for(i=0;i<obj->numFaces;i++)
	{
		calcFaceNormal(&tmpnrm,obj->vertexList[obj->faceList[i].v[0]],obj->vertexList[obj->faceList[i].v[1]],obj->vertexList[obj->faceList[i].v[2]],0);
		obj->faceList[i].fc_normal.n1=tmpnrm.n1;
		obj->faceList[i].fc_normal.n2=tmpnrm.n2;
		obj->faceList[i].fc_normal.n3=tmpnrm.n3;
	}
  return 1;
}


int readOBJ(struct OBJ_Model * obj)
{
  if (obj->filename == 0 ) { fprintf(stderr,"readOBJ called with a null filename , cannot continue \n"); return 0; }
  /* Read the .obj model from file FILENAME */
  /* All faces are converted to be triangles */
  FILE *file=0;
  char buf[128];
  char buf1[128];
  long unsigned int    numvertices;		/* number of vertices in model */
  long unsigned int    numnormals;                 /* number of normals in model */
  long unsigned int    numtexs;                 /* number of normals in texture coordintaes */
  long unsigned int    numfaces;			/* number of faces in model */
  long unsigned int    numgroups;			/* number of groups in model */
  GLuint cur_group,material, mat;
  long unsigned int v,n,t,i;
  int grp;

  fprintf(stderr,"TODO : proper string allocation here for filename %s \n",obj->filename);
  char fname[MAX_MODEL_PATHS]={0};
  strncpy(fname,obj->filename,MAX_MODEL_PATHS);

  file=fopen(fname,"r");
  if(file==0) { fprintf(stderr,"Could not open file %s for reading Object\n",fname); return 0;  }
 // strcpy(name,filename);
  strcpy(obj->matLib,"");
  //Group Pass
  rewind(file);
  numgroups = 1;
  while(fscanf(file, "%s", buf) != EOF)
  {
	  if(buf[0]=='g')
		  numgroups++;
	  else
		  fgets(buf, sizeof(buf), file); // eat up rest of line
  }
  if(numgroups==0) { numgroups=1; }
  obj->groups = (Group*) malloc(sizeof(Group)* numgroups);
  if (obj->groups == 0) { fprintf(stderr,"Could not make enough space for %u groups \n",numgroups); return 0; }
  obj->numGroups = 0;
  obj->numFaces =0;

  // 1st Pass
  rewind(file);
  numtexs = 0;
  numvertices = 0;
  numnormals = 0;
  numfaces = 0;
  cur_group = AddGroup(obj,"default");
  obj->groups[0].material=0;
  while(fscanf(file, "%s", buf) != EOF)
  {

	  if(!strcmp(buf, "mtllib"))
	  {
		  fscanf(file, "%s", buf1);
		  strcpy(obj->matLib, buf1);
		  loadMTL(obj,buf1);
		  printf("loadmtl %s survived\n", obj->matLib);
	  }
    switch(buf[0]) {
    case '#':	fgets(buf, sizeof(buf), file);	 break;		// comment   eat up rest of line

    // v, vn, vt
    case 'v':
               switch(buf[1])
               {
                  case '\0': // vertex  eat up rest of line
	                         fgets(buf, sizeof(buf), file);
	                         numvertices++;
	              break;
                  case 'n': // normal  eat up rest of line
	                         fgets(buf, sizeof(buf), file);
	                         numnormals++;
                  break;
                  case 't':	//texture coordinate  eat up rest of line
	                         fgets(buf, sizeof(buf), file);
	                         numtexs ++;
	              break;
                  default:
	                         fprintf(stderr,"Unexpected characters  ( \"%s\" ) while waiting for v, vn ,vt .\n", buf);
	                         return 0;
	              break;
                }
     break;

     case 'm': fgets(buf, sizeof(buf), file);  break;
     case 'u': fgets(buf, sizeof(buf), file);  break;// eat up rest of line

     case 'g': //group eat up rest of line
               fgets(buf, sizeof(buf), file);
               sscanf(buf, "%s", buf);
	           cur_group = AddGroup(obj,buf);
     break;

     case 'f':	// face
                v =0; n = 0; t = 0;
                fscanf(file, "%s", buf); // can be one of %d, %d//%d, %d/%d, %d/%d/%d
                if (strstr(buf, "//"))
                  { //        v//n
	                sscanf(buf, "%d//%d", &v, &n);
	                fscanf(file, "%d//%d", &v, &n);
	                fscanf(file, "%d//%d", &v, &n);
	                obj->numFaces++;
	                while(fscanf(file, "%d//%d", &v, &n) > 0) { obj->numFaces++; }
	              } else
                if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3)
                  { //        v/t/n
	                fscanf(file, "%d/%d/%d", &v, &t, &n);
	                fscanf(file, "%d/%d/%d", &v, &t, &n);
	                obj->numFaces++;
	                while(fscanf(file, "%d/%d/%d", &v, &t, &n) > 0) { obj->numFaces++; }
	               } else
                if (sscanf(buf, "%d/%d", &v, &t) == 2)
                  { //        v/t
	                fscanf(file, "%d/%d", &v, &t);
	                fscanf(file, "%d/%d", &v, &t);
	                obj->numFaces++;
	                while(fscanf(file, "%d/%d", &v, &t) > 0) { obj->numFaces++; }
	              } else
	              { //        v
	                fscanf(file, "%d", &v);
	                fscanf(file, "%d", &v);
	                obj->numFaces++;
	                while(fscanf(file, "%d", &v) > 0)  { obj->numFaces++; }
	              }
      break; //end of face case

    default: fgets(buf, sizeof(buf), file); break; // eat up rest of line


    }
  }

  // set the stats in the model structure
  obj->numVertices  = numvertices;
  obj->numNormals   = numnormals;
  obj->numTexs = numtexs;

  printf("Vertices : %ld\n",obj->numVertices);
  printf("Normals  : %ld\n",obj->numNormals);
  printf("Faces    : %ld\n",obj->numFaces);
  printf("Groups   : %ld\n",obj->numGroups);
  printf("Texes   : %ld\n",obj->numTexs);
  for(i=0; i<obj->numGroups; i++)
  {
	  obj->groups[i].numFaces=0;
	  printf("%d\n", obj->groups[i].hasNormals);//0, 0
  }

  // Allocating memory
  fprintf(stderr,"Allocating memory for faces\n");
  obj->faceList = (Face*) malloc(sizeof(Face)*(obj->numFaces+2));
  if (obj->faceList == 0) { fprintf(stderr,"Could not allocate enough memory for face struct\n"); }
  memset (obj->faceList,0,sizeof(Face)*(obj->numFaces+1));

  fprintf(stderr,"Allocating memory for vertices\n");
  obj->vertexList = (Vertex*) malloc(sizeof(Vertex)*(obj->numVertices+2));
  if (obj->vertexList == 0) { fprintf(stderr,"Could not allocate enough memory for vertex struct\n"); }
  memset (obj->vertexList,0,sizeof(Vertex)*(obj->numVertices+1));

  if(obj->numNormals!=0)
  {
    fprintf(stderr,"Allocating memory for normals\n");
    obj->normalList=(Normal*)malloc(sizeof(Normal)*(obj->numNormals+2));
    if (obj->normalList == 0) { fprintf(stderr,"Could not allocate enough memory for normal struct\n"); }
    memset (obj->normalList,0,sizeof(Normal)*(obj->numNormals+1));
  }

  if(obj->numTexs!=0)
  {
    fprintf(stderr,"Allocating memory for textures\n");
    obj->texList=(TexCoords*)malloc(sizeof(TexCoords)*(obj->numTexs+2));
    if (obj->texList == 0) { fprintf(stderr,"Could not allocate enough memory for texture struct\n"); }
    memset (obj->texList,0,sizeof(TexCoords)*(obj->numTexs+1));
  }


   fprintf(stderr,"Second pass of the file\n");

  // Second Pass
  rewind(file);

  obj->numVertices = 1;
  obj->numNormals = 1;
  obj->numTexs = 1;
  obj->numFaces = 0;
  material = 0;
  grp = 0;

  while(!feof(file))
  {
    fscanf(file, "%s", buf);

	if(!strcmp(buf, "usemtl"))
	{
		  fscanf(file, "%s", buf1);
		  mat = FindMaterial( obj, buf1);
		  obj->groups[grp].material = mat;
		  strcpy(obj->matLib, buf1);
		  printf("loadmtl %s\n", obj->matLib);
    }
	switch(buf[0])
	{
      case '#':	fgets(buf, sizeof(buf), file);	break;		// comment  eat up rest of line

      case 'v':	 // v, vn, vt
                  switch(buf[1])
                  {
                       case '\0': //  vertex
	                              fscanf(file, "%f %f %f",
	                              &obj->vertexList[obj->numVertices].x,
	                              &obj->vertexList[obj->numVertices].y,
	                              &obj->vertexList[obj->numVertices].z);
	                              obj->numVertices++;
	                   break;
                       case 'n': // normal
	                             fscanf(file, "%f %f %f",
	                             &obj->normalList[obj->numNormals].n1,
	                             &obj->normalList[obj->numNormals].n2,
	                             &obj->normalList[obj->numNormals].n3);
	                             obj->numNormals++;
	                   break;

	                   case 't': // normal
	                             fscanf(file, "%f %f",
	                             &obj->texList[obj->numTexs].u,
	                             &obj->texList[obj->numTexs].v);
	                             obj->numTexs++;
	                   break;
                  }
      break;

      case 'u': fgets(buf, sizeof(buf), file);  break; // eat up rest of line

      case 'g': // group eat up rest of line
                fgets(buf, sizeof(buf), file);
	            sscanf(buf, "%s", buf);
	            grp = FindGroup(obj,buf);
                obj->groups[grp].material = material;
	  break;

      case 'f': //face
                v = 0;  n = 0; t = 0;
                fscanf(file, "%s", buf);
                // can be one of %d, %d//%d, %d/%d, %d/%d/%d
                if (strstr(buf, "//"))
                { //  v//n
	              sscanf(buf, "%d//%d", &v, &n);
	              obj->faceList[obj->numFaces].v[0] = v;
	              obj->faceList[obj->numFaces].n[0] = n;
	              fscanf(file, "%d//%d", &v, &n);
	              obj->faceList[obj->numFaces].v[1] = v;
	              obj->faceList[obj->numFaces].n[1] = n;
	              fscanf(file, "%d//%d", &v, &n);
	              obj->faceList[obj->numFaces].v[2] = v;
	              obj->faceList[obj->numFaces].n[2] = n;
	              AddFacetoG(&obj->groups[grp],obj->numFaces);
	              obj->numFaces++;
	              obj->groups[grp].hasNormals = 1;
	              obj->groups[grp].hasTex = 0;
	              while(fscanf(file, "%d//%d", &v, &n) > 0)
	               {
	                 obj->faceList[obj->numFaces].v[0] = obj->faceList[obj->numFaces-1].v[0];
	                 obj->faceList[obj->numFaces].n[0] = obj->faceList[obj->numFaces-1].n[0];
	                 obj->faceList[obj->numFaces].v[1] = obj->faceList[obj->numFaces-1].v[2];
	                 obj->faceList[obj->numFaces].n[1] = obj->faceList[obj->numFaces-1].n[2];
	                 obj->faceList[obj->numFaces].v[2] = v;
	                 obj->faceList[obj->numFaces].n[2] = n;
	                 AddFacetoG(&obj->groups[grp],obj->numFaces);
	                 obj->numFaces++;
	               }
	             } else
	             if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3)
	             { // v/t/n
	                obj->faceList[obj->numFaces].v[0] = v;
	                obj->faceList[obj->numFaces].n[0] = n;
	                obj->faceList[obj->numFaces].t[0] = t;
	                fscanf(file, "%d/%d/%d", &v, &t, &n);
	                obj->faceList[obj->numFaces].v[1] = v;
	                obj->faceList[obj->numFaces].n[1] = n;
	                obj->faceList[obj->numFaces].t[1] = t;
	                fscanf(file, "%d/%d/%d", &v, &t, &n);
	                obj->faceList[obj->numFaces].v[2] = v;
	                obj->faceList[obj->numFaces].n[2] = n;
	                obj->faceList[obj->numFaces].t[2] = t;
	                AddFacetoG(&obj->groups[grp],obj->numFaces);
	                obj->numFaces++;
	                obj->groups[grp].hasNormals = 1;
	                obj->groups[grp].hasTex = 1;

	                while(fscanf(file, "%d/%d/%d", &v, &t, &n) > 0)
	                  {
	                    obj->faceList[obj->numFaces].v[0] = obj->faceList[obj->numFaces-1].v[0];
	                    obj->faceList[obj->numFaces].n[0] = obj->faceList[obj->numFaces-1].n[0];
	                    obj->faceList[obj->numFaces].t[0] = obj->faceList[obj->numFaces-1].t[0];
	                    obj->faceList[obj->numFaces].v[1] = obj->faceList[obj->numFaces-1].v[2];
	                    obj->faceList[obj->numFaces].n[1] = obj->faceList[obj->numFaces-1].n[2];
	                    obj->faceList[obj->numFaces].t[1] = obj->faceList[obj->numFaces-1].t[2];
	                    obj->faceList[obj->numFaces].v[2] = v;
	                    obj->faceList[obj->numFaces].n[2] = n;
	                    obj->faceList[obj->numFaces].t[2] = t;
	                    AddFacetoG(&obj->groups[grp],obj->numFaces);
	                    obj->numFaces++;
	                  }
                } else
                if (sscanf(buf, "%d/%d", &v, &t) == 2)
                { // v/t
		          obj->groups[grp].hasTex = 1;
		          obj->faceList[obj->numFaces].v[0] = v;
		          obj->faceList[obj->numFaces].t[0] = t;
		          fscanf(file, "%d/%d", &v, &t);
		          obj->faceList[obj->numFaces].v[1] = v;
		          obj->faceList[obj->numFaces].t[1] = t;
		          fscanf(file, "%d/%d", &v, &t);
		          obj->faceList[obj->numFaces].v[2] = v;
		          obj->faceList[obj->numFaces].t[2] = t;
		          AddFacetoG(&obj->groups[grp],obj->numFaces);
		          obj->numFaces++;
		          while(fscanf(file, "%d/%d", &v, &t) > 0)
		          {
	  	           obj->faceList[obj->numFaces].v[0] = obj->faceList[obj->numFaces-1].v[0];
	  	           obj->faceList[obj->numFaces].t[0] = obj->faceList[obj->numFaces-1].t[0];
	  	           obj->faceList[obj->numFaces].v[1] = obj->faceList[obj->numFaces-1].v[2];
	  	           obj->faceList[obj->numFaces].t[1] = obj->faceList[obj->numFaces-1].t[2];
	  	           obj->faceList[obj->numFaces].v[2] = v;
	  	           obj->faceList[obj->numFaces].t[2] = t;
	  	           AddFacetoG(&obj->groups[grp],obj->numFaces);
	  	           obj->numFaces++;
		          }//while
               } else
               { // v
	             sscanf(buf, "%d", &v);
	             obj->faceList[obj->numFaces].v[0] = v;
	             fscanf(file, "%d", &v);
	             obj->faceList[obj->numFaces].v[1] = v;
	             fscanf(file, "%d", &v);
	             obj->faceList[obj->numFaces].v[2] = v;
	             obj->groups[grp].hasNormals = 0;
	             obj->groups[grp].hasTex = 0;
	             AddFacetoG(&obj->groups[grp],obj->numFaces);
	             obj->numFaces++;
	             while(fscanf(file, "%d", &v) == 1)
	              {
	               obj->faceList[obj->numFaces].v[0] = obj->faceList[obj->numFaces-1].v[0];
	               obj->faceList[obj->numFaces].v[1] = obj->faceList[obj->numFaces-1].v[2];
	               obj->faceList[obj->numFaces].v[2] = v;
	               AddFacetoG(&obj->groups[grp],obj->numFaces);
	               obj->numFaces++;
	              }
                }
      break;

     default: fgets(buf, sizeof(buf), file); break; // eat up rest of line
    }
  }
  fclose(file);
  printf("Model has %ld faces\n",obj->numFaces);
  for(i=0; i<obj->numGroups; i++)
  {
	 // fprintf(stderr,"Group %s has %ld faces and material %s, \t \n",obj->groups[i].name,obj->groups[i].numFaces,obj->matList[obj->groups[i].material].name);
  }


 return 1;
}



void InitAutoTex()
{

   glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
   glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);


   glEnable(GL_TEXTURE_GEN_S);
   glEnable(GL_TEXTURE_GEN_T);
}

GLuint getDispList(struct OBJ_Model * obj)
{
	return obj->dispList;
}



void  drawOBJMesh(struct OBJ_Model * obj)
{
        if (obj == 0 ) { fprintf(stderr,"drawOBJMesh called with unloaded object \n"); return; }
        long unsigned int i,j;

        glDisable(GL_CULL_FACE);
		//for every group
		for(i=0; i<obj->numGroups; i++)
		{

		if (obj->matList!=0)
		 { //We might not have a material with our object!
			//if there is a bmp file to load the texture from, in the mtl file
			if( obj->matList[obj->groups[i].material].hasTex )
            {
				if( obj->matList[obj->groups[i].material].ldText>0)
				{
			 	  glEnable(GL_TEXTURE_2D);
				  glBindTexture(GL_TEXTURE_2D, obj->matList[ obj->groups[i].material].ldText);
				}
			}
			else
			 {	/*glDisable(GL_TEXTURE_2D);*/ }

			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  obj->matList[ obj->groups[i].material].ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  obj->matList[ obj->groups[i].material].diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, obj->matList[ obj->groups[i].material].specular);
			glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, obj->matList[ obj->groups[i].material].shine);



			//if the group has texture coordinates
			if(  ( obj->groups[i].hasTex) ==0 ) { InitAutoTex(); } else
			                               {
			                                 glDisable(GL_TEXTURE_GEN_S);
                                             glDisable(GL_TEXTURE_GEN_T);
			                               }
		}   else
        {
              //No Matterials , No Textures
			   glDisable(GL_TEXTURE_GEN_S);
               glDisable(GL_TEXTURE_GEN_T);
               glDisable(GL_TEXTURE_2D);
        }

			glBegin(GL_TRIANGLES);
			for(j=0; j<obj->groups[i].numFaces; j++)
			{
                if( obj->groups[i].hasNormals)
                  {
                    glNormal3f(
                                 obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[0]].n1 ,
                                 obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[0]].n2 ,
                                 obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[0]].n3
                              );
                  }
				else
				 {
					glNormal3f(
                                 obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n1,
                                 obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n2,
                                 obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n3
                              );
				}

				if( obj->groups[i].hasTex)
				  {
                    glTexCoord2f(
                                  obj->texList[ obj->faceList[ obj->groups[i].faceList[j]].t[0]].u,
                                  obj->texList[ obj->faceList[ obj->groups[i].faceList[j]].t[0]].v
                                );
                  }

				glVertex3f(
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[0]].x,
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[0]].y,
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[0]].z
                          );


				if( obj->groups[i].hasNormals)
				 {
                   glNormal3f(
                               obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[1]].n1,
                               obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[1]].n2,
                               obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[1]].n3
                             );
				 }
				else
				 {
                   glNormal3f(
                               obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n1,
                               obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n2,
                               obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n3
                              );
				 }

				if( obj->groups[i].hasTex)
				{
                  glTexCoord2f(
                                obj->texList[ obj->faceList[ obj->groups[i].faceList[j]].t[1]].u,
                                obj->texList[ obj->faceList[ obj->groups[i].faceList[j]].t[1]].v
                              );
				}

				glVertex3f(
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[1]].x,
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[1]].y,
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[1]].z
                          );


				if( obj->groups[i].hasNormals)
				 {
                    glNormal3f(
                                obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[2]].n1,
                                obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[2]].n2,
                                obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[2]].n3
                              );
				 }
				else
                 {
                    glNormal3f(
                                obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n1,
                                obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n2,
                                obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n3
                              );
                 }

				if( obj->groups[i].hasTex)
				{
                   glTexCoord2f(
                                 obj->texList[ obj->faceList[ obj->groups[i].faceList[j]].t[2]].u,
                                 obj->texList[ obj->faceList[ obj->groups[i].faceList[j]].t[2]].v
                               );
				}


				glVertex3f(
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[2]].x,
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[2]].y,
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[2]].z
                          );

			}//FOR J
			glEnd();
    }//FOR I

}



inline int findIntersectionInternal(struct OBJ_Model * obj,Face triangle, Vertex p1, Vertex p2, Vector * new_normal, Vector * intersection_point)
{
	Vector e1, e2, p, s, q;
	Vector bcoords;
	float t,u,v, tmp, e=0.000001;
	double l, new_normal_length;
	Vector v0,v1,v2,dir;
	Vector origin, end;
	origin.n1 = p1.x;
	origin.n2 = p1.y;
	origin.n3 = p1.z;

	end.n1 = p2.x;
	end.n2 = p2.y;
	end.n3 = p2.z;


	Subtraction(dir,end,origin);
	l = VectorLength(dir);
	dir.n1/=(float)l;
	dir.n2/=(float)l;
	dir.n3/=(float)l;
	v0.n1 = obj->vertexList[ triangle.v[0]].x;
	v0.n2 = obj->vertexList[ triangle.v[0]].y;
	v0.n3 = obj->vertexList[ triangle.v[0]].z;
	v1.n1 = obj->vertexList[ triangle.v[1]].x;
	v1.n2 = obj->vertexList[ triangle.v[1]].y;
	v1.n3 = obj->vertexList[ triangle.v[1]].z;
	v2.n1 = obj->vertexList[ triangle.v[2]].x;
	v2.n2 = obj->vertexList[ triangle.v[2]].y;
	v2.n3 = obj->vertexList[ triangle.v[2]].z;

	Subtraction(e1, v1, v0);
	Subtraction(e2, v2, v0);
	CrossProduct(p, dir, e2);
	tmp = DotProduct(p, e1);

	if (tmp > -e && tmp < e)
		return 0;

	tmp = 1.0/tmp;
	Subtraction(s, origin, v0);
	u = tmp * DotProduct(s, p);
	if (u<0 || u>1)
		return 0;

	CrossProduct(q, s, e1);
	v = tmp * DotProduct(dir, q);
	if (v<0 || v>1)
		return 0;

	if ( u+v >1)
		return 0;

	t = tmp * DotProduct(e2, q);

	bcoords.n2 = u;
	bcoords.n3 = v;
	bcoords.n1 = 1 - u - v;

	new_normal->n1 = bcoords.n1 * obj->normalList[ triangle.n[0]].n1 +
					bcoords.n2 * obj->normalList[ triangle.n[1]].n1 +
					bcoords.n3 * obj->normalList[ triangle.n[2]].n1;

	new_normal->n2 = bcoords.n1 * obj->normalList[ triangle.n[0]].n2 +
					bcoords.n2 * obj->normalList[ triangle.n[1]].n2 +
					bcoords.n3 * obj->normalList[ triangle.n[2]].n2;

	new_normal->n3 = bcoords.n1 * obj->normalList[ triangle.n[0]].n3 +
					bcoords.n2 * obj->normalList[ triangle.n[1]].n3 +
					bcoords.n3 * obj->normalList[ triangle.n[2]].n3;

    Vector tmpVec = *new_normal;
	new_normal_length = VectorLength(tmpVec);
	new_normal->n1 /=new_normal_length;
	new_normal->n2 /=new_normal_length;
	new_normal->n3 /=new_normal_length;

	intersection_point->n1 = dir.n1 * t + origin.n1;
	intersection_point->n2 = dir.n2 * t + origin.n2;
	intersection_point->n3 = dir.n3 * t + origin.n3;
	return 1;
}



int findIntersection(struct OBJ_Model * obj,Vertex v1, Vertex v2, Vector* new_normal, Vector* intersection_point)
// This is a generic method to compute an intersection between a semi-infinite line beginning at v1 and passing
// through v2 and the mesh. If the intersection between the line and the mesh is located before v1, then the
// intersection point is discarded. See the overloaded member below for argument details.
{
	int i, j;
	for(i=0;i<obj->numGroups;i++)
	{
		for(j=0;j< obj->groups[i].numFaces;j++)
		{
			if( findIntersectionInternal( obj,  obj->faceList[ obj->groups[i].faceList[j]], v1, v2, new_normal, intersection_point))
			   return 1;
		}//for j
	}//for i
	return 0;
}

void scaleMesh(struct OBJ_Model * obj , GLfloat sx, GLfloat sy, GLfloat sz)
{
	long unsigned int i;
	for(i=0; i<=obj->numVertices; i++)
	{
		obj->vertexList[i].x*=sx;
		obj->vertexList[i].y*=sy;
		obj->vertexList[i].z*=sz;
	}
	obj->boundBox.max.x*=sx;
	obj->boundBox.max.y*=sy;
	obj->boundBox.max.z*=sz;
	obj->boundBox.min.x*=sx;
	obj->boundBox.min.y*=sy;
	obj->boundBox.min.z*=sz;

	obj->center[0]*=sx;
	obj->center[1]*=sy;
	obj->center[2]*=sz;
}



int compileOBJList(struct OBJ_Model * obj)
{

	/*
	create	or replace a display list with :
	void glNewList( GLuint list, GLenum mode )
	void glEndList( void )
	where:
	list	Specifies the display-list name.

	mode	Specifies the compilation mode,	which can be
		GL_COMPILE or GL_COMPILE_AND_EXECUTE.

	  Display lists	are groups of GL commands that have been
	  stored for subsequent	execution.  Display lists are created
	  with glNewList.  All subsequent commands are placed in the
	  display list,	in the order issued, until glEndList is
	  called.

	  glNewList has	two arguments.	The first argument, list, is a
	  positive integer that	becomes	the unique name	for the
	  display list.	 Names can be created and reserved with
	  glGenLists
	*/


	long unsigned int i,j;

	//generate an empty display list, and save its id in dispList
	obj->dispList=glGenLists(1);

	glNewList(obj->dispList,GL_COMPILE);

	for(i=0; i<obj->numGroups; i++)
		{

		if (obj->matList!=0)
		 { //We might not have a material with our object!
			if( obj->matList[ obj->groups[i].material].hasTex)
			{
				if( obj->matList[ obj->groups[i].material].ldText>0)
				{
				  glEnable(GL_TEXTURE_2D);
				  glBindTexture(GL_TEXTURE_2D,  obj->matList[ obj->groups[i].material].ldText);
				}
			}
			else
			{
			  //glDisable(GL_TEXTURE_2D);
			}

			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  obj->matList[ obj->groups[i].material].ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  obj->matList[ obj->groups[i].material].diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, obj->matList[ obj->groups[i].material].specular);
			glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, obj->matList[ obj->groups[i].material].shine);

			if(  ( obj->groups[i].hasTex) ==0 )
			{
				InitAutoTex();
			}
			else
			{
				glDisable(GL_TEXTURE_GEN_S);
                glDisable(GL_TEXTURE_GEN_T);
			}
          } else
          {
              //No Matterials , No Textures
			   glDisable(GL_TEXTURE_GEN_S);
               glDisable(GL_TEXTURE_GEN_T);
               glDisable(GL_TEXTURE_2D);
          }

			glBegin(GL_TRIANGLES);
           for(j=0; j<obj->groups[i].numFaces; j++)
			{
                if( obj->groups[i].hasNormals)
                  {
                    glNormal3f(
                                 obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[0]].n1 ,
                                 obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[0]].n2 ,
                                 obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[0]].n3
                              );
                  }
				else
				 {
					glNormal3f(
                                 obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n1,
                                 obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n2,
                                 obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n3
                              );
				}

				if( obj->groups[i].hasTex)
				  {
                    glTexCoord2f(
                                  obj->texList[ obj->faceList[ obj->groups[i].faceList[j]].t[0]].u,
                                  obj->texList[ obj->faceList[ obj->groups[i].faceList[j]].t[0]].v
                                );
                  }

				glVertex3f(
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[0]].x,
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[0]].y,
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[0]].z
                          );


				if( obj->groups[i].hasNormals)
				 {
                   glNormal3f(
                               obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[1]].n1,
                               obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[1]].n2,
                               obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[1]].n3
                             );
				 }
				else
				 {
                   glNormal3f(
                               obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n1,
                               obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n2,
                               obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n3
                              );
				 }

				if( obj->groups[i].hasTex)
				{
                  glTexCoord2f(
                                obj->texList[ obj->faceList[ obj->groups[i].faceList[j]].t[1]].u,
                                obj->texList[ obj->faceList[ obj->groups[i].faceList[j]].t[1]].v
                              );
				}

				glVertex3f(
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[1]].x,
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[1]].y,
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[1]].z
                          );


				if( obj->groups[i].hasNormals)
				 {
                    glNormal3f(
                                obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[2]].n1,
                                obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[2]].n2,
                                obj->normalList[ obj->faceList[ obj->groups[i].faceList[j]].n[2]].n3
                              );
				 }
				else
                 {
                    glNormal3f(
                                obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n1,
                                obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n2,
                                obj->faceList[ obj->groups[i].faceList[j]].fc_normal.n3
                              );
                 }

				if( obj->groups[i].hasTex)
				{
                   glTexCoord2f(
                                 obj->texList[ obj->faceList[ obj->groups[i].faceList[j]].t[2]].u,
                                 obj->texList[ obj->faceList[ obj->groups[i].faceList[j]].t[2]].v
                               );
				}


				glVertex3f(
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[2]].x,
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[2]].y,
                            obj->vertexList[ obj->faceList[ obj->groups[i].faceList[j]].v[2]].z
                          );

			}//FOR J
			glEnd();

		}//FOR I
	glEndList();


}


///////////////////////////////////////////////////////////////////////////////
GLuint getObjOGLList(struct OBJ_Model * obj)
{
    if (obj==0) { fprintf(stderr,"Object does not exist \n"); return 0; }
    if (obj->dispList==0) { fprintf(stderr,"Object does not have a compiled list \n"); return 0; }
    return obj->dispList;
}



int unloadObj(struct OBJ_Model * obj)
{
    if (obj==0) { fprintf(stderr,"Object already deleted\n"); return 0; }

	if(obj->vertexList!=0) free(obj->vertexList);
	if(obj->normalList!=0) free(obj->normalList);
	if(obj->texList!=0)    free(obj->texList);
	if(obj->matList!=0)    free(obj->matList);
	if(obj->groups!=0)     free(obj->groups);
	if(obj->faceList!=0)   free(obj->faceList);


    free(obj);
	return 1;

}

struct OBJ_Model * loadObj(char * filename)
{
    fprintf(stderr,"Starting to load object %s \n",filename);
    struct OBJ_Model * obj = ( struct OBJ_Model * ) malloc(sizeof(struct OBJ_Model));
    if ( obj == 0 )  { fprintf(stderr,"Could not allocate enough space for model %s \n",filename);  return 0; }


    memset (obj,0,sizeof(struct OBJ_Model));
	obj->scale=1.0f;

    unsigned int file_name_length = strlen(filename);
    if (file_name_length > 60 ) { fprintf(stderr,"Huge filename provided , will not loadObject ( 60 char limit ) \n"); return 0; }
	strncpy(obj->filename, filename, 60 );

    if (!readOBJ(obj) ) { fprintf(stderr," Could not read object %s \n",filename); unloadObj(obj); return 0;}
    if (!calculateBoundingBox(obj)) { fprintf(stderr," Could not calculate bounding box for object %s \n",filename); unloadObj(obj); return 0;}
    if (!prepareObject(obj))  { fprintf(stderr," Could not prepare object %s \n",filename); unloadObj(obj); return 0;}
    if (!calculateBoundingBox(obj)) { fprintf(stderr," Could not calculate bounding box for object %s \n",filename); unloadObj(obj); return 0;}


    compileOBJList(obj);


   return obj;
}

