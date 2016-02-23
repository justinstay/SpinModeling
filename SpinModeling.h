#ifndef LIB_SPINMODELING
#define LIB_SPINMODELING
/* ------------------ #Define Statments ------------------------- */
#define LIB_SPINMODELING_DEBUG 0
#define LIB_SPINMODELING_NORMALIZEARC 1
#define LIB_SPINMODELING_ARCLENGTHINDEGREES 40.0
/* ------------------ #Define Statments ------------------------- */

/* ------------------ #Include Statments ------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* ------------------ #Include Statments ------------------------ */

/* ------------------ Type Def Structs -------------------------- */
typedef struct
{
  double x;
  double y;
  double z;
} vector;

typedef struct
{
  double r;  
  double phi;
  double theta;
} sphericalCoordinate;

typedef struct
{
  double spinAxisInDegrees;
  double spinInDegrees;
  double spinInRPMs;
} spinDescription;

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
spinDescription calcSpinAxisAndSpin(vector point1Time1, vector point2Time1, vector ballCenterTime1, double ballRadiusTime1, vector point1Time2, vector point2Time2, vector ballCenterTime2, double ballRadiusTime2, double deltaTimeInSeconds);

#endif
