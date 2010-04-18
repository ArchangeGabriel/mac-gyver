// proto.h / Definition du protocole //


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
//////////////////////////////////////


#define MOTORS     0x01
#define ANALOG     0x02
#define INITAN     0x03
#define SERVOS     0x04
#define DIGITS     0x05

//////////////////////////////////////

#define NB_ANALOGS 4

#endif // PROTO_H_
