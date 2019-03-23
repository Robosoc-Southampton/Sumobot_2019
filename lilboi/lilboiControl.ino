#include "L298.h"
#include <Servo.h>


// REMEMBER THAT SERVOS NEED TO BE AT 90 WHEN PUT INSIDE THE ROBOT

//right stick, left-right (horizontal) - turning
#define CH1_MID 1530
#define CH1_HALFRANGE 390L

//right stick, up-down (vertical) - drive
#define CH2_MID 1545
#define CH2_HALFRANGE 380L

#define CH2_MAXVAL 2000L
#define CH2_MINVAL 1000L

//switch A, right
#define CH3_MID 1200

//left stick, left-right (horizontal) - flippity front
#define CH4_LEFT 1400
#define CH4_RIGHT 1540

//Servos:
// Servo tankservo;

// int tankservo_pos = 0;

// int tankservo_pos_next = 0;

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

bool motordisable = false;



void setup() {
    // put your setup code here, to run once:
    pinMode(CH1_PIN, INPUT); //turn
    pinMode(CH2_PIN, INPUT); //speed
    pinMode(CH3_PIN, INPUT); //switch A
    pinMode(CH4_PIN, INPUT); //left stick - sponge
    Serial.begin(9600);


    // //Servo initialization: PIN connection for servo
    // tankservo.attach(9);
    // for (int pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    //     // in steps of 1 degree
    //     tankservo.write(pos);              // tell servo to go to position in variable 'pos'
    //     delay(15);                       // waits 15ms for the servo to reach the position
    // }
    // for (int pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    //     tankservo.write(pos);              // tell servo to go to position in variable 'pos'
    //     delay(15);                       // waits 15ms for the servo to reach the position
    // }
    // // tankservo.write(0);
    // delay(15);


    // tankservo_pos = tankservo.read();

    // //starting values of servos
    // down_tankservo_pos = tankservo_pos;
    // down_rightservo_pos = rightservo_pos;

    //motors control initialization, change the pins maybe
    motor.setLeftMotorPins(ENA, IN1, IN2);
    motor.setRightMotorPins(ENB, IN3, IN4);
    motor.setup();

    //previous values from pilot of speed and turn
    prevvalue_speed = pulseIn(CH2_PIN, HIGH);
    prevvalue_turn = pulseIn(CH1_PIN, HIGH);
}

void loop() {
    // put your main code here, to run repeatedly:

    //SWITCH A, RIGHT:
    // switch_value = pulseIn(CH3_PIN, HIGH);
    // if((switch_value < CH3_MID) && (switch_down == true))
    // {
    //     motor.setMotorSpeed(0);
    //     motordisable = true;

    // }else if((switch_value > CH3_MID) && (switch_down == false))
    // {
    //     motor.setMotorSpeed(0);
    //     motordisable = true;

    // }

    // //LEFT STICK, sideways
    // leftstick_value = pulseIn(CH4_PIN, HIGH);

    // if(leftstick_value < CH4_LEFT)
    // {
    //     for(int i = 0; i<=20; i++)
    //     {
    //         tankservo_pos = tankservo_pos + 1;
    //         if(tankservo_pos > 180) 
    //         {
    //             tankservo_pos = 180;
    //         }
    //         tankservo.write(tankservo_pos);
    //         delay(10);
    //     }
    // }else if(leftstick_value > CH4_RIGHT)
    // {
    //     for(int i = 0; i<=20; i++)
    //     {
    //         tankservo_pos = tankservo_pos - 1;
    //         if(tankservo_pos < 0) 
    //         {
    //             tankservo_pos = 0;
    //         }
    //         tankservo.write(tankservo_pos);
    //         delay(10);
    //     }
    // }

    //MOTORS:
    value_speed = pulseIn(CH2_PIN, HIGH, 25000);
    value_turn = pulseIn(CH1_PIN, HIGH, 25000);

    Serial.println(value_speed);

    if (value_speed <= CH2_MAXVAL && value_speed >= CH2_MINVAL) {
        float left_speed_scale_fwd = (float) (value_speed - CH2_MID) / (float) CH2_HALFRANGE;
        float left_speed_scale_trn = -(float) (value_turn - CH1_MID) * 0.5 / (float) CH1_HALFRANGE;

        if (left_speed_scale_fwd < 0.1) left_speed_scale_trn = -left_speed_scale_trn;

        float left_speed_scale = left_speed_scale_trn + left_speed_scale_fwd;

        if (left_speed_scale < -1) left_speed_scale = -1;
        if (left_speed_scale > 1) left_speed_scale = 1;

        float right_speed_scale_fwd = (float) (value_speed - CH2_MID) / CH2_HALFRANGE;
        float right_speed_scale_trn = (float) (value_turn - CH1_MID) * 0.5 / CH1_HALFRANGE;

        if (right_speed_scale_fwd < 0.1) right_speed_scale_trn = -right_speed_scale_trn;

        float right_speed_scale = right_speed_scale_trn + right_speed_scale_fwd;

        if (right_speed_scale < -1) right_speed_scale = -1;
        if (right_speed_scale > 1) right_speed_scale = 1;

        motor.setLeftMotorSpeed((int) (left_speed_scale * 255));
        motor.setRightMotorSpeed((int) (right_speed_scale * 255));
    }
    else {
        motor.setMotorSpeed(0);
    }
    
    prevvalue_speed = value_speed;
    prevvalue_turn = value_turn;
}
