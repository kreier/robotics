// updated original sketch from 2018 to run 2024/01/29
// with my Galaxy Note 20 and BlueDuino 

#include <AFMotor.h> // download from subdirectory 'library' here and install zip file
#include <SoftwareSerial.h>  // updated to match T300 with key assignment

#define LED_PIN 13
 
AF_DCMotor motor1(1, MOTOR12_64KHZ); // create motor #1, 64KHz pwm
AF_DCMotor motor4(4, MOTOR12_64KHZ); // create motor #2, 64KHz pwm
SoftwareSerial BTSerial(A0, 3);   // RX | TX  -  pin 2 creates errors on my motor shield, analog pin is fine ...
char BTinput = '0';
int speed = 200;
byte react_to_zero = 1;
byte received_speed_change = 0;

void setup() {
  motor1.setSpeed(100);
  motor4.setSpeed(100);     // set the speed to 200/255
  BTSerial.begin(38400);    // its not 9600 - this drove me nuts! HC-10 default speed?
  Serial.begin(57600);      // just to check while programming
  Serial.print( BTinput );
}
 
void loop() {
  if (BTSerial.available()) 
  {
    BTinput = BTSerial.read();
    if (BTinput == 'w')      // forward  - A
    {
      motor1.run(FORWARD);
      motor4.run(FORWARD);
    }
    if (BTinput == 's')      // backward - C
    {
      motor1.run(BACKWARD);
      motor4.run(BACKWARD);
    }
    if (BTinput == 'd')      // left     - D
    {
      motor1.run(FORWARD);
      motor4.run(BACKWARD);
    }
    if (BTinput == 'a')      // right    - B
    {
      motor1.run(BACKWARD);
      motor4.run(FORWARD);
    }      
    if (BTinput == 'b')      // that's the "X" key      - G
    {
      motor1.run(RELEASE);      // stopped
      motor4.run(RELEASE);
    }
    if (BTinput == 'x')  // faster - plus 10  - circle  - F
    {
      speed = speed + 10;
      received_speed_change = 1;
    }
    if (BTinput == 'c')  // slower - minus 10 - square  - H
    {
      speed = speed - 10;
      received_speed_change = 1;
    } 
    if (BTinput == 'v')  // Toggle response to zero
    {
      if (react_to_zero == 1) {
        react_to_zero = 0;
      } 
      else {
        react_to_zero = 1;
      }
    }
    if (BTinput == '0')  // stop signal received
    {
      if ((received_speed_change == 0) && (react_to_zero == 1)) {
        motor1.run(RELEASE);      // stopped
        motor4.run(RELEASE);
      }
    }

    if( speed > 255 ) speed = 255;
    if( speed < 0   ) speed = 0;
    received_speed_change = 0;
    motor1.setSpeed(speed);
    motor4.setSpeed(speed);
    Serial.print(millis());
    Serial.print(" recieved: ");
    Serial.print( BTinput );
    Serial.print(" speed: ");
    Serial.println( speed );
  }
}
