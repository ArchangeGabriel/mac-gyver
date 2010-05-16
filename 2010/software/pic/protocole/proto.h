// proto.h / Definition du protocole //
//modif pour le spam

#ifndef PROTO_H_
#define PROTO_H_

#define MACGYVERID   0xa5a5
#define BOOTLOADERID 0x0001
#define MOTORID      0x0002
#define INOUTID      0x0003

#define SETPWM     0x80
#define CODERS     0x90

#define MSKSENS0   0x01
#define MSKSENS1   0x02
#define MSKCODER   0xf0

//////////////////////////////////////
#define ISDEAD     0xf0
#define DEBUGS     0xa0
//////////////////////////////////////

#define MOTORS     0x01
#define ANALOG     0x02
#define INITAN     0x03
#define SERVOS     0x04
#define DIGITS     0x05
#define SETFDC     0x06

//////////////////////////////////////

#define NB_ANALOGS 4

// Motors
#define MOTOR_PUSHER_LEFT  1
#define MOTOR_PUSHER_RIGHT 2

#define MOTOR_PUSHER_FORWARD  -1
#define MOTOR_PUSHER_STOP      0
#define MOTOR_PUSHER_BACKWARD  1

// Servo-motors
#define SERVOM_DOOR  2
#define MOTOR_DOOR_OPEN    65
#define MOTOR_DOOR_CLOSED  200
#define MOTOR_DOOR_FREE    0

// Capteurs digitaux
#define DIGIT_PUSHER_BACK       1
#define DIGIT_PUSHER_FRONT      2
#define DIGIT_JACK              4
#define DIGIT_SWITCH_BACK_RIGHT 8
#define DIGIT_SWITCH_BACK_LEFT  16

// Capteurs analogiques
#define ANALO_DIST_FRONT_LEFT   0
#define ANALO_DIST_FRONT_RIGHT  1
#define ANALO_DIST_BACK_LEFT    3
#define ANALO_DIST_BACK_RIGHT   2


#endif // PROTO_H_
