#include <stdio.h>
#include "SpinModeling.h"

int main()
{
  printf("Starting program . . . \n");  

  vector point1Time1;
  vector point1Time2;
  vector point2Time1;
  vector point2Time2;

  vector ballCenterTime1;
  vector ballCenterTime2;
  double ballRadiusTime1;
  double ballRadiusTime2;

  double deltaTimeInSeconds;

  spinDescription mySpin;

  // Set the center of the golf ball in the xy coordiantes of the image.
  // (0,0) is in the lower left for the first image.
  ballCenterTime1.x=274;//1;
  ballCenterTime1.y=173;//2;
  ballCenterTime1.z=0;

  // Set the center of the golf ball in the xy coordiantes of the image.
  // (0,0) is in the lower left for the second image.
  ballCenterTime2.x=271;//2;
  ballCenterTime2.y=172;//4;
  ballCenterTime2.z=0;

  // Set the ball radius for both images.
  ballRadiusTime1 = 71;//1;
  ballRadiusTime2 = 76;//1;

  // Set the known point #1 in the image in the first image.
  // Note:  ONLY SET THE X AND Y POINTS. SET Z = 0;
  // Note, here for test I am adding the center, but you only
  // have to define the vector from the image.  The vectorAdd is not needed.
  point1Time1.x = 243;//-0.25;     
  point1Time1.y = 177;//0.25;
  point1Time1.z = 0;//0.935414346693485;
  //point1Time1 = vectorAdd(point1Time1,ballCenterTime1);

  // Set the known point #2 in the image in the first image.
  // Note:  ONLY SET THE X AND Y POINTS. SET Z = 0;
  // Note, here for test I am adding the center, but you only
  // have to define the vector from the image.  The vectorAdd is not needed.
  point2Time1.x = 305;//-.25;
  point2Time1.y = 170;//-.25;
  point2Time1.z = 0;//0.935414346693485;
  //point2Time1 = vectorAdd(point2Time1,ballCenterTime1);

  // Set the known point #1 in the image in the second image.
  // Note:  ONLY SET THE X AND Y POINTS. SET Z = 0;
  // Note, here for test I am adding the center, but you only
  // have to define the vector from the image.  The vectorAdd is not needed.
  point1Time2.x = 242;//-0.187199408023956;     
  point1Time2.y = 152;//0.0119597071948924;
  point1Time2.z = 0;//0.982249126769423;
  //point1Time2 = vectorAdd(point1Time2,ballCenterTime2);

  // Set the known point #2 in the image in the second image.
  // Note:  ONLY SET THE X AND Y POINTS. SET Z = 0;
  // Note, here for test I am adding the center, but you only
  // have to define the vector from the image.  The vectorAdd is not needed.
  point2Time2.x = 304;//0.0424674725176793;
  point2Time2.y = 183;//-0.425239700829063;
  point2Time2.z = 0;//0.90408390684536;
  //point2Time2 = vectorAdd(point2Time2,ballCenterTime2);

  deltaTimeInSeconds = 0.001;

  // Pass in the parameters as described above.
  // A struct is returned with two paramters:  spinInDegrees and spinAxisInDegrees;
  // Both describe the amount of spin and spin axis between the two images.
  mySpin = calcSpinAxisAndSpin(point1Time1, point2Time1, ballCenterTime1, ballRadiusTime1, point1Time2, point2Time2, ballCenterTime2, ballRadiusTime2,deltaTimeInSeconds);

  printf("I have a spin of %f degrees about a spin axis of %f degrees:  %f RPMs\n",mySpin.spinInDegrees,mySpin.spinAxisInDegrees,mySpin.spinInRPMs);
  printf("\nShould say\n\n");
  printf("I have a spin of 22.915517 degrees about a spin axis of 29.029118 degrees:  3819.252890 RPMs\n");
  printf("Finished program!\n"); 

  return 0;
}
