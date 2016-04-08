/*                 Version 2.3.2                 */
#include <stdio.h>
#include "SpinModeling.h"

#define RIGHTHANDED 1
#define LEFTHANDED -1

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

  int testNum;
  
  // 3 - Sample #3, Khan Adjusted test case
  // 12 - Sample #12, Wil data
  // 16 - Sample #16, Wil data
  // -1 - 0 spin
  // -2 - 
  // -3 - 
  // -4 - 

  testNum = 3;

  if (testNum == 3)
    {
      // Set the center of the golf ball in the xy coordiantes of the image.
      // (0,0) is in the lower left for the first image.
      ballCenterTime1.x=83.0;
      ballCenterTime1.y=186.0;
      ballCenterTime1.z=0.0;
      
      // Set the center of the golf ball in the xy coordiantes of the image.
      // (0,0) is in the lower left for the second image.
      ballCenterTime2.x=407.0;
      ballCenterTime2.y=276.0;
      ballCenterTime2.z=0.0;

      // Set the ball radius for both images.
      ballRadiusTime1 = 83.0/2.0;
      ballRadiusTime2 = 93.0/2.0;

      // Set the known point #1 in the image in the first image.
      // Note:  ONLY SET THE X AND Y POINTS. SET Z = 0;
      point1Time1.x = 54.0;
      point1Time1.y = 192.0;
      point1Time1.z = 0.0;
      
      // Set the known point #2 in the image in the first image.
      // Note:  ONLY SET THE X AND Y POINTS. SET Z = 0;
      point2Time1.x = 76.0;
      point2Time1.y = 192.0;
      point2Time1.z = 0.0;
      
      // Set the known point #1 in the image in the second image.
      // Note:  ONLY SET THE X AND Y POINTS. SET Z = 0;
      // Note, here for test I am adding the center, but you only
      // have to define the vector from the image.  The vectorAdd is not needed.
      point1Time2.x = 414.0;
      point1Time2.y = 253.0;
      point1Time2.z = 0.0;
      
      // Set the known point #2 in the image in the second image.
      // Note:  ONLY SET THE X AND Y POINTS. SET Z = 0;
      // Note, here for test I am adding the center, but you only
      // have to define the vector from the image.  The vectorAdd is not needed.
      point2Time2.x = 391.0;
      point2Time2.y = 290.0;
      point2Time2.z = 0.0;
    }
  else if (testNum == 12)
    {
      // Set the center of the golf ball in the xy coordiantes of the image.
      // (0,0) is in the lower left for the first image.
      ballCenterTime1.x=79.0;
      ballCenterTime1.y=137.0;
      ballCenterTime1.z=0.0;
      
      // Set the center of the golf ball in the xy coordiantes of the image.
      // (0,0) is in the lower left for the second image.
      ballCenterTime2.x=409;
      ballCenterTime2.y=232;
      ballCenterTime2.z=0.0;

      // Set the ball radius for both images.
      ballRadiusTime1 = 83.0/2.0;
      ballRadiusTime2 = 96.0/2.0;

      // Set the known point #1 in the image in the first image.
      // Note:  ONLY SET THE X AND Y POINTS. SET Z = 0;
      point1Time1.x = 52.0;
      point1Time1.y = 133.0;
      point1Time1.z = 0.0;
      
      // Set the known point #2 in the image in the first image.
      // Note:  ONLY SET THE X AND Y POINTS. SET Z = 0;
      point2Time1.x = 65.0;
      point2Time1.y = 133.0;
      point2Time1.z = 0.0;
      
      // Set the known point #1 in the image in the second image.
      // Note:  ONLY SET THE X AND Y POINTS. SET Z = 0;
      // Note, here for test I am adding the center, but you only
      // have to define the vector from the image.  The vectorAdd is not needed.
      point1Time2.x = 407.0;
      point1Time2.y = 220.0;
      point1Time2.z = 0.0;
      
      // Set the known point #2 in the image in the second image.
      // Note:  ONLY SET THE X AND Y POINTS. SET Z = 0;
      // Note, here for test I am adding the center, but you only
      // have to define the vector from the image.  The vectorAdd is not needed.
      point2Time2.x = 405.0;
      point2Time2.y = 257.0;
      point2Time2.z = 0.0;
    }
else if (testNum == 16)
    {
      // Set the center of the golf ball in the xy coordiantes of the image.
      // (0,0) is in the lower left for the first image.
      ballCenterTime1.x = 55.0;
      ballCenterTime1.y = 172.0;
      ballCenterTime1.z = 0.0;
      
      // Set the center of the golf ball in the xy coordiantes of the image.
      // (0,0) is in the lower left for the second image.
      ballCenterTime2.x = 390.0;
      ballCenterTime2.y = 278.0;
      ballCenterTime2.z = 0.0;

      // Set the ball radius for both images.
      ballRadiusTime1 = 86.0 / 2;
      ballRadiusTime2 = 101.0 / 2;

      // Set the known point #1 in the image in the first image.
      // Note:  ONLY SET THE X AND Y POINTS. SET Z = 0;
      point1Time1.x = 21.0;
      point1Time1.y = 165.0;
      point1Time1.z = 0.0;
      
      // Set the known point #2 in the image in the first image.
      // Note:  ONLY SET THE X AND Y POINTS. SET Z = 0;
      point2Time1.x = 53.0;
      point2Time1.y = 172.0;
      point2Time1.z = 0.0;
      
      // Set the known point #1 in the image in the second image.
      // Note:  ONLY SET THE X AND Y POINTS. SET Z = 0;
      // Note, here for test I am adding the center, but you only
      // have to define the vector from the image.  The vectorAdd is not needed.
      point1Time2.x = 379.0;
      point1Time2.y = 244.0;
      point1Time2.z = 0.0;
      
      // Set the known point #2 in the image in the second image.
      // Note:  ONLY SET THE X AND Y POINTS. SET Z = 0;
      // Note, here for test I am adding the center, but you only
      // have to define the vector from the image.  The vectorAdd is not needed.
      point2Time2.x = 372.0;
      point2Time2.y = 291.0;
      point2Time2.z = 0.0;
    }
else if (testNum == -1)
    {
      ballCenterTime1.x = 100.0;
      ballCenterTime1.y = 100.0;
      ballCenterTime1.z = 0.0;
      
      ballCenterTime2.x = 100.0;
      ballCenterTime2.y = 100.0;
      ballCenterTime2.z = 0.0;

      ballRadiusTime1 = 100.0 / 2;
      ballRadiusTime2 = 100.0 / 2;

      point1Time1.x = 95.0;
      point1Time1.y = 100.0;
      point1Time1.z = 0.0;
      
      point2Time1.x = 105.0;
      point2Time1.y = 100.0;
      point2Time1.z = 0.0;
      
      point1Time2.x = 95.0;
      point1Time2.y = 100.0;
      point1Time2.z = 0.0;
      
      point2Time2.x = 105.0;
      point2Time2.y = 100.0;
      point2Time2.z = 0.0;
    }
 else if (testNum == -2)
    {
      ballCenterTime1.x = 100.0;
      ballCenterTime1.y = 100.0;
      ballCenterTime1.z = 0.0;
      
      ballCenterTime2.x = 100.0;
      ballCenterTime2.y = 100.0;
      ballCenterTime2.z = 0.0;

      ballRadiusTime1 = 100.0 / 2;
      ballRadiusTime2 = 100.0 / 2;

      point1Time1.x = 95.0;
      point1Time1.y = 100.0;
      point1Time1.z = 0.0;
      
      point2Time1.x = 105.0;
      point2Time1.y = 100.0;
      point2Time1.z = 0.0;
      
      point1Time2.x = 100.0;
      point1Time2.y = 95.0;
      point1Time2.z = 0.0;
      
      point2Time2.x = 100.0;
      point2Time2.y = 105.0;
      point2Time2.z = 0.0;
    }
else if (testNum == -3)
    {
      ballCenterTime1.x = 100.0;
      ballCenterTime1.y = 100.0;
      ballCenterTime1.z = 0.0;
      
      ballCenterTime2.x = 100.0;
      ballCenterTime2.y = 100.0;
      ballCenterTime2.z = 0.0;

      ballRadiusTime1 = 100.0 / 2;
      ballRadiusTime2 = 100.0 / 2;

      point1Time1.x = 95.0;
      point1Time1.y = 100.0;
      point1Time1.z = 0.0;
      
      point2Time1.x = 105.0;
      point2Time1.y = 100.0;
      point2Time1.z = 0.0;
      
      point1Time2.x = 105.0;
      point1Time2.y = 100.0;
      point1Time2.z = 0.0;
      
      point2Time2.x = 95.0;
      point2Time2.y = 100.0;
      point2Time2.z = 0.0;
    }
else if (testNum == -4)
    {
      ballCenterTime1.x = 100.0;
      ballCenterTime1.y = 100.0;
      ballCenterTime1.z = 0.0;
      
      ballCenterTime2.x = 100.0;
      ballCenterTime2.y = 100.0;
      ballCenterTime2.z = 0.0;

      ballRadiusTime1 = 100.0 / 2;
      ballRadiusTime2 = 100.0 / 2;

      point1Time1.x = 95.0;
      point1Time1.y = 100.0;
      point1Time1.z = 0.0;
      
      point2Time1.x = 105.0;
      point2Time1.y = 100.0;
      point2Time1.z = 0.0;
      
      point1Time2.x = 100.0;
      point1Time2.y = 105.0;
      point1Time2.z = 0.0;
      
      point2Time2.x = 100.0;
      point2Time2.y = 95.0;
      point2Time2.z = 0.0;
    }
  
  deltaTimeInSeconds = 0.001;

  // Pass in the parameters as described above.
  // A struct is returned with two paramters:  spinInDegrees and spinAxisInDegrees;
  // Both describe the amount of spin and spin axis between the two images.
  mySpin = calcSpinAxisAndSpin(point1Time1, point2Time1, ballCenterTime1, ballRadiusTime1, point1Time2, point2Time2, ballCenterTime2, ballRadiusTime2,deltaTimeInSeconds,RIGHTHANDED);

  printf("I have a spin of %f degrees about a spin axis of %f degrees:  %f RPMs\n",mySpin.spinInDegrees,mySpin.spinAxisInDegrees,mySpin.spinInRPMs);
  printf("\nShould say\n\n");

  if (testNum == 3)
    {
      printf("I have a spin of 121.602557 degrees about a spin axis of -1.211893 degrees:  20267.092783 RPMs\n");
    }
  else if (testNum == 12)
    {
      printf("I have a spin of 93.110362 degrees about a spin axis of -6.575197 degrees:  15518.393731 RPMs\n");
    }
  else if(testNum == 16)
    {
      printf("I have a spin of 86.303097 degrees about a spin axis of 16.816952 degrees:  14383.849520 RPMs\n");
    }
  else if(testNum == -1)
    {
      printf("I have a spin of 0.000000 degrees about a spin axis of -0.000000 degrees:  0.000000 RPMs\n");
    }
  else if(testNum == -2)
    {
      printf("I have a spin of 237.725058 degrees about a spin axis of -0.000000 degrees:  39620.843060 RPMs\n");
    }
  else if(testNum == -3)
    {
      printf("I have a spin of 279.836844 degrees about a spin axis of -0.000000 degrees:  46639.473948 RPMs\n");
    }
  else if(testNum == -4)
   {
     printf("I have a spin of 325.250682 degrees about a spin axis of -0.000000 degrees:  54208.447026 RPMs\n");
    }
  printf("Finished program!\n"); 

  return 0;
}
