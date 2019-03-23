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

#define CH2_MAXVAL 2000L
#define CH2_MINVAL 1000L

//switch A, right
#define CH3_MID 1400

//left stick, left-right (horizontal) - flippity front
#define CH4_LEFT 1400
#define CH4_RIGHT 1540

//Servos movement restriction (for left one)
#define FLIP_RIGHTUP 45
#define FLIP_UP 1
#define FLIP_DOWN -1


//number of step between sponge up and down
#define SPONGE_DIFF 10
#define SPONGE_SPEED 200

//Stepper motor PINS (sponge pusher)
int stepperrevolution = 20;//100;

byte IN1_STEPPER = A0;
byte IN2_STEPPER = A1;

byte IN3_STEPPER = A2;
byte IN4_STEPPER = A3;


//Servos:
Servo leftservo;
Servo rightservo;

int leftservo_pos = 0;
int rightservo_pos = 0;

int leftservo_pos_next = 0;
int rightservo_pos_next = 0;

int down_leftservo_pos = 0;
int down_rightservo_pos = 0;



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

//Stepper motor - sponge on bottom
Stepper sponge(stepperrevolution, IN1_STEPPER, IN2_STEPPER,
                                    IN3_STEPPER, IN4_STEPPER);



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
int flip_value = 0;

//synchronous (almost) movement of front servos (flip thing)
void flip_front(int dir_up) //1 if up, direction_up
{
    // leftservo_pos_next = leftservo_pos - FLIP_RIGHTUP * dir_up;
    leftservo_pos_next = leftservo_pos + FLIP_RIGHTUP * dir_up;
    rightservo_pos_next = rightservo_pos + FLIP_RIGHTUP * dir_up;
    
    //move by one each one until satisfieds
    while((leftservo_pos != leftservo_pos_next) && (rightservo_pos != rightservo_pos_next))
    {
        // leftservo_pos -= dir_up;
        leftservo_pos += dir_up;
        rightservo_pos += dir_up;

        leftservo.write(leftservo_pos);
        rightservo.write(rightservo_pos);
        delay(15); 
    }
}

void setup() {
    // put your setup code here, to run once:
    pinMode(CH1_PIN, INPUT); //turn
    pinMode(CH2_PIN, INPUT); //speed
    pinMode(CH3_PIN, INPUT); //switch A
    pinMode(CH4_PIN, INPUT); //left stick - sponge
    Serial.begin(9600);

    //Stepper motor speed TODO check if correct
    sponge.setSpeed(SPONGE_SPEED);

    //Servo initialization: PIN connection for servo
    leftservo.attach(9);
    leftservo.write(90);

    rightservo.attach(10);
    rightservo.write(90);

    leftservo_pos = leftservo.read();
    rightservo_pos = rightservo.read();

    //starting vlaues of servos
    down_leftservo_pos = leftservo_pos;
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

    switch_value = pulseIn(CH3_PIN, HIGH, 25000);

    if (switch_value < CH3_MID - 100) {
        Serial.println("switch turned up");
        sponge.step(-SPONGE_DIFF);
    }
    else if (switch_value > CH3_MID + 100) {
        Serial.println("switch turned up");
        sponge.step(SPONGE_DIFF);
    }

    //Left stick change
    flip_value = pulseIn(CH4_PIN, HIGH, 25000);

    if (flip_value <= CH4_LEFT && rightservo_pos <= down_rightservo_pos) {
        Serial.println("flip up");
        flip_front(FLIP_UP);
    }
    else if (flip_value >= CH4_RIGHT && rightservo_pos >= down_rightservo_pos) {
        Serial.println("flip down");
        flip_front(FLIP_DOWN);
    }

    value_speed = pulseIn(CH2_PIN, HIGH, 25000);
    value_turn = pulseIn(CH1_PIN, HIGH, 25000);

    //safety measure if receiver disconnected
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

        motor.setLeftMotorSpeed(-(int) (left_speed_scale * 255));
        motor.setRightMotorSpeed(-(int) (right_speed_scale * 255));
    }
    else {
        motor.setMotorSpeed(0);
    }
}
