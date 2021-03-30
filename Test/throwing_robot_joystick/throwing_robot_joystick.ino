#include <math.h>

//Direction pins
int w1 = 43;
int w2 = 45;
int w3 = 41;
int w4 = 47;

//Magnitude (PWM) pins
int w1PWM = 4;
int w2PWM = 3;
int w3PWM = 7;
int w4PWM = 2;

//Joystick analog in pins
int joystickX = A6;
int joystickY = A7;

double DAMP_FACTOR = 2;

void setup()
{
	Serial.begin(9600);
	pinMode(w1, OUTPUT);
	pinMode(w2, OUTPUT);
	pinMode(w3, OUTPUT);
	pinMode(w4, OUTPUT);
	pinMode(w1PWM, OUTPUT);
	pinMode(w2PWM, OUTPUT);
	pinMode(w3PWM, OUTPUT);
	pinMode(w4PWM, OUTPUT);

	pinMode(joystickX, INPUT);
	pinMode(joystickY, INPUT);
}


void loop()
{
	int xr = analogRead(joystickX);
	int yr = analogRead(joystickY);

  Serial.print("xr: ");
  Serial.print(xr);
  Serial.print("\t");
  Serial.print("yr: ");
  Serial.print(yr);

	drive_motors(127, 127, xr/4, yr/4);  
}


void drive_motors(int xl, int yl, int xr, int yr)
{
  xr = xr - 127;
  yr = 127 - yr;


	double f1, f2;
	f1 = 1.414 * xr + 1.414 * yr;
	f2 = -1.414 * xr + 1.414 * yr;

  f1=f1/DAMP_FACTOR;
  f2=f2/DAMP_FACTOR;

  Serial.print("\t");
  Serial.print("f1 = ");
  Serial.print(f1);
  Serial.print("\t");
  Serial.print("f2 = ");
  Serial.println(f2);
  
	if (f1 > 0)
	{
		wheel2forward();
		wheel4forward();
	}
	else
	{
		wheel2backward();
		wheel4backward();
	}

	if (f2 > 0)
	{
		wheel1forward();
		wheel3forward();
	}
	else
	{
		wheel1backward();
		wheel3backward();
	}

  f1 = abs(f1);
  f2 = abs(f2);

  if(f1>30){
    analogWrite(w2PWM, f1);
    analogWrite(w4PWM, f1);
  }
  else{
    analogWrite(w2PWM, 0);
    analogWrite(w4PWM, 0);
  }

  if(f2>30){
    analogWrite(w1PWM, f2);
    analogWrite(w3PWM, f2);
  }
  else{
    analogWrite(w1PWM, 0);
    analogWrite(w3PWM, 0);

  }


	if (xl < 100) //100 is a minimum limit value for xl to activate rotation
	{
		xl = 100 - xl;
		map(xl, 0, 100, 0, 255);
		rotateLeft(xl);
	}
	if (xl > 155)
	{
		xl = xl - 155;
		map(xl, 0, 100, 0, 255);
		rotateRight(xl);
	}

}

//	2--1
//	|  |
//	3--4

void wheel1forward()
{
	digitalWrite(w1, LOW); //HIGH-LOW is positive direction & LOW-HIGH is negative direction
}

void wheel1backward()
{
	digitalWrite(w1, HIGH); //HIGH-LOW is positive direction & LOW-HIGH is negative direction
}

void wheel2forward()
{
	digitalWrite(w2, HIGH); //HIGH-LOW is positive direction & LOW-HIGH is negative direction
}

void wheel2backward()
{
	digitalWrite(w2, LOW); //HIGH-LOW is positive direction & LOW-HIGH is negative direction
}

void wheel3forward()
{
	digitalWrite(w3, HIGH); //HIGH-LOW is positive direction & LOW-HIGH is negative direction
}

void wheel3backward()
{
	digitalWrite(w3, LOW); //HIGH-LOW is positive direction & LOW-HIGH is negative direction
}

void wheel4forward()
{
	digitalWrite(w4, LOW); //HIGH-LOW is positive direction & LOW-HIGH is negative direction
}

void wheel4backward()
{
	digitalWrite(w4, HIGH); //HIGH-LOW is positive direction & LOW-HIGH is negative direction
}

void rotateLeft(int mag)
{
	analogWrite(w1PWM, mag);
	analogWrite(w2PWM, mag);
	analogWrite(w3PWM, mag);
	analogWrite(w4PWM, mag);
	wheel1forward();
	wheel2backward();
	wheel3backward();
	wheel4forward();
}

void rotateRight(int mag)
{
	analogWrite(w1PWM, mag);
	analogWrite(w2PWM, mag);
	analogWrite(w3PWM, mag);
	analogWrite(w4PWM, mag);
	wheel2forward();
	wheel1backward();
	wheel4backward();
	wheel3forward();
}
