#include "projection.h"
#include <stdio.h>
#include <stdlib.h>


#include "math.h"
// Define our six cube faces.
// 0 - 3 are side faces, clockwise order
// 4 and 5 are top and bottom, respectively
float faceTransform[6][2] =
{
    {0, 0},
    {M_PI / 2, 0},
    {M_PI, 0},
    {-M_PI / 2, 0},
    {0, -M_PI / 2},
    {0, M_PI / 2}
};

// Map a part of the equirectangular panorama (in) to a cube face
// (face). The ID of the face is given by faceId. The desired
// width and height are given by width and height.


void createCubeMapFace(char * out , unsigned int outWidth ,unsigned int outHeight , unsigned int outChannels , unsigned int outBitsPerPixel ,
                       char * in , unsigned int inWidth , unsigned int inHeight , unsigned int inChannels , unsigned int inBitsPerPixel
                        )
{
 // http://stackoverflow.com/questions/29678510/convert-21-equirectangular-panorama-to-cube-map

    unsigned int faceId = 0;
    unsigned int width = inWidth;
    unsigned int height = inHeight;

    // Allocate map

    float * mapx = (float * ) malloc(outWidth * outHeight * sizeof(float));
    if (mapx==0) { return ; }
    float * mapy = (float * ) malloc(outWidth * outHeight * sizeof(float));
    if (mapy==0) { free(mapx); return ; }


    // Calculate adjacent (ak) and opposite (an) of the
    // triangle that is spanned from the sphere center
    //to our cube face.
    const float an = sin(M_PI / 4);
    const float ak = cos(M_PI / 4);

    const float ftu = faceTransform[faceId][0];
    const float ftv = faceTransform[faceId][1];

    // For each point in the target image,
    // calculate the corresponding source coordinates.

    unsigned int x=0, y=0;
    for(y = 0; y < height; y++)
     {
      for(x = 0; x < width; x++)
      {
        // Map face pixel coordinates to [-1, 1] on plane
        float nx = (float)y / (float)height - 0.5f;
        float ny = (float)x / (float)width - 0.5f;

        nx *= 2;
        ny *= 2;

        // Map [-1, 1] plane coords to [-an, an]
        // thats the coordinates in respect to a unit sphere
        // that contains our box.
        nx *= an;
        ny *= an;

        float u, v;

        // Project from plane to sphere surface.
        if(ftv == 0)
            {  // Center faces
                u = atan2(nx, ak);
                v = atan2(ny * cos(u), ak);
                u += ftu;
            } else
        if(ftv > 0)
            {   // Bottom face
                float d = sqrt(nx * nx + ny * ny);
                v = M_PI / 2 - atan2(d, ak);
                u = atan2(ny, nx);
            } else
            {   // Top face
                float d = sqrt(nx * nx + ny * ny);
                v = -M_PI / 2 + atan2(d, ak);
                u = atan2(-ny, nx);
            }

            // Map from angular coordinates to [-1, 1], respectively.
            u = u / (M_PI);
            v = v / (M_PI / 2);

            // Warp around, if our coordinates are out of bounds.
            while (v < -1)  { v += 2; u += 1; }
            while (v > 1)   { v -= 2; u += 1; }

            while(u < -1) { u += 2; }
            while(u > 1)  { u -= 2; }

            // Map from [-1, 1] to in texture space
            u = u / 2.0f + 0.5f;     v = v / 2.0f + 0.5f;
            u = u * (inWidth - 1);   v = v * (inHeight - 1);

            // Save the result for this pixel in map
            mapx[y*outWidth + x] = u;
            mapy[y*outWidth + x] = v;

            //mapx.at<float>(x, y) = u;
            //mapy.at<float>(x, y) = v;
        }
    }

    // Recreate output image if it has wrong size or type.
    //if(face.cols != width || face.rows != height ||
    //    face.type() != in.type()) {
    //    face = Mat(width, height, in.type());
    //}
fprintf(stderr,"Recoonstructing projections\n ");
for(y = 0; y < height; y++)
     {
      for(x = 0; x < width; x++)
      {
          unsigned int outX =  (unsigned int) mapx[y*outWidth + x];
          unsigned int outY =  (unsigned int) mapy[y*outWidth + x];

          if (outX>=outWidth)  { outX=outWidth-1;  }
          if (outY>=outHeight) { outY=outHeight-1; }
          unsigned int inPlace = y * inWidth + x;
          unsigned int outPlace = outY * outWidth + outX;
          out[outPlace]=in[inPlace]; ++outPlace; ++inPlace;
          out[outPlace]=in[inPlace]; ++outPlace; ++inPlace;
          out[outPlace]=in[inPlace];
      }
     }



    free(mapx);
    free(mapy);

    // Do actual resampling using OpenCV's remap
    // remap(in, face, mapx, mapy, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
   /* */
}
