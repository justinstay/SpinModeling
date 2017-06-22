/*                 Version 4.1.0                */
#include "SpinModeling.h"

// --------------------------------------------------//
// Function calculates magnitude of a vector.
// --------------------------------------------------//
double vectorMag(vector A)
{
  return sqrt(A.x*A.x + A.y*A.y + A.z*A.z);
}

// --------------------------------------------------//
// Calculates the dot product of two vectors.
// --------------------------------------------------//
double vectorDotProduct(vector A, vector B)
{
  return A.x*B.x + A.y*B.y + A.z*B.z;
}

// --------------------------------------------------//
// Calculates the cross product of two vectors.
// --------------------------------------------------//
vector vectorCrossProduct(vector A, vector B)
{
  vector C;
  C.x=A.y*B.z-A.z*B.y;
  C.y=A.z*B.x-A.x*B.z;
  C.z=A.x*B.y-A.y*B.x;
  return C;
}

// --------------------------------------------------//
// Calculates the addition of two vectors.
// --------------------------------------------------//
vector vectorAdd(vector A, vector B)
{
  vector C;
  C.x=A.x+B.x;
  C.y=A.y+B.y;
  C.z=A.z+B.z;
  return C;
}

// --------------------------------------------------//
// Calculates the subtract of two vectors (A-B)
// --------------------------------------------------//
vector vectorSubtract(vector A, vector B)
{
  vector C;
  C.x=A.x-B.x;
  C.y=A.y-B.y;
  C.z=A.z-B.z;
  return C;
}

// --------------------------------------------------//
// Performcs a scalar divide of a vector vector A / scalar B
// --------------------------------------------------//
vector vectorScalarDivide(vector A, double B)
{
  vector C;
  C.x=A.x/B;
  C.y=A.y/B;
  C.z=A.z/B;   
  return C;
}

// --------------------------------------------------//
// Converts cartesian coordiantes to spherical coordiantes.
// --------------------------------------------------//
sphericalCoordinate vectorToSpherical(vector A)
{
  sphericalCoordinate B;
  B.r=sqrt(A.x*A.x+A.y*A.y+A.z*A.z);
  B.theta=atan2(A.y,A.x);
  B.phi=acos(A.z/B.r);
  return B;
}

// --------------------------------------------------//
// Performs a right-hand-rotation about the x-axis
// --------------------------------------------------//
vector rotateX(vector A,double theta)
{
  vector B;
  B.x = 1.0*A.x;
  B.y = cos(theta) * A.y - sin(theta)*A.z;
  B.z = sin(theta) * A.y + cos(theta)*A.z;
  return B;
}

// --------------------------------------------------//
// Performs a right-hand-rotation about the y-axis
// --------------------------------------------------//
vector rotateY(vector A,double theta)
{
  vector B;
  B.x = cos(theta) * A.x + sin(theta)*A.z;  
  B.y = 1.0*A.y;  
  B.z = -sin(theta) * A.x + cos(theta)*A.z;
  return B;
}

// --------------------------------------------------//
// Performs a right-hand-rotation about the z-axis
// --------------------------------------------------//
vector rotateZ(vector A,double theta)
{
  vector B;
  B.x = cos(theta) * A.x - sin(theta)*A.y;  
  B.y = sin(theta) * A.x + cos(theta)*A.y;  
  B.z = 1.0*A.z;  
  return B;
}

// --------------------------------------------------//
// Projects a vector upon the xy plane by zero-ing the z component.
// --------------------------------------------------//
vector projectXY(vector A)
{
  vector B;
  B.x = A.x;
  B.y=A.y;
  B.z=0;
  return B;
}

/* --------------------------------------------------
   Calculates the rotation angles required for correction for the FOV, i.e. perspective.
   The line of sight to the ball is not parallel to the optic axis.  These angles adjust for that. 

   Input Arguments

   ballCenter - Center of the ball in the image in pixels, with (1,1) is at the lower left
   FOVInDegrees - Field of View in degrees from corner to corner of the image arr
   imageHeight - The number of pixels in the height, or y direction, of the image arra
   imageWidth - The number of pixels in the width, or x direction, of the image array

   Output Arguments

   FOVCorrection - the angle required to rotate detected points on the golf ball given this field of view once normalized (Centered at 0,0, radius of 1)
   -------------------------------------------------- */
FOVCorrection calcFOVCorrection(vector ballCenter,double FOVInDegrees,int imageWidth,int imageHeight)
{
  double FOVInRads;
  // Define the center of the picture
  vector centerOfImage;
  // Center ball center
  vector recenteredBallCenter;
  // Define some angles
  // Phi is the angle from the optic axis to the ball. If the ball center is exactly in the corner, than phi = FOV/2
  double phi;
  // d1 is the distance, in pixels from the center of the image to the ball
  double d1;
  // d2 is the distance from the lens to the ball (in pixels) to satisfy the FOV
  // error maybe!  d2 is the distance along the optic axis to the ball (z coordinate)??
  double d2;
  // d3 is the distance on the xz plane of the projection from the center to the ball
  double d3;
  // alpha is the rotation about the y axis that the center of the ball experiences due to the FOV.
  double alpha;
  // beta is the angle between the xz plane and the line of site to the ball
  double beta;
  // this is the return values to hold alpha and beta
  FOVCorrection myFOVCorrection;

  FOVInRads = FOVInDegrees * M_PI / 180.0;

  centerOfImage.x = ((double) imageWidth + 1)/2;
  centerOfImage.y = ((double) imageHeight +1)/2;
  centerOfImage.z = 0;

  // Normalize center of ball from center of image
  recenteredBallCenter = vectorSubtract(ballCenter,centerOfImage);

  phi = atan( tan(FOVInRads/2) / (sqrt(((double) imageHeight-1)*((double) imageHeight-1)+((double) imageWidth-1)*((double) imageWidth-1))/2) * vectorMag(recenteredBallCenter));
  d1 = vectorMag(recenteredBallCenter);
  d2 = d1 / tan(phi);
  d3 = sqrt(recenteredBallCenter.x*recenteredBallCenter.x+d2*d2);

  alpha = atan(recenteredBallCenter.x/d2);
  beta = atan(recenteredBallCenter.y/d3);

  myFOVCorrection.alpha = alpha;
  myFOVCorrection.beta = beta;

  if (LIB_SPINMODELING_DEBUG)
    {
      printf("phi = %f, d1 = %f, d2 = %f, d3 = %f, alpha = %f, beta = %f\n",phi,d1,d2,d3,alpha,beta);
    }

  return myFOVCorrection;
}

vector correctForFOV(vector point,FOVCorrection myFOVCorrection)
{
  return rotateX(rotateY(point,-myFOVCorrection.alpha),myFOVCorrection.beta);
}

/* --------------------------------------------------
   Corrects for barrel distortion using a simple model.
   See http://stackoverflow.com/questions/6199636/formulas-for-barrel-pincushion-distortion

   There are more complicated ones but this one is simple and seems to do well in matlab.
   Could very easily incorporate a more complicated method in this funcion.

   Input Arguments
   
   point           - a point in the image in pixels (1,1) is lower left hand of image
   imageWidth      - width of image in pixels
   imageHeight     - height of image in pixels
   distortionAlpha - parameter used to define the amount of distortion in the image

   Output Arguments

   newPoint        - transformed point in pixels
-------------------------------------------------- */

vector correctForBarrelDistortion(vector point,int imageWidth, int imageHeight, double distortionAlpha)
{
  // Center of the image
  vector centerOfImage;
  // Radial distance to the center of the image from the point
  double distanceFromCenter;
  // Scale factor from transform
  double gamma;
  // Transformed point
  vector newPoint;

  centerOfImage.x = ((double) imageWidth + 1)/2;
  centerOfImage.y = ((double) imageHeight +1)/2;
  centerOfImage.z = 0;

  distanceFromCenter = vectorMag(vectorSubtract(point,centerOfImage));

  gamma = 1 / (1 - distortionAlpha * distanceFromCenter*distanceFromCenter);

  newPoint.x = gamma*(point.x-centerOfImage.x)+centerOfImage.x;
  newPoint.y = gamma*(point.y-centerOfImage.y)+centerOfImage.y;
  newPoint.z = gamma*(point.z-centerOfImage.z)+centerOfImage.z;

 return newPoint;
}


/* --------------------------------------------------
   Calcules the spin direction for a set of images and two points.
   This approach uses the two point approach.  Given two points
   observed in two images, the spin and spin axis can be calculated.
   
   Note the conventions used for handedness.  For a right handed golfer,
   backspin is assumed to be right hand rule positive, i.e., counter clockwise
   in the image.  Although not specifically checked, the ball should be moving
   right.  Conversely, for a left handed golfer, backspin is assumed to be
   left hand rule positive, i.e., clockwise in the image.  Although not specifically
   checked, the ball should be moving left.

   Input Arguments

   point1Time1     - vector, location in XY space indicating the postion of a point #1 @ time #1
   point1Time2     - vector, location in XY space indicating the postion of a point #1 @ time #2
   ballCenterTime1 - vector, location in XY space indicating the center of the ball in time #1
   ballRadiusTime1 - double, radius of ball in image @ time #1
   time1LogoID     - ID of the ball logo used during time 1
   point2Time1     - vector, location in XY space indicating the postion of a point #2 @ time #1
   point2Time2     - vector, location in XY space indicating the postion of a point #2 @ time #2
   ballCenterTime2 - vector, location in XY space indicating the center of the ball in time #2
   ballRadiusTime2 - double, radius of ball in image @ time #2
   time2LogoID     - ID of the ball logo used during time 2
   handedness      - int   , 1 for right handed, otherwise left handed.

   Output Arguments

   spinDescription - return a spin description (spin, spin axis, etc.).  See header file for more details.
   -------------------------------------------------- */

spinDescription calcSpinAxisAndSpin(vector point1Time1, vector point2Time1, vector ballCenterTime1, double ballRadiusTime1, int time1LogoID,
				    vector point1Time2, vector point2Time2, vector ballCenterTime2, double ballRadiusTime2, int time2LogoID,
				    double deltaTimeInSeconds, int handedness)
{

  // Defines the midpoint on the arc connection points 1 and time at time 1, and at time 1
  vector time1Midpoint;
  vector time2Midpoint;

  // Defines a vector normal to the points at time #1 and time #2 (cross product)
  vector time1Normal;
  vector time2Normal;
  sphericalCoordinate time1Normal_spherical;
  sphericalCoordinate time2Normal_spherical;

  // FOV corrections for time 1 and time 2
  FOVCorrection FOVCorrectionForTime1;
  FOVCorrection FOVCorrectionForTime2;

  // Defines the midpoint on the arc connecting a point at time #1 and time #2.
  // Normalized to unity and centered at the origin.
  vector point1MidPoint;
  vector point2MidPoint;

  // Defines a vector normal to a point at time #1 and time #2 (cross product).
  // Normalized to unity and centered at the origin.
  vector point1Normal;
  vector point2Normal;

  // Defines a vector that is normal to the great circle that bisects the arc
  // defined by a point at time #1 and time #2.
  // Based on vecotrs that were normalized to unity and centered at the origin.
  vector point1GC;
  vector point2GC;

    // Temporary variables used in calculating the spin axis.
  double h,g,k;

  // Spin axis defined in both cartesian and spherical coordiantes.
  vector spinAxis;
  sphericalCoordinate spinAxisSpherical;

  // Projections of a point at time #1 and time #2 on a plane perpendicular
  // to the spin axis.
  vector point1Time1Prime;
  vector point1Time2Prime;
  vector point2Time1Prime;
  vector point2Time2Prime;

  // Cross product of the projected point 1's
  vector projectedCrossProduct;

  // Amount of spin calculated between the two times
  double spinMagInDegrees;
  double spinMagInDegrees_point2;

  // Spin axis in degrees
  double spinAxisInDegrees;
  // Spin in RPMs
  double spinInRPMs;

  // Spin axis pitch in relation to ZX plane
  double spinAxisZXPitchInDegrees;

  // Y axis unit vector
  vector yAxisUnitVector;

  // Output arguement.  See header file.
  spinDescription mySpinDescription;

  double myArg;

  // Some temp point holder to calc the new radius after distortion correction
  vector radialVector;
  vector radialPoint;
  double maxRadius;
  vector correctedCenter;
  int k1;

  // Logo list
  logoList * myLogoList;

  /*
   *  Let's define the logo list to use.  Here we have hardcoded it.  Still have to free the list at the end.
   *  But this is easily extended to be dynamic in the future.
   */
  myLogoList = initLogoList();
  addLogo(myLogoList,1,0,270,0);
  addLogo(myLogoList,2,0,315,135);
  addLogo(myLogoList,3,0,315,225);
  addLogo(myLogoList,4,0,315,315);
  addLogo(myLogoList,5,0,315,45);
  addLogo(myLogoList,6,0,0,0);
  addLogo(myLogoList,7,0,0,90);
  addLogo(myLogoList,8,180,180,0);
  addLogo(myLogoList,9,0,0,270);
  addLogo(myLogoList,10,0,45,135);
  addLogo(myLogoList,11,0,45,225);
  addLogo(myLogoList,12,0,45,315);
  addLogo(myLogoList,13,0,45,45);
  addLogo(myLogoList,14,0,90,0);

  // Logo descriptions for both times
  logoDescription time1LogoDescription;
  logoDescription time2LogoDescription;

  if (LIB_SPINMODELING_BARREL_DISTORTION_CORRECTION)
    {
      point1Time1 = correctForBarrelDistortion(point1Time1,LIB_SPINMODELING_NUMBER_OF_PIXELS_WIDTH,LIB_SPINMODELING_NUMBER_OF_PIXELS_HEIGHT,LIB_SPINMODELING_BARREL_DISTORTION_ALPHA_FIRST_IMAGE);
      point2Time1 = correctForBarrelDistortion(point2Time1,LIB_SPINMODELING_NUMBER_OF_PIXELS_WIDTH,LIB_SPINMODELING_NUMBER_OF_PIXELS_HEIGHT,LIB_SPINMODELING_BARREL_DISTORTION_ALPHA_FIRST_IMAGE);
      correctedCenter = correctForBarrelDistortion(ballCenterTime1,LIB_SPINMODELING_NUMBER_OF_PIXELS_WIDTH,LIB_SPINMODELING_NUMBER_OF_PIXELS_HEIGHT,LIB_SPINMODELING_BARREL_DISTORTION_ALPHA_FIRST_IMAGE);      
      maxRadius = 0;
      for (k1=1;k1<=100;k1++)
	{
	  radialVector.x = ballRadiusTime1*cos(k1/100.0*2.0*M_PI);
	  radialVector.y = ballRadiusTime1*sin(k1/100.0*2.0*M_PI);
	  radialVector.z = 0;
	  radialPoint = vectorAdd(ballCenterTime1,radialVector);
	  radialPoint = correctForBarrelDistortion(radialPoint,LIB_SPINMODELING_NUMBER_OF_PIXELS_WIDTH,LIB_SPINMODELING_NUMBER_OF_PIXELS_HEIGHT,LIB_SPINMODELING_BARREL_DISTORTION_ALPHA_FIRST_IMAGE);
	  radialPoint = vectorSubtract(radialPoint,correctedCenter);
	  //printf("My diameter is %f\n",vectorMag(radialPoint));
	  maxRadius = fmax(maxRadius,vectorMag(radialPoint));
	}      
      ballRadiusTime1 = maxRadius;
      ballCenterTime1 = correctedCenter;
      /*   For time 2 */
      point1Time2 = correctForBarrelDistortion(point1Time2,LIB_SPINMODELING_NUMBER_OF_PIXELS_WIDTH,LIB_SPINMODELING_NUMBER_OF_PIXELS_HEIGHT,LIB_SPINMODELING_BARREL_DISTORTION_ALPHA_SECOND_IMAGE);
      point2Time2 = correctForBarrelDistortion(point2Time2,LIB_SPINMODELING_NUMBER_OF_PIXELS_WIDTH,LIB_SPINMODELING_NUMBER_OF_PIXELS_HEIGHT,LIB_SPINMODELING_BARREL_DISTORTION_ALPHA_SECOND_IMAGE);
      correctedCenter = correctForBarrelDistortion(ballCenterTime2,LIB_SPINMODELING_NUMBER_OF_PIXELS_WIDTH,LIB_SPINMODELING_NUMBER_OF_PIXELS_HEIGHT,LIB_SPINMODELING_BARREL_DISTORTION_ALPHA_SECOND_IMAGE);            
      maxRadius = 0;
      for (k1=1;k1<=100;k1++)
	{
	  radialVector.x = ballRadiusTime2*cos(k1/100.0*2.0*M_PI);
	  radialVector.y = ballRadiusTime2*sin(k1/100.0*2.0*M_PI);
	  radialVector.z = 0;
	  radialPoint = vectorAdd(ballCenterTime2,radialVector);
	  radialPoint = correctForBarrelDistortion(radialPoint,LIB_SPINMODELING_NUMBER_OF_PIXELS_WIDTH,LIB_SPINMODELING_NUMBER_OF_PIXELS_HEIGHT,LIB_SPINMODELING_BARREL_DISTORTION_ALPHA_SECOND_IMAGE);
	  radialPoint = vectorSubtract(radialPoint,correctedCenter);
	  //printf("My diameter is %f\n",vectorMag(radialPoint));
	  maxRadius = fmax(maxRadius,vectorMag(radialPoint));
	}            
      ballRadiusTime2 = maxRadius;
      ballCenterTime2 = correctedCenter;
      
      if (LIB_SPINMODELING_DEBUG)
	{
	  printf("Distortion corrected vectors\n");
	  printf("p1t1 x=%f, y=%f, z=%f\n",point1Time1.x,point1Time1.y,point1Time1.z);
	  printf("p1t2 x=%f, y=%f, z=%f\n",point1Time2.x,point1Time2.y,point1Time2.z);
	  printf("p2t1 x=%f, y=%f, z=%f\n",point2Time1.x,point2Time1.y,point2Time1.z);
	  printf("p2t2 x=%f, y=%f, z=%f\n",point2Time2.x,point2Time2.y,point2Time2.z);
	  printf("centert1 x=%f, y=%f, z=%f\n",ballCenterTime1.x,ballCenterTime1.y,ballCenterTime1.z);
	  printf("centert2 x=%f, y=%f, z=%f\n",ballCenterTime2.x,ballCenterTime2.y,ballCenterTime2.z);
	  printf("radiust1 = %f\n",ballRadiusTime1);
	  printf("radiust2 = %f\n",ballRadiusTime2);

 
	}
    }

  
  // Normalize all 4 points (2 points, 2 times) to the origin and unity.
  point1Time1   = vectorSubtract(point1Time1,ballCenterTime1);
  point1Time1.z = sqrt(ballRadiusTime1*ballRadiusTime1 - vectorMag(point1Time1)*vectorMag(point1Time1));
  point1Time1   = vectorScalarDivide(point1Time1,vectorMag(point1Time1)); 
    
  point2Time1 = vectorSubtract(point2Time1,ballCenterTime1);
  point2Time1.z = sqrt(ballRadiusTime1*ballRadiusTime1 - vectorMag(point2Time1)*vectorMag(point2Time1));
  point2Time1   = vectorScalarDivide(point2Time1,vectorMag(point2Time1));;

  point1Time2 = vectorSubtract(point1Time2,ballCenterTime2);
  point1Time2.z = sqrt(ballRadiusTime2*ballRadiusTime2 - vectorMag(point1Time2)*vectorMag(point1Time2));
  point1Time2   = vectorScalarDivide(point1Time2,vectorMag(point1Time2));

  point2Time2 = vectorSubtract(point2Time2,ballCenterTime2);
  point2Time2.z = sqrt(ballRadiusTime2*ballRadiusTime2 - vectorMag(point2Time2)*vectorMag(point2Time2));
  point2Time2   = vectorScalarDivide(point2Time2,vectorMag(point2Time2));

  // Print normalized vectors
  if (LIB_SPINMODELING_DEBUG)
    {
      printf("p1t1 x=%f, y=%f, z=%f\n",point1Time1.x,point1Time1.y,point1Time1.z);
      printf("p1t2 x=%f, y=%f, z=%f\n",point1Time2.x,point1Time2.y,point1Time2.z);
      printf("p2t1 x=%f, y=%f, z=%f\n",point2Time1.x,point2Time1.y,point2Time1.z);
      printf("p2t2 x=%f, y=%f, z=%f\n",point2Time2.x,point2Time2.y,point2Time2.z);
    }

  if (LIB_SPINMODELING_FOVCORRECTION)
    {
      FOVCorrectionForTime1 = calcFOVCorrection(ballCenterTime1,LIB_SPINMODELING_FIELDOFVIEW_FIRST_IMAGE,LIB_SPINMODELING_NUMBER_OF_PIXELS_WIDTH,LIB_SPINMODELING_NUMBER_OF_PIXELS_HEIGHT);
      FOVCorrectionForTime2 = calcFOVCorrection(ballCenterTime2,LIB_SPINMODELING_FIELDOFVIEW_SECOND_IMAGE,LIB_SPINMODELING_NUMBER_OF_PIXELS_WIDTH,LIB_SPINMODELING_NUMBER_OF_PIXELS_HEIGHT);
      if (LIB_SPINMODELING_DEBUG)
      {
      printf("alpha t1 = %f, beta t1 = %f, alpha t2 = %f, beta t2 = %f\n",FOVCorrectionForTime1.alpha,FOVCorrectionForTime1.beta,FOVCorrectionForTime2.alpha,FOVCorrectionForTime2.beta);
      }

      point1Time1 = correctForFOV(point1Time1,FOVCorrectionForTime1);
      point2Time1 = correctForFOV(point2Time1,FOVCorrectionForTime1);

      point1Time2 = correctForFOV(point1Time2,FOVCorrectionForTime2);
      point2Time2 = correctForFOV(point2Time2,FOVCorrectionForTime2);
    }

  if (LIB_SPINMODELING_DEBUG)
    {
      printf("p1t1 x=%f, y=%f, z=%f\n",point1Time1.x,point1Time1.y,point1Time1.z);
      printf("p1t2 x=%f, y=%f, z=%f\n",point1Time2.x,point1Time2.y,point1Time2.z);
      printf("p2t1 x=%f, y=%f, z=%f\n",point2Time1.x,point2Time1.y,point2Time1.z);
      printf("p2t2 x=%f, y=%f, z=%f\n",point2Time2.x,point2Time2.y,point2Time2.z);
    }



  // We'll normalize the arcs (defined by point 1 and point 2 at time 1, . . .) to a default arc length
  
  if (LIB_SPINMODELING_NORMALIZEARC)
    {
      // Calculate the midpoint of each arc at time 1, and at time 2
      time1Midpoint = vectorAdd(point1Time1, point2Time1);
      time1Midpoint = vectorScalarDivide(time1Midpoint,vectorMag(time1Midpoint));
      time2Midpoint = vectorAdd(point1Time2, point2Time2);
      time2Midpoint = vectorScalarDivide(time2Midpoint,vectorMag(time2Midpoint));

      // Calculate the vector normal to the arc defined at time 1, and at time 2
      time1Normal = vectorCrossProduct(point1Time1,point2Time1);
      time2Normal = vectorCrossProduct(point1Time2,point2Time2);
  
      // We'll get the spherical cooridantes of the above vector
      time1Normal_spherical = vectorToSpherical(time1Normal);
      time2Normal_spherical = vectorToSpherical(time2Normal);

      // Create equal arc lengths at each time by redefining the endpoints but keepin the same midpoint and great circle (defined by the arc)
      point1Time1 = rotateZ(rotateY(rotateZ(rotateY(rotateZ(time1Midpoint,-time1Normal_spherical.theta),-time1Normal_spherical.phi),-LIB_SPINMODELING_ARCLENGTHINDEGREES/2.0*M_PI/180.0),time1Normal_spherical.phi),time1Normal_spherical.theta);
      point2Time1 = rotateZ(rotateY(rotateZ(rotateY(rotateZ(time1Midpoint,-time1Normal_spherical.theta),-time1Normal_spherical.phi),LIB_SPINMODELING_ARCLENGTHINDEGREES/2.0*M_PI/180.0),time1Normal_spherical.phi),time1Normal_spherical.theta);
      point1Time2 = rotateZ(rotateY(rotateZ(rotateY(rotateZ(time2Midpoint,-time2Normal_spherical.theta),-time2Normal_spherical.phi),-LIB_SPINMODELING_ARCLENGTHINDEGREES/2.0*M_PI/180.0),time2Normal_spherical.phi),time2Normal_spherical.theta);
      point2Time2 = rotateZ(rotateY(rotateZ(rotateY(rotateZ(time2Midpoint,-time2Normal_spherical.theta),-time2Normal_spherical.phi),LIB_SPINMODELING_ARCLENGTHINDEGREES/2.0*M_PI/180.0),time2Normal_spherical.phi),time2Normal_spherical.theta);
    }

  if (LIB_SPINMODELING_DEBUG)
    {
      printf("p1t1 x=%f, y=%f, z=%f\n",point1Time1.x,point1Time1.y,point1Time1.z);
      printf("p1t2 x=%f, y=%f, z=%f\n",point1Time2.x,point1Time2.y,point1Time2.z);
      printf("p2t1 x=%f, y=%f, z=%f\n",point2Time1.x,point2Time1.y,point2Time1.z);
      printf("p2t2 x=%f, y=%f, z=%f\n",point2Time2.x,point2Time2.y,point2Time2.z);
    }

  /*
   *  At this point, we have corrected for barrel distortion and FOV (persepective) and have created
   *  and normalized two sets of points from the original images.
   *
   *  If these sets of points originate from the same logo, then no rotation will occur.  However,
   *  if the two logos are differente, we need to rotate the 2nd set of points such that they are
   *  in the same reference frame as the 1st.  Using the list of logo locations, we can caluclate
   *  what rotation is required.
   */
    if (LIB_SPINMODELING_MULTI_LOGO)
    {
      time1LogoDescription = getLogoDescription(myLogoList, time1LogoID);
      time2LogoDescription = getLogoDescription(myLogoList, time2LogoID);
      point1Time2 = rotateLogoPoint(point1Time2,time1LogoDescription,time2LogoDescription);
      point2Time2 = rotateLogoPoint(point2Time2,time1LogoDescription,time2LogoDescription);
    }

  // Calculate the midpoint of the arc for a point #1 between 2 times.
  // Note, must normalize back to unity.
  point1MidPoint = vectorAdd(point1Time2,point1Time1);
  point1MidPoint = vectorScalarDivide(point1MidPoint,vectorMag(point1MidPoint));

  // Print the midpoint vector
  if (LIB_SPINMODELING_DEBUG)
    {
      printf("Mid1 = [%f,%f,%f]\n",point1MidPoint.x,point1MidPoint.y,point1MidPoint.z);
    }

  // Calculate the midpoint of the arc for a point #2 between 2 times.
  // Note, must normalize back to unity.
  point2MidPoint = vectorAdd(point2Time2,point2Time1);
  point2MidPoint = vectorScalarDivide(point2MidPoint,vectorMag(point2MidPoint));

  // Print the midpoint vector
  if (LIB_SPINMODELING_DEBUG)
    {
      printf("Mid2 = [%f,%f,%f]\n",point2MidPoint.x,point2MidPoint.y,point2MidPoint.z);
    }

  // Calculate the vector normal to the vectors for a point at time #1 and #2.
  point1Normal = vectorCrossProduct(point1Time1,point1Time2);
  point2Normal = vectorCrossProduct(point2Time1,point2Time2);

   // The great circle is defined by the circle that intersects the sphere
  // that is perpendicular to this vecor.
  point1GC = vectorCrossProduct(point1Normal,point1MidPoint);
  point2GC = vectorCrossProduct(point2Normal,point2MidPoint);

  // Print the great circle vectors.
  if (LIB_SPINMODELING_DEBUG)
    {
      printf("GC1 = [%f,%f,%f]\n",point1GC.x,point1GC.y,point1GC.z);
      printf("GC2 = [%f,%f,%f]\n",point2GC.x,point2GC.y,point2GC.z);
    }

  if (0)
    { /*   Old way of doing it   */
      // The interesction of the two great circles above defines the spin axis.
      // See document "Intersection of Great Circles on a Sphere"
      // The next few lines are used to calculate the intersection, i.e. spin axis.
      h = (point2GC.x*point1GC.z - point2GC.z*point1GC.x) / (point2GC.y*point1GC.x - point2GC.x*point1GC.y);
      g = (-h*point1GC.y-point1GC.z) / point1GC.x;
      k = sqrt(1/(g*g + h*h + 1));
      
      if (LIB_SPINMODELING_DEBUG)
	{
	  printf("h=%f,g=%f,k=%f\n",h,g,k);
	}  
      
      // Define spin axis
      spinAxis.x = g*k;
      spinAxis.y = h*k;
      spinAxis.z = k;
    }
  else /* We can just do cross products! */
    {
      if ( (vectorMag(point1GC)==0) & (vectorMag(point2GC)!=0) )
	{
	  if (LIB_SPINMODELING_DEBUG)
	    {
	      printf("Spin axis is @ point 1!\n");
	    }
	  spinAxis = point1Time1;
	}
      else if ( (vectorMag(point1GC)!=0) & (vectorMag(point2GC)==0) )
	{
	  if (LIB_SPINMODELING_DEBUG)
	    {
	      printf("Spin axis is @ point 2!\n");
	    }
	  spinAxis = point2Time1;
	}
      else if ( (vectorMag(point1GC)==0) & (vectorMag(point2GC)==0) )
	{
	  if (LIB_SPINMODELING_DEBUG)
	    {
	      printf("0 degrees spin!  Setting spin axis to 0,0,1\n");
	    }
	  spinAxis.x=0;
	  spinAxis.y=0;
	  spinAxis.z=1;
	}
      else
	{
	  spinAxis = vectorCrossProduct(point1GC,point2GC);
	  if (LIB_SPINMODELING_DEBUG)
	    {
	      printf("Mag of cross product = %e\n",vectorMag(spinAxis));
	    }
	  if (vectorMag(spinAxis) < 1e-14) // Numerically a problem.  Need to really find the intersection of the labels
	    {
	      spinAxis.x = 0;
	      spinAxis.y = 0;
	      spinAxis.z = 1;
	    }
	  spinAxis = vectorScalarDivide(spinAxis,vectorMag(spinAxis));
	  if (spinAxis.z < 0)
	    {
	      spinAxis.x=-spinAxis.x;
	      spinAxis.y=-spinAxis.y;
	      spinAxis.z=-spinAxis.z;
	    }
	}
    }
  
  // Calculate spherical coordiantes.
  spinAxisSpherical = vectorToSpherical(spinAxis);

  // Print spin axis info.
  if (LIB_SPINMODELING_DEBUG)
    {
      printf("spin theta = %f, spin phi = %f, spin r = %f\n",spinAxisSpherical.r,spinAxisSpherical.phi,spinAxisSpherical.theta);
    }
  
  // Calculate projection of points onto plane perpendicular to spin axis.
  point1Time2Prime = projectXY(rotateY(rotateZ(point1Time2,-spinAxisSpherical.theta),-spinAxisSpherical.phi));
  point1Time1Prime = projectXY(rotateY(rotateZ(point1Time1,-spinAxisSpherical.theta),-spinAxisSpherical.phi));
  point2Time1Prime = projectXY(rotateY(rotateZ(point2Time1,-spinAxisSpherical.theta),-spinAxisSpherical.phi));
  point2Time2Prime = projectXY(rotateY(rotateZ(point2Time2,-spinAxisSpherical.theta),-spinAxisSpherical.phi));  

  // Print projections.
  if (LIB_SPINMODELING_DEBUG)
    {
      printf("p1t1_p x=%f,y=%f,z=%f\n",point1Time1Prime.x,point1Time1Prime.y,point1Time1Prime.z);
      printf("p1t2_p x=%f,y=%f,z=%f\n",point1Time2Prime.x,point1Time2Prime.y,point1Time2Prime.z);  
    }  

  // Spin is calculate using a dot product and convert to degrees.
  myArg = vectorDotProduct(point1Time1Prime,point1Time2Prime)/vectorMag(point1Time1Prime)/vectorMag(point1Time2Prime);
  if (myArg > 1) // We check here because there can be some numerical errors that cause the argument to be > 1 (stationary images)
    {
      myArg = 1;
    }  
  spinMagInDegrees = 180/M_PI*acos(myArg);
  myArg = vectorDotProduct(point2Time1Prime,point2Time2Prime)/vectorMag(point2Time1Prime)/vectorMag(point2Time2Prime);
  if (myArg > 1)
    {
      myArg = 1;
    }
  spinMagInDegrees_point2 = 180/M_PI*acos(myArg);
  if (LIB_SPINMODELING_DEBUG)
    {
      printf("Spin angles of %f and %f found!\n",spinMagInDegrees, spinMagInDegrees_point2);
    }
  
  // It is possible that the spin was actually between 180 and 360 degrees, since the arcos doesn't know.
  // We'll define that we care about a right hand rotation from the first point @ time 1.
  // So if the cross product results in a negative z, then it's > 180 degrees!
  projectedCrossProduct = vectorCrossProduct(point1Time1Prime,point1Time2Prime);
  if (projectedCrossProduct.z < 0)
    {
      spinMagInDegrees = 360.0 - spinMagInDegrees;
      spinMagInDegrees_point2 = 360.0 - spinMagInDegrees_point2;
      if (LIB_SPINMODELING_DEBUG)
	{
	  printf("Spin is actually >180 degrees @ %f\n",spinMagInDegrees);
	}
    }

  // Spin axis defined as the "pitch" angle (angle between spin axis and YZ plane)
  // Positive angle is below ZX plane for RH golfer
  // Negative angle is above ZX plane for RH golfer
  // Vice versa for LH golfer.
  // In a broader sense, a positive spin axis means the ball curves to the right
  // and a negative spin axis means the ball curves to the left.
  yAxisUnitVector.x=0;
  yAxisUnitVector.y=1;
  yAxisUnitVector.z=0;
  spinAxisZXPitchInDegrees = 90.0 - 180.0/M_PI*acos(vectorDotProduct(spinAxis,yAxisUnitVector));
  if (handedness == LIB_SPINMODELING_RIGHTHANDED)
    {
      spinAxisInDegrees = -spinAxisZXPitchInDegrees;
    }
  else
    {
      spinAxisInDegrees = spinAxisZXPitchInDegrees;
    }

    // Define spin description struct.
  mySpinDescription.spinAxisInDegrees = spinAxisInDegrees;
  // Check handedness for sign change
  if (handedness == LIB_SPINMODELING_RIGHTHANDED)
    {
      mySpinDescription.spinInDegrees = spinMagInDegrees;
    }
  else
    {
      mySpinDescription.spinInDegrees = 360.0 - spinMagInDegrees;
    }

  // Calculate RPMs
  spinInRPMs = (mySpinDescription.spinInDegrees / 360.0) / (deltaTimeInSeconds / 60.0);
  mySpinDescription.spinInRPMs = spinInRPMs;

  // Print results.
  if (LIB_SPINMODELING_DEBUG)
    {
      printf("Spin axis = [%f,%f,%f]\n",spinAxis.x,spinAxis.y,spinAxis.z);
      printf("Spin angle = %f degs\n",spinMagInDegrees);
    }

  // Free logo list (dynamically allocated)
  freeLogoList(myLogoList);  

  return mySpinDescription;

}

/* --------------------------------------------------
   Initializes logo list.  Note this allocates memory.  The linked list MUST BE FREE'ED at the end.

   Input Arguments
   
    NONE

   Output Arguments

   myLogoList     - newly initialized logo list (this and next are both NULL, empty list)
-------------------------------------------------- */
 logoList * initLogoList()
 {
  logoList * myLogoList;
  myLogoList = (logoList *) malloc(sizeof(logoList));
  myLogoList->logoID = -1;
  myLogoList->thisLogo = NULL;
  myLogoList->nextLogo = NULL;
  return myLogoList;
 }

 /* --------------------------------------------------
   Adds a new logo to the logo list.  NOTE:  each node is dynamically allocated
   and MUST BE FREE'ed with freeLogoList.

   Input Arguments
   
   myLogoList   - head pointer to the head of the logo list
   newID        - (unique) ID for new logo to add.
   newRoll      - Relative roll for new logo
   newPitch     - Relative pitch for new logo
   newYaw       - Relative yaw for new logo

   Output Arguments

   NONE
-------------------------------------------------- */
 void addLogo(logoList * myLogoList, int newID, double newRollInDegrees, double newPitchInDegrees, double newYawInDegrees)
 {
  /*
   *  Checks for an empty logo list (when thisLogo is NULL)
   */
  if (myLogoList->thisLogo == NULL)
  {
    myLogoList->logoID = newID;
    myLogoList->thisLogo = (logoDescription *) malloc(sizeof(logoDescription));
    myLogoList->thisLogo->rollInDegrees = newRollInDegrees;
    myLogoList->thisLogo->pitchInDegrees = newPitchInDegrees;
    myLogoList->thisLogo->yawInDegrees = newYawInDegrees;
    myLogoList->nextLogo = NULL;
  }
  /*
   *  Otherwise, we simpley find the last node and add a new logo description.
   */
  else
  {
    while (myLogoList->nextLogo != NULL)
    {
      myLogoList = myLogoList->nextLogo;
    }
    myLogoList->nextLogo = (logoList *) malloc(sizeof(logoList));
    myLogoList->nextLogo->thisLogo = (logoDescription *) malloc(sizeof(logoDescription));
    myLogoList->nextLogo->logoID = newID;
    myLogoList->nextLogo->thisLogo->rollInDegrees = newRollInDegrees;
    myLogoList->nextLogo->thisLogo->pitchInDegrees = newPitchInDegrees;
    myLogoList->nextLogo->thisLogo->yawInDegrees = newYawInDegrees;
    myLogoList->nextLogo->nextLogo = NULL;
  }
 }

/* --------------------------------------------------
   Frees logo list as each node is dynamically allocated.

   Input Arguments
   
   head           - a pointer to the head of the logo list

   Output Arguments

   NONE
-------------------------------------------------- */
void freeLogoList(logoList * head)
{
  logoList * tmpNode;
     
  while (head != NULL)
  {
    free(head->thisLogo);
    tmpNode = head;
    head = head->nextLogo;
    free(tmpNode);
  }
}

/* --------------------------------------------------
   Gets logo description from logo list

   Input Arguments
   
   myLogoList           - a pointer to the head of the logo list
   logoID               - logo ID to find and return description

   Output Arguments

   logo description
-------------------------------------------------- */
logoDescription getLogoDescription(logoList * myLogoList, int logoID)
{
  while(myLogoList->logoID != logoID)
  {
    myLogoList = myLogoList->nextLogo;
  }
  return *(myLogoList->thisLogo);
}

/* --------------------------------------------------
   Rotates the input vector (myPoint), which is assumed to be from the logo at time 2 (time2LogoDescription)
   into the frame of reference of the logo at time 1 (time1LogoDescription).

   Input Arguments
   
   myPoint              - a point taking at time 2 from logo 2
   time1LogoDescription - logo description from time 1
   time2LogoDescription - logo description from time 2
   
   Output Arguments

   rotated myPoint into frame of refernece of logo 1

   Notes:  a logo description is defined by a roll, pitch, and yaw.  In order to get the logo into the correct orientation
   we start with the logo @ 1,0,0.  In other words, the front of the logo points in the 1,0,0 direction and the top is in
   the 0,0,1 direction.  To rotate this initial logo, we use the euler angles yaw, pitch, roll and calculate the transfer
   matrix (all +RHR rotations about the axes)

      M = Mz(yaw)*My(pitch)*Mx(roll)

    Rotating back from the logo location to the origin location is simply the reverse process

      M = Mx(-roll)*My(-pitch)*Mz(-yaw)

    So to rotate the 2nd logo into the frame of reference of the 1st, we calculate the transfer matrix to bring logo 2 to the origin and to logo 1 orientation

      M = Mz(yaw_logo1)*My(pitch_logo1)*Mx(roll_logo1)*Mx(-roll_logo2)*My(-pitch_logo2)*Mz(-yaw_logo2)
-------------------------------------------------- */
vector rotateLogoPoint(vector myPoint,logoDescription time1LogoDescription,logoDescription time2LogoDescription)
{
  double y1, p1, r1, y2, p2, r2;

  // Let's get some shorter variable names to make this cleaner looking
  y1 = time1LogoDescription.yawInDegrees * M_PI / 180.0;
  p1 = time1LogoDescription.pitchInDegrees * M_PI / 180.0;
  r1 = time1LogoDescription.rollInDegrees * M_PI / 180.0;

  y2 = time2LogoDescription.yawInDegrees * M_PI / 180.0;
  p2 = time2LogoDescription.pitchInDegrees * M_PI / 180.0;
  r2 = time2LogoDescription.rollInDegrees * M_PI / 180.0;

  return rotateZ(rotateY(rotateX(rotateX(rotateY(rotateZ(myPoint,-y2),-p2),-r2),r1),p1),y1);
}


