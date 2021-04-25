# Robocon-2021

##Defensive Robot

To use custom library for universal gripper which includes two servos and a vacuum pump.

Note: Most of the constants are declared inside universal_gripper.cpp. So in case of any updations, change those inside cpp file.

Step 1. Include the library
```
      #include "universal_gripper.h" 
```
      
      Note: universal_gripper.h, universal_gripper.cpp and actual code file must be in same folder.
      
      if universal_gripper.h, universal_gripper.cpp files are in default lib folder of arduino then use,
 ```
      #include<universal_gripper.h>
 ```     
      
Step 2. Instantiate the class UGripper as,
```
        UGripper instance_name(pin for shouder servo, pin for elbow servo, pin for vacuum);
```        

##methods

Note: 1)The methods must be called as instance_name.method_name();
      2) These methods do not recieve any arguments.
```      
servoSetInitial();  ->Sets the servos in initial angle
shoulderServoInc(); ->increases shoulder servo angle
shoulderServoDec(); ->decreases shoulder servo angle
elbowServoInc(); ->increases elbow servo angle
elbowServoDec(); ->decreases elbow servo angle
releaseVacuum(); ->stops the vacuum motor to release the vacuum
createVacuum();  ->starts the vacuum motor to create the vacuum
```
