#include "L298.h"

//right stick, left-right (horizontal) - turning
#define CH1_MID 1530
#define CH1_HALFRANGE 400L

//right stick, up-down (vertical) - drive
#define CH2_MID 1545
#define CH2_HALFRANGE 380L

//switch A, right
#define CH3_MID 1200

//Receiver PINS:
//connect to channel 1 on receiver
byte CH1_PIN = 11;
//connect to channel 2 on receiver
byte CH2_PIN = 10;
//connect to channel 3 on receiver //TODO implement switch
byte CH3_PIN = 9;
// //connect to channel 4 on receiver //TODO implement left stick
// byte CH4_PIN = 6;

//Motors PINS:
byte ENA = 5;
byte IN1 = 7;
byte IN2 = 8;

byte IN3 = 4;
byte IN4 = 2;
byte ENB = 3;


//driving
int32_t value_speed = 0;
int32_t prevvalue_speed = 0;
int32_t speed = 0;
int32_t speed_turned = 0;

int32_t value_turn = 0;
int32_t prevvalue_turn = 0;
int32_t turn = 0;

//switch A, right and left stick left-right, horizontal
bool switch_down;
int switch_value = 0;


L298 motor;

void setup() {
    // put your setup code here, to run once:
    pinMode(CH1_PIN, INPUT); //turn
    pinMode(CH2_PIN, INPUT); //speed
    pinMode(CH3_PIN, INPUT); //switch A
    // pinMode(CH3_PIN, INPUT); //TODO uncomment for switch and left stick
    // pinMode(CH4_PIN, INPUT);
    Serial.begin(9600);

    //motors control initialization, change the pins maybe
    motor.setLeftMotorPins(ENA, IN1, IN2);
    motor.setRightMotorPins(ENB, IN3, IN4);
    motor.setup();

    switch_value = pulseIn(CH3_PIN, HIGH);
    if(switch_value < CH3_MID)
    {
        switch_down = false;
    }else
    {
        switch_down = true;
    }

    prevvalue_speed = pulseIn(CH2_PIN, HIGH);
    prevvalue_turn = pulseIn(CH1_PIN, HIGH);
}

void loop() {
    // put your main code here, to run repeatedly:

    //SWITCH A, RIGHT:
    switch_value = pulseIn(CH3_PIN, HIGH);
    if((switch_value < CH3_MID) && (switch_down == true))
    {
        Serial.println("switch turned up");
        //TODO implement what happen when switch turned up
    }else if((switch_value > CH3_MID) && (switch_down == false))
    {
        Serial.println("switch turned up");
        //TODO implement what happen when switch turned up
    }


    //MOTORS:
    value_speed = pulseIn(CH2_PIN, HIGH);
    value_turn = pulseIn(CH1_PIN, HIGH);
    Serial.println(value_speed);

    //start if of jittery prevention
    // if(((value_speed - prevvalue_speed) < 20) && ((value_speed - prevvalue_speed) > -20))
    //     || (((value_turn - prevvalue_turn) < 20) && ((value_turn - prevvalue_turn) > -20))
    // {

    //asssumes channel 2 to PIN 3
    //rescaling of speed
    value_speed = value_speed - CH2_MID;
    Serial.println(value_speed);
    speed = -(value_speed*255) / CH2_HALFRANGE;
    Serial.println(speed);

    //limiting so its not out of range
    if(speed > 255)
    {
        speed = 255;
    }else if(speed < -255)
    {
        speed = -255;
    }

    //rescaling of turn, differenc ebetween the motors.
    //max to the one side, the other motor has opposite speed
    value_turn = value_turn - CH1_MID;
    turn = (value_turn * speed * 2) / CH1_HALFRANGE;

    //limiting so its not out of range
    if(turn > 510) //max speed * 2
    {
        turn = 510;
    }else if(turn < -510)
    {
        turn = -510;
    }
    //set turn to 0 for too small values
    if((turn < 40)&&(turn > -40))
    {
        turn = 0;
    }

    if(turn >0) //left motor changed, turn left
    {
        speed_turned = speed - turn;
        motor.setRightMotorSpeed(speed);
        motor.setLeftMotorSpeed(speed_turned);
    }else//right motor changed, turn right
    {
        speed_turned = speed + turn;
        motor.setRightMotorSpeed(speed_turned);
        motor.setLeftMotorSpeed(speed);
    }
    prevvalue_speed = value_speed;
    prevvalue_turn = value_turn;

    // }
    //end if of jittery prevention

    //delay so the motors can actually move
    delay(5);
}
