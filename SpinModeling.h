/*                 Version 3.0.0                 */
#ifndef LIB_SPINMODELING
#define LIB_SPINMODELING
/* ------------------ #Define Statements ------------------------- */
/* Booleans */
#define LIB_SPINMODELING_DEBUG 0
#define LIB_SPINMODELING_NORMALIZEARC 1
#define LIB_SPINMODELING_FOVCORRECTION 1
#define LIB_SPINMODELING_BARREL_DISTORTION_CORRECTION 1
/* Values */
#define LIB_SPINMODELING_ARCLENGTHINDEGREES 40.0
#define LIB_SPINMODELING_RIGHTHANDED 1
#define LIB_SPINMODELING_LEFTHANDED -1
#define LIB_SPINMODELING_FIELDOFVIEW_FIRST_IMAGE 70.0
#define LIB_SPINMODELING_FIELDOFVIEW_SECOND_IMAGE 70.0
#define LIB_SPINMODELING_NUMBER_OF_PIXELS_WIDTH 752
#define LIB_SPINMODELING_NUMBER_OF_PIXELS_HEIGHT 480
#define LIB_SPINMODELING_BARREL_DISTORTION_ALPHA_FIRST_IMAGE 9e-7
#define LIB_SPINMODELING_BARREL_DISTORTION_ALPHA_SECOND_IMAGE 9e-7


/* ------------------ #Define Statments ------------------------- */

/* ------------------ #Include Statments ------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* ------------------ #Include Statments ------------------------ */

/* ------------------ Type Def Structs -------------------------- */
/*
 *  Defines a 3 dimensional vector
 */
typedef struct
{
  double x;
  double y;
  double z;
} vector;

/*
 *  Defines a 3 dimensional spherical coordinate
 */
typedef struct
{
  double r;  
  double phi;
  double theta;
} sphericalCoordinate;

/*
 *  Defines the final spin description
 */
typedef struct
{
  double spinAxisInDegrees;
  double spinInDegrees;
  double spinInRPMs;
} spinDescription;

/*
 *  When performing FOV (perspective) correction, these angles are required to correct for that
 */
typedef struct
{
  double alpha;
  double beta;
} FOVCorrection;

typedef struct logoDescription
{
  double yaw;
  double pitch;
  double roll;
} logoDescription;

typedef struct logoList
{
  int logoID;
  logoDescription * thisLogo;
  struct logoList * nextLogo;
} logoList;


double vectorMag(vector A);
double vectorDotProduct(vector A, vector B);
vector vectorCrossProduct(vector A, vector B);
vector vectorAdd(vector A, vector B);
vector vectorSubtract(vector A, vector B);
vector vectorScalarDivide(vector A, double B);
sphericalCoordinate vectorToSpherical(vector A);
vector rotateX(vector A,double theta);
vector rotateY(vector A,double theta);
vector rotateZ(vector A,double theta);
vector projectXY(vector A);
FOVCorrection calcFOVCorrection(vector ballCenter,double FOVInDegrees,int imageWidth,int imageHeight);
vector correctForFOV(vector point,FOVCorrection myFOVCorrection);
vector correctForBarrelDistortion(vector point,int imageWidth, int imageHeight, double distortionAlpha);
spinDescription calcSpinAxisAndSpin(vector point1Time1, vector point2Time1, vector ballCenterTime1, double ballRadiusTime1, vector point1Time2, vector point2Time2, vector ballCenterTime2, double ballRadiusTime2, double deltaTimeInSeconds, int handedness);
logoList * initLogoList();
void addLogo(logoList * myLogoList, int logoID, double newRoll, double newPitch, double newYaw);
void freeLogoList(logoList * myLogoList);

#endif
