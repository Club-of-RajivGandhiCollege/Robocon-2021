#include "universal_gripper.h"
#include<Servo.h>

#define MinElbowGripperAngle  0      
#define MaxElbowGripperAngle 90      
#define MinShoulderGripperAngle 0
#define MaxShoulderGripperAngle 120

Servo ElbowGripperservo, ShoulderGripperservo;
int shouldergrippingAngle,elbowgrippingAngle,vac_pin;





UGripper::UGripper(int shoulder_servo_pin,int elbow_servo_pin,int vacuum_pin){
  //instantiate anything here 
  ElbowGripperservo.attach(elbow_servo_pin);
  ShoulderGripperservo.attach(shoulder_servo_pin);
  vac_pin = vacuum_pin;

}





void UGripper::servoSetInitial(){
  Serial.println("Servo angles set");
    ElbowGripperservo.write(MinElbowGripperAngle);
    ShoulderGripperservo.write(MinShoulderGripperAngle);
    
    // initializing our angles to its minimum constraint
    shouldergrippingAngle = MinShoulderGripperAngle;
    elbowgrippingAngle = MinElbowGripperAngle;
}

void UGripper::elbowServoInc(){
    /*** checking if my shoulder angle is lesser than threshold or no **/
  if (elbowgrippingAngle < MaxElbowGripperAngle ) 
  {
    ElbowGripperservo.write(elbowgrippingAngle);
    elbowgrippingAngle++;
    Serial.print("Elbow Increase: ");
    Serial.println(elbowgrippingAngle);
  }
  else {
    Serial.print("max limit reached: ");
    Serial.println(elbowgrippingAngle+1);
  }
}


void UGripper::elbowServoDec()
{ 
  /*** checking if my shoulder angle is greater than threshold or no **/
  if (elbowgrippingAngle > MinElbowGripperAngle )
      {
      ElbowGripperservo.write(elbowgrippingAngle);
      elbowgrippingAngle--;
      Serial.print("Elbow Decrease: ");
      Serial.println(elbowgrippingAngle);
      }
  else {
    Serial.print("max limit reached: ");
    Serial.println(elbowgrippingAngle-1);
  }
}


void UGripper::shoulderServoInc()
{ 
  /*** checking if my shoulder angle is lesser than threshold or no **/
    if (shouldergrippingAngle < MaxShoulderGripperAngle ) 
    {
        ShoulderGripperservo.write(shouldergrippingAngle);
        shouldergrippingAngle++;
        Serial.print("Shouler Increase: ");
        Serial.println(shouldergrippingAngle);
    }
    else {
        Serial.print("max limit reached: ");
        Serial.println(shouldergrippingAngle+1);
    }
}

void UGripper::shoulderServoDec()
{ 
  /*** checking if my shoulder angle is greater than threshold or no **/
    if (shouldergrippingAngle > MinShoulderGripperAngle ){
        ShoulderGripperservo.write(shouldergrippingAngle);
        shouldergrippingAngle--;
        Serial.print("Shouler Decrease: ");
        Serial.println(shouldergrippingAngle);
    }
    else {
        Serial.print("max limit reached: ");
        Serial.println(shouldergrippingAngle-1);
    }
}


void UGripper::releaseVacuum(){
  Serial.println("Release Vacuum");
  digitalWrite(vac_pin,LOW);
}

void UGripper::createVacuum(){
  Serial.println("Create vacuum");
  digitalWrite(vac_pin,HIGH);
}
