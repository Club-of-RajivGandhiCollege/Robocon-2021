#include <PS3BT.h>
#include <usbhub.h>
#include <math.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
PS3BT PS3(&Btd); // This will just create the instance
//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

bool printTemperature, printAngle;

//******************Locomotion*********************
//Direction pins
int w1 = 47;
int w2 = 45;
int w3 = 41;
int w4 = 43;

//Magnitude (PWM) pins
int w1PWM = 2;
int w2PWM = 3;
int w3PWM = 7;
int w4PWM = 4;
//**************************************************

//**************Gripping Arrow from rack*********************
int arrowGripperEnable = 8;
int arrowGripperDirection1 = 40;
int arrowGripperDirection2 = 41;
boolean arrowGripperPressed = false;
long arrowGripperCloseDelay = 500;
//************************************************************

double DAMP_FACTOR = 2;

void setup()
{
	Serial.begin(115200);
#if !defined(__MIPSEL__)
	while (!Serial)
		; // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
	if (Usb.Init() == -1)
	{
		Serial.print(F("\r\nOSC did not start"));
		while (1)
			; //halt
	}
	Serial.print(F("\r\nPS3 Bluetooth Library Started"));
}

void drive_motors(int xl, int yl, int xr, int yr)
{
	xr = xr - 127;
	yr = yr - 127;

	Serial.print("\t\txr: ");
	Serial.print(xr);
	Serial.print("\t");
	Serial.print("yr: ");
	Serial.print(yr);

	double f1, f2;
	f1 = 1.414 * xr + 1.414 * yr;
	f2 = -1.414 * xr + 1.414 * yr;

	f1 = f1 / DAMP_FACTOR;
	f2 = f2 / DAMP_FACTOR;

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

	if (f1 > 30)
	{
		analogWrite(w2PWM, f1);
		analogWrite(w4PWM, f1);
	}
	else
	{
		analogWrite(w2PWM, 0);
		analogWrite(w4PWM, 0);
	}

	if (f2 > 30)
	{
		analogWrite(w1PWM, f2);
		analogWrite(w3PWM, f2);
	}
	else
	{
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

void loop()
{
	Usb.Task();

	unsigned int xl, yl, xr, yr;

	if (PS3.PS3Connected || PS3.PS3NavigationConnected)
	{
		if (PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 || PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 || PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117)
		{
			xl = PS3.getAnalogHat(LeftHatX);
			yl = PS3.getAnalogHat(LeftHatY);

			if (PS3.PS3Connected)
			{ // The Navigation controller only have one joystick
				xr = PS3.getAnalogHat(RightHatX);
				yr = PS3.getAnalogHat(RightHatY);
			}

			drive_motors(xl, yl, xr, yr);
		}
		else
		{
			drive_motors(xl, yl, 127, 127);
		}

		// Analog button values can be read from almost all buttons
		if (PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2))
		{
			Serial.print(F("\r\nL2: "));
			Serial.print(PS3.getAnalogButton(L2));
			if (PS3.PS3Connected)
			{
				Serial.print(F("\tR2: "));
				Serial.print(PS3.getAnalogButton(R2));
			}
		}

		if (PS3.getButtonClick(PS))
		{
			Serial.print(F("\r\nPS"));
			PS3.disconnect();
		}
		else
		{
			if (PS3.getButtonClick(TRIANGLE))
			{
				Serial.print(F("\r\nTraingle"));
				PS3.setRumbleOn(RumbleLow);
			}
			if (PS3.getButtonClick(CIRCLE))
			{
				Serial.print(F("\r\nCircle"));
				PS3.setRumbleOn(RumbleHigh);
			}
			if (PS3.getButtonClick(CROSS))
				Serial.print(F("\r\nCross"));
			if (PS3.getButtonClick(SQUARE))
				Serial.print(F("\r\nSquare"));

			if (PS3.getButtonClick(UP))
			{
				Serial.print(F("\r\nUp"));
				if (PS3.PS3Connected)
				{
					PS3.setLedOff();
					PS3.setLedOn(LED4);
				}
			}
			if (PS3.getButtonClick(RIGHT))
			{
				Serial.print(F("\r\nRight"));
				if (PS3.PS3Connected)
				{
					PS3.setLedOff();
					PS3.setLedOn(LED1);
				}
			}
			if (PS3.getButtonClick(DOWN))
			{
				Serial.print(F("\r\nDown"));
				if (PS3.PS3Connected)
				{
					PS3.setLedOff();
					PS3.setLedOn(LED2);
				}
			}
			if (PS3.getButtonClick(LEFT))
			{
				Serial.print(F("\r\nLeft"));

				//Here the arrow grip commond is given, pressing this again will do the opposite
				gripArrowsFromRack();
				if (PS3.PS3Connected)
				{
					PS3.setLedOff();
					PS3.setLedOn(LED3);
				}
			}

			if (PS3.getButtonClick(L1))
				Serial.print(F("\r\nL1"));
			if (PS3.getButtonClick(L3))
				Serial.print(F("\r\nL3"));
			if (PS3.getButtonClick(R1))
				Serial.print(F("\r\nR1"));
			if (PS3.getButtonClick(R3))
				Serial.print(F("\r\nR3"));

			if (PS3.getButtonClick(SELECT))
			{
				Serial.print(F("\r\nSelect - "));
				PS3.printStatusString();
			}
			if (PS3.getButtonClick(START))
			{
				Serial.print(F("\r\nStart"));
				printAngle = !printAngle;
			}
		}
#if 0 // Set this to 1 in order to see the angle of the controller
    if (printAngle) {
      Serial.print(F("\r\nPitch: "));
      Serial.print(PS3.getAngle(Pitch));
      Serial.print(F("\tRoll: "));
      Serial.print(PS3.getAngle(Roll));
    }
#endif
	}
#if 0 // Set this to 1 in order to enable support for the Playstation Move controller
  else if (PS3.PS3MoveConnected) {
    if (PS3.getAnalogButton(T)) {
      Serial.print(F("\r\nT: "));
      Serial.print(PS3.getAnalogButton(T));
    }
    if (PS3.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS3.disconnect();
    }
    else {
      if (PS3.getButtonClick(SELECT)) {
        Serial.print(F("\r\nSelect"));
        printTemperature = !printTemperature;
      }
      if (PS3.getButtonClick(START)) {
        Serial.print(F("\r\nStart"));
        printAngle = !printAngle;
      }
      if (PS3.getButtonClick(TRIANGLE)) {
        Serial.print(F("\r\nTriangle"));
        PS3.moveSetBulb(Red);
      }
      if (PS3.getButtonClick(CIRCLE)) {
        Serial.print(F("\r\nCircle"));
        PS3.moveSetBulb(Green);
      }
      if (PS3.getButtonClick(SQUARE)) {
        Serial.print(F("\r\nSquare"));
        PS3.moveSetBulb(Blue);
      }
      if (PS3.getButtonClick(CROSS)) {
        Serial.print(F("\r\nCross"));
        PS3.moveSetBulb(Yellow);
      }
      if (PS3.getButtonClick(MOVE)) {
        PS3.moveSetBulb(Off);
        Serial.print(F("\r\nMove"));
        Serial.print(F(" - "));
        PS3.printStatusString();
      }
    }
    if (printAngle) {
      Serial.print(F("\r\nPitch: "));
      Serial.print(PS3.getAngle(Pitch));
      Serial.print(F("\tRoll: "));
      Serial.print(PS3.getAngle(Roll));
    }
    else if (printTemperature) {
      Serial.print(F("\r\nTemperature: "));
      Serial.print(PS3.getTemperature());
    }
  }
#endif
}

//**************Gripping Arrow from rack*********************

void arrowGripperClose()
{
	digitalWrite(arrowGripperDirection1, HIGH);
	digitalWrite(arrowGripperDirection2, LOW);
	delay(arrowGripperCloseDelay);
	arrowGripperStop();
}

void arrowGripperOpen()
{
	digitalWrite(arrowGripperDirection1, LOW);
	digitalWrite(arrowGripperDirection2, HIGH);
	delay(arrowGripperCloseDelay);
	arrowGripperStop();
}

void arrowGripperStop()
{
	digitalWrite(arrowGripperDirection1, LOW);
	digitalWrite(arrowGripperDirection2, LOW);
}

void gripArrowsFromRack()
{
	arrowGripperPressed = !arrowGripperPressed;
	if (arrowGripperPressed = true)
	{
		arrowGripperClose();
	}
	else
	{
		arrowGripperOpen();
	}
}

//************************************************************