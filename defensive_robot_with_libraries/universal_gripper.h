#ifndef tl
#define tl

#if (ARDUINO>=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class UGripper{
  public:
    //constructor
    UGripper(int shoulder_servo_pin,int elbow_servo_pin,int vacuum_pin);    //We can pass pin number here

    //Methods
    void servoSetInitial();
    void shoulderServoInc();
    void shoulderServoDec();
    void elbowServoInc();
    void elbowServoDec();

    void releaseVacuum();
    void createVacuum();

  private:
  
};





#endif
