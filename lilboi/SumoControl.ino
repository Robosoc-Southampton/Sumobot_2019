#include "L298.h"
#include <Servo.h>
#include <Stepper.h>

// REMEMBER THAT SERVOS NEED TO BE AT 90 WHEN PUT INSIDE THE ROBOT

//right stick, left-right (horizontal) - turning
#define CH1_MID 1530
#define CH1_HALFRANGE 390L

//right stick, up-down (vertical) - drive
#define CH2_MID 1545
#define CH2_HALFRANGE 380L

//switch A, right
#define CH3_MID 1200

//left stick, left-right (horizontal) - flippity front
#define CH4_LEFT 1400
#define CH4_RIGHT 1540

//Servos movement restriction (for left one)
#define FLIP_RIGHTUP 45
#define FLIP_UP 1
#define FLIP_DOWN -1


//number of step between sponge up and down
#define SPONGE_DIFF 10
#define SPONGE_SPEED 100


//Servos:
Servo tankservo;

int tankservo_pos = 0;

int tankservo_pos_next = 0;




//Receiver PINS:
//connect to channel 1 on receiver GREY
byte CH1_PIN = 11;
//connect to channel 2 on receiver YELLOW
byte CH2_PIN = 6;
//connect to channel 3 on receiver ORANGE
byte CH3_PIN = 5;
//connect to channel 4 on receiver GREEN
byte CH4_PIN = 3;

//Motors PINS:
byte ENA = 13;
byte IN1 = 12;
byte IN2 = 8;

byte IN3 = 7;
byte IN4 = 4;
byte ENB = 2;

//Motors:
L298 motor;


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
int leftstick_value = 0;



void setup() {
    // put your setup code here, to run once:
    pinMode(CH1_PIN, INPUT); //turn
    pinMode(CH2_PIN, INPUT); //speed
    pinMode(CH3_PIN, INPUT); //switch A
    pinMode(CH4_PIN, INPUT); //left stick - sponge
    Serial.begin(9600);


    //Servo initialization: PIN connection for servo
    tankservo.attach(9);


    tankservo_pos = tankservo.read();

    //starting vlaues of servos
    down_tankservo_pos = tankservo_pos;
    down_rightservo_pos = rightservo_pos;

    //motors control initialization, change the pins maybe
    motor.setLeftMotorPins(ENA, IN1, IN2);
    motor.setRightMotorPins(ENB, IN3, IN4);
    motor.setup();

    switch_value = pulseIn(CH3_PIN, HIGH);
    if(switch_value < CH3_MID)//switch is up
    {
        switch_down = false;

    }else //switch is down
    {
        switch_down = true;
        
    }

    //previous values from pilot of speed and turn
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

    }else if((switch_value > CH3_MID) && (switch_down == false))
    {
        Serial.println("switch turned up");

    }

    //LEFT STICK, sideways
    leftstick_value = pulseIn(CH4_PIN);

    if(leftstick_value < CH4_LEFT)
    {
        tankservo_pos = tankservo + 5;
        if(tankservo > 180) 
        {
            tankservo_pos = 180;
        }
        tankservo.write(tankservo_pos);
    }else if(leftstick_value > CH4_RIGHT)
    {
        tankservo_pos = tankservo - 5;
        if(tankservo < 0) 
        {
            tankservo_pos = 0;
        }
        tankservo.write(tankservo_pos);
    }

    //MOTORS:
    value_speed = pulseIn(CH2_PIN, HIGH);
    value_turn = pulseIn(CH1_PIN, HIGH);
    Serial.println(value_speed);

    //start if of jittery prevention
    // if(((value_speed - prevvalue_speed) < 20) && ((value_speed - prevvalue_speed) > -20))
    //     || (((value_turn - prevvalue_turn) < 20) && ((value_turn - prevvalue_turn) > -20))
    // {
 
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