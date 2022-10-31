
 
//definitions of motors
#include <AFMotor.h>
AF_DCMotor Left_Front_Motor(4);
AF_DCMotor Right_Front_Motor(3);
AF_DCMotor Left_Back_Motor(1);
AF_DCMotor Right_Back_Motor(2);
#include <Ultrasonic.h>
 
Ultrasonic ultrasonic_back(40,41),ultrasonic_left_back(38,39),ultrasonic_left_front(36,37),ultrasonic_front(34,35);
// definitions of ultrasonic sensors
 
#define Left 0 //left direction command
#define Right 1 //right direction command
#define Forward 2 //forward command
#define Back 3 //reverse command
#define minimum_limit 15 //width of the car (cm)
#define minimum_limit1 28 //length of the car (cm)
 
byte park_status = 0;
int signalpin= 21;
volatile int val;
 
int counter= 0;
int current_status = 0;
int previous_status = 0;
 
void say(int count)
{
for (int i = 0 ; i <= count; i+1)
{
val = digitalRead(signalpin);
if (val == LOW) {
 
current_status = 0;
}
else {
 
current_status = 1;
}
 
if(current_status != previous_status)
{
if(current_status == 1)
{
counter= counter+ 1;
Serial.println(counter);
i = i+1;
}
else
{
i = i ;
}
 
previous_status = current_status;
 
}
if (i == count)
{
 
Left_Front_Motor.run(RELEASE);
Right_Front_Motor.run(RELEASE);
Left_Back_Motor.run(RELEASE);
Right_Back_Motor.run(RELEASE);
 
}
 
}
 
}
 
void motor_pinSetup()
{
 
Left_Front_Motor.run(RELEASE);
Right_Front_Motor.run(RELEASE);
Left_Back_Motor.run(RELEASE);
Right_Back_Motor.run(RELEASE);
}
 
// Motion functions
void Robot_Movement(byte motor, byte spd)
{
if (motor == Forward)
{
Left_Front_Motor.setSpeed(spd);
Right_Front_Motor.setSpeed(spd);
Left_Back_Motor.setSpeed(spd);
Right_Back_Motor.setSpeed(spd);
Left_Front_Motor.run(FORWARD);
Right_Front_Motor.run(FORWARD);
Left_Back_Motor.run(FORWARD);
Right_Back_Motor.run(FORWARD);
 
}
if (motor == Back)
{
Left_Front_Motor.setSpeed(spd);
Right_Front_Motor.setSpeed(spd);
Left_Back_Motor.setSpeed(spd);
Right_Back_Motor.setSpeed(spd);
Left_Front_Motor.run(BACKWARD);
Right_Front_Motor.run(BACKWARD);
Left_Back_Motor.run(BACKWARD);
Right_Back_Motor.run(BACKWARD);
 
}
if (motor == Left)
{
Left_Front_Motor.setSpeed(spd);
Right_Front_Motor.setSpeed(spd);
Left_Back_Motor.setSpeed(spd);
Right_Back_Motor.setSpeed(spd);
Left_Front_Motor.run(BACKWARD);
Right_Front_Motor.run(FORWARD);
Left_Back_Motor.run(BACKWARD);
Right_Back_Motor.run(FORWARD);
 
}
 
if (motor == Right)
{
Left_Front_Motor.setSpeed(spd);
Right_Front_Motor.setSpeed(spd);
Left_Back_Motor.setSpeed(spd);
Right_Back_Motor.setSpeed(spd);
Left_Front_Motor.run(FORWARD);
Right_Front_Motor.run(BACKWARD);
Left_Back_Motor.run(FORWARD);
Right_Back_Motor.run(BACKWARD);
 
}
 
}
 
void Robot_Stop()
{
Left_Front_Motor.run(RELEASE);
Right_Front_Motor.run(RELEASE);
Left_Back_Motor.run(RELEASE);
Right_Back_Motor.run(RELEASE);
}
 
// Search for parking space
bool Park_Location_Control()
{
 
long front_Sensor = ultrasonic_front.Ranging(CM);
long right_Sensor = ultrasonic_left_front.Ranging(CM);
long right_back_Sensor =ultrasonic_left_back.Ranging(CM);
 
if( (right_Sensor <= minimum_limit)&&(right_back_Sensor <= minimum_limit)&&(park_status == 0))
{
Robot_Movement(Forward, 100);
park_status = 1; Serial.println(park_status);
}
 
if((right_Sensor > minimum_limit)&&(right_Sensor < minimum_limit1)&&(right_back_Sensor > minimum_limit)&&(right_back_Sensor < minimum_limit1)&&(park_status == 1))
{
Robot_Movement(Forward, 100);
park_status = 2;Serial.println(park_status);
}
 
if((right_Sensor >= minimum_limit1)&&(right_back_Sensor >= minimum_limit1)&&(park_status == 1))
{
/* Vertical Parking Decision */
Robot_Stop() ;
delay(500);
park_status = 10;Serial.println(park_status);
}
 
if((right_Sensor <= minimum_limit)&&(right_back_Sensor <= minimum_limit)&&(park_status == 2))
{
/* Parallel Parking Decision */
park_status = 3; Serial.println(park_status);
}
 
return park_status;
}
 
void Park_find()
{
Park_Location_Control();
if(park_status == 3 )
{
Robot_Stop();Serial.println(park_status);
delay(400);
park_status = 4;
}
if(park_status == 4 )
{
 
Robot_Movement(Back,120);
say(18);
Robot_Stop();Serial.println(park_status);
delay(500);
Robot_Movement(Right,150);
say(9);
Robot_Stop();
delay(500);
park_status = 5;
}
if(park_status == 5)
{
 
Robot_Movement(Back,120);
long back_Sensor = ultrasonic_back.Ranging(CM);Serial.println(back_Sensor);
 
if(back_Sensor>0 && back_Sensor <= 13)
{
Robot_Stop();
delay(400);
park_status = 6;
}
return back_Sensor;
}
 
if(park_status == 6)
{
Robot_Movement(Left,150);
long right_Sensor = ultrasonic_left_front.Ranging(CM); Serial.println(right_Sensor);
long right_back_Sensor = ultrasonic_left_back.Ranging(CM); Serial.println(right_back_Sensor);
 
if(right_Sensor == right_back_Sensor)
{
Robot_Stop();
park_status = 7;
}
 
return right_Sensor,right_back_Sensor;
}
if(park_status == 7)
{
long front_Sensor = ultrasonic_front.Ranging(CM);
 
if(front_Sensor<=6)
{
Robot_Stop();
park_status = 8;
}
else
{
Robot_Movement(Forward,100);
}
return front_Sensor;
}
if (park_status ==10)
{
 
Robot_Movement(Left,180);
say(14);
Robot_Stop();
delay(500);
park_status = 7;
 
}
 
}
 
void setup()
{
Serial.begin(9600);
attachInterrupt(5, say, CHANGE);
pinMode (signalpin, INPUT) ;
 
motor_pinSetup();
}
 
void loop()
{
Park_find();
}