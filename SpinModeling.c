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
   Calcules the spin direction for a set of images and two points.
   This approach uses the two point approach.  Given two points
   observed in two images, the spin and spin axis can be calculated.

   Input Arguments

   point1Time1     - vector, location in XY space indicating the postion of a point #1 @ time #1
   point1Time2     - vector, location in XY space indicating the postion of a point #1 @ time #2
   ballCenterTime1 - vector, location in XY space indicating the center of the ball in time #1
   ballRadiusTime1 - double, radius of ball in image @ time #1
   point2Time1     - vector, location in XY space indicating the postion of a point #2 @ time #1
   point2Time2     - vector, location in XY space indicating the postion of a point #2 @ time #2
   ballCenterTime2 - vector, location in XY space indicating the center of the ball in time #2
   ballRadiusTime2 - double, radius of ball in image @ time #2

   Output Arguments

   spinDescription - return a spin description (spin, spin axis, etc.).  See header file.
   -------------------------------------------------- */

spinDescription calcSpinAxisAndSpin(vector point1Time1, vector point2Time1, vector ballCenterTime1, double ballRadiusTime1,
				    vector point1Time2, vector point2Time2, vector ballCenterTime2, double ballRadiusTime2,
				    double deltaTimeInSeconds)
{

  // Defines the midpoint on the arc connection points 1 and time at time 1, and at time 1
  vector time1Midpoint;
  vector time2Midpoint;

  // Defines a vector normal to the points at time #1 and time #2 (cross product)
  vector time1Normal;
  vector time2Normal;
  sphericalCoordinate time1Normal_spherical;
  sphericalCoordinate time2Normal_spherical;

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

  // Amount of spin calculated between the two times
  double spinMagInDegrees;
  double spinMagInDegrees_point2;

  // Spin axis in degrees
  double spinAxisInDegrees;
  // Spin in RPMs
  double spinInRPMs;

  // Output arguement.  See header file.
  spinDescription mySpinDescription;
  
  // Normalized all 4 points (2 points, 2 times) to the origin and unity.
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
  spinMagInDegrees = 180/M_PI*acos(vectorDotProduct(point1Time1Prime,point1Time2Prime)/vectorMag(point1Time1Prime)/vectorMag(point1Time2Prime));
  spinMagInDegrees_point2 = 180/M_PI*acos(vectorDotProduct(point2Time1Prime,point2Time2Prime)/vectorMag(point2Time1Prime)/vectorMag(point2Time2Prime));
  if (LIB_SPINMODELING_DEBUG)
    {
      printf("Spin angles of %f and %f found!\n",spinMagInDegrees, spinMagInDegrees_point2);
    }

  // Spin axis (for now) is only the phi component.  Convert to degreees.
  if (spinAxisSpherical.theta < 0)
    {
      spinAxisInDegrees = -spinAxisSpherical.phi*180/M_PI;
    }
    else
    {
      spinAxisInDegrees = spinAxisSpherical.phi*180/M_PI;
    }

  // Calculate RPMs
  spinInRPMs = (spinMagInDegrees / 360) / (deltaTimeInSeconds / 60);

  // Define spin description struct.
  mySpinDescription.spinAxisInDegrees = spinAxisInDegrees;
  mySpinDescription.spinInDegrees = spinMagInDegrees;
  mySpinDescription.spinInRPMs = spinInRPMs;

  // Print results.
  if (LIB_SPINMODELING_DEBUG)
    {
      printf("Spin axis = [%f,%f,%f]\n",spinAxis.x,spinAxis.y,spinAxis.z);
      printf("Spin angle = %f degs\n",spinMagInDegrees);
    }

  return mySpinDescription;

}
 



