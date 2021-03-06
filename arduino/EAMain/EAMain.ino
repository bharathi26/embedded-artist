/***************************************************************************
 * Duino-Pi Firmware
 ***************************************************************************
 *wolfgang spahn 26-okt-2014
 *
 *
 * Duino-Pi Protokol
 * The signal beginns with 4 diffrent characters (S,M,A,D) for stepper motor,
 * gear motors, analog- and digital output and the number of the device.
 * Followed by an other character for the attribut and the value.
 *
 * structure:
 * character, number, character, number, …. return (“\n”)
 *
 * Servo:
 * S = servo + number (1,2,3,…)
 * P = position + angle (0 - 180)
 * Example: S2P180 (servo: 2, possition = 180)
 * 
 * Motor:
 * M = motor + number (1,2,3,…)
 * D = direction + number (0,1) (0 = clockwise, 1 = counterclockwise)
 * S = speed + number (0-255)
 * Example: M1D0S150 (motor: 1, direction: clockwise, speed = 150)
 * 
 * Analog
 * A = analog + number (1,2,3,…)
 * V = value + number (0 - 255)
 * Example: A4V0 (analog: 4, value = 0)
 *
 * Digital
 * D = digital + number (1,2,3,…)
 *  S = switch + number (0,1)
 *  or
 *  F = flash + milliseconds (1000 = 1 sec)
 * Example: D2S1 (digital: 2, on),  D4F100 (digital: 4, flash = every 100 millisec)
 *
 * Preset
 * P = preset + number(1,2,3,...)
 * Preset 0 = turn off everything
 * Preset 1 = turn on everything
 *
 ***************************************************************************/

#include <Servo.h>

Servo Servo1;  // create servo object to control a servo 
Servo Servo2;

int Servo1Possition = 0;    // variable to store the servo position 
int Servo2Possition = 0; 

int Motor1Pin = 6;
int Motor2Pin = 5;
int Motor1Dir1Pin= 8;
int Motor1Dir2Pin= 7;
int Motor2Dir1Pin= 4;
int Motor2Dir2Pin= 2;
int Motor1Speed = 0;
int Motor2Speed = 0;

int Analog1Pin = 11;
int Analog2Pin = 3;
int Analog1Value = 0;
int Analog2Value = 0;

int Digital1Pin = A0; // Red
int Digital2Pin = A1; // Green
int Digital3Pin = A2; // Blue
int Digital4Pin = A3; // IR
int Digital1Value = 0;
int Digital2Value = 0;
int Digital3Value = 0;
int Digital4Value = 0;
int Flash1 = LOW;
long Flash1Time = 0;
int Flash2 = LOW;
long Flash2Time = 0;
int Flash3 = LOW;
long Flash3Time = 0;
int Flash4 = LOW;
long Flash4Time = 0;
long FlashInterval = 100;

char CommandTyp1;
int Command1;
char CommandTyp2;
int Command2;
char CommandTyp3;
int Command3;

unsigned long CurrentTime = millis();

void setup() 
{ 
  Serial.begin(9600);
  Serial.println("open serial --->");
  Servo1.attach(9);  // attaches the servo on pin 9 to the servo object 
  Servo2.attach(10);
  pinMode(Motor1Pin, OUTPUT);
  pinMode(Motor2Pin, OUTPUT);
  pinMode(Motor1Dir1Pin, OUTPUT);
  pinMode(Motor1Dir2Pin, OUTPUT);
  pinMode(Motor2Dir1Pin, OUTPUT);
  pinMode(Motor2Dir2Pin, OUTPUT);

  pinMode(Analog1Pin, OUTPUT);
  pinMode(Analog2Pin, OUTPUT);

  pinMode(Digital1Pin, OUTPUT);
  pinMode(Digital2Pin, OUTPUT);
  pinMode(Digital3Pin, OUTPUT);
  pinMode(Digital4Pin, OUTPUT);
} 


void loop() 
{ 
  DataControl();
  Flash();
  // Serial.println("open serial --->");
}


void DataControl(){
  // if there's any serial available, read it:
  while (Serial.available() > 0) {
    // look for the next valid integer in the incoming serial stream:
    CommandTyp1 = Serial.read(); 
    if (CommandTyp1 == '\n') Serial.println("No-Code --->");
    else{
      Command1 = Serial.parseInt(); 
      // parse S commands:
      if (CommandTyp1 =='S'){    // S = servo
        CommandTyp2 = Serial.read(); 
        Command2 = Serial.parseInt();
        ServoSwitch();
      }
      // parse M commands:
      if (CommandTyp1 =='M'){   // M = motor
        CommandTyp2 = Serial.read(); 
        Command2 = Serial.parseInt();
        CommandTyp3 = Serial.read(); 
        Command3 = Serial.parseInt();
        MotorSwitch();
      }
      // parse A commands:
      if (CommandTyp1 =='A'){    // A = analog
        CommandTyp2 = Serial.read(); 
        Command2 = Serial.parseInt();
        AnalogSwitch();
      }
      // parse D commands:
      if (CommandTyp1 =='D'){    // D = digital
        CommandTyp2 = Serial.read(); 
        Command2 = Serial.parseInt();
        DigitalSwitch();
      }
      // parse P commands:
      if (CommandTyp1 =='P'){    // P = preset
        PresetSwitch();
      }
    }
    Flash();
  }
}

/*************************************************************************************/
// switching the servos
void ServoSwitch(){
  if (Command1==1){
    Servo1Possition = Command2;
    Servo1.write(Servo1Possition);
  }
  else if (Command1==2){
    Servo2Possition = Command2;
    Servo2.write(Servo2Possition);
  }
}

/*************************************************************************************/
// switching the motor ports
void MotorSwitch(){
  if (Command1==1){
    Motor1Speed = Command3;
    if (Command2 == 0){
      digitalWrite(Motor1Dir1Pin, LOW);
      digitalWrite(Motor1Dir2Pin, HIGH);
    }
    else if (Command2 == 1){
      digitalWrite(Motor1Dir1Pin, HIGH);
      digitalWrite(Motor1Dir2Pin, LOW);
    }
    analogWrite(Motor1Pin, Motor1Speed);
  }
  else if (Command1==2){
    Motor2Speed = Command3;
    if (Command2 == 0){
      digitalWrite(Motor2Dir1Pin, LOW);
      digitalWrite(Motor2Dir2Pin, HIGH);
    }
    else if (Command2 == 1){
      digitalWrite(Motor2Dir1Pin, HIGH);
      digitalWrite(Motor2Dir2Pin, LOW);
    }
    analogWrite(Motor2Pin, Motor2Speed);
  }

}

/*************************************************************************************/
// switching the analog ports
void AnalogSwitch(){
  if (Command1==1){
    Analog1Value = Command2;
    analogWrite(Analog1Pin, Analog1Value);
  }
  else if (Command1==2){
    Analog2Value = Command2;
    analogWrite(Analog2Pin, Analog2Value);
  }
}

/*************************************************************************************/
// switching the digital ports
void DigitalSwitch(){
  if (CommandTyp2=='S'){ // Switch On or OFF
    if (Command1==1){
      digitalWrite(Digital1Pin, Command2);
    }
    if (Command1==2){
      digitalWrite(Digital2Pin, Command2);
    }
    if (Command1==3){
      digitalWrite(Digital3Pin, Command2);
    }
    if (Command1==4){
      digitalWrite(Digital4Pin, Command2);
    }
  }
  else if (CommandTyp2=='F'){
    if (Command2==1){
      if (Command1==1){
        Flash1Time = millis();
        Flash1 = HIGH;
        digitalWrite(Digital1Pin, 1);
      }
      if (Command1==2){
        Flash2Time = millis();
        Flash2 = HIGH;
        digitalWrite(Digital2Pin, 1);
      }
      if (Command1==3){
        Flash3Time = millis();
        Flash3 = HIGH;
        digitalWrite(Digital3Pin, 1);
      }
      if (Command1==4){
        Flash4Time = millis();
        Flash4 = HIGH;
        digitalWrite(Digital4Pin, 1);
      } 

    }
  }
}

/*************************************************************************************/
// flashing the digital pins

void Flash(){
  if (Flash1 == HIGH){
    CurrentTime = millis();
    if(CurrentTime - Flash1Time > FlashInterval) {
      digitalWrite(Digital1Pin, 0);
      Flash1 = LOW;
    }
  }
  if (Flash2 == HIGH){
    CurrentTime = millis();
    if(CurrentTime - Flash2Time > FlashInterval) {
      digitalWrite(Digital2Pin, 0);
      Flash2 = LOW;
    }
  }
  if (Flash3 == HIGH){
    CurrentTime = millis();
    if(CurrentTime - Flash3Time > FlashInterval) {
      digitalWrite(Digital3Pin, 0);
      Flash3 = LOW;
    }
  }
  if (Flash4 == HIGH){
    CurrentTime = millis();
    if(CurrentTime - Flash4Time > FlashInterval) {
      digitalWrite(Digital4Pin, 0);
      Flash4 = LOW;
    }
  }
}

/*************************************************************************************/
// switching the preset
void PresetSwitch(){
  if (Command1==0){ // turn off everything
    Servo1Possition = 90;
    Servo2Possition = 90;
    Motor1Speed = 0;
    digitalWrite(Motor1Dir1Pin, LOW);
    digitalWrite(Motor1Dir2Pin, LOW);
    Motor2Speed = 0;
    digitalWrite(Motor2Dir1Pin, LOW);
    digitalWrite(Motor2Dir2Pin, LOW);
    Analog1Value = 0;
    Analog2Value = 0;
    Digital1Value = LOW;
    Digital2Value = LOW;
    digitalWrite(Digital1Pin, 0);
    digitalWrite(Digital2Pin, 0);
    digitalWrite(Digital3Pin, 0);
    digitalWrite(Digital4Pin, 0);
  }
  else if (Command1==1){  // turn on everything
    Servo1Possition = 180;
    Servo2Possition = 180;
    Motor1Speed = 255;
    digitalWrite(Motor1Dir1Pin, HIGH);
    digitalWrite(Motor1Dir2Pin, LOW);
    Motor2Speed = 255;
    digitalWrite(Motor2Dir1Pin, HIGH);
    digitalWrite(Motor2Dir2Pin, LOW);
    Analog1Value = 255;
    Analog2Value = 255;
    Digital1Value = HIGH;
    Digital2Value = HIGH;
    digitalWrite(Digital1Pin, 1);
    digitalWrite(Digital2Pin, 1);
    digitalWrite(Digital3Pin, 1);
    digitalWrite(Digital4Pin, 1);

  }
  Servo1.write(Servo1Possition);
  Servo2.write(Servo2Possition);
  analogWrite(Motor1Pin, Motor1Speed);
  analogWrite(Motor2Pin, Motor2Speed);
  analogWrite(Analog1Pin, Analog1Value);
  analogWrite(Analog2Pin, Analog2Value);
  digitalWrite(Digital1Pin, Digital1Value);
  digitalWrite(Digital2Pin, Digital2Value);
}










