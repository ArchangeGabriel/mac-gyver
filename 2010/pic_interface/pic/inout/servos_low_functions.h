// servos_low_functions.h //


#define set_servotris() {\
TRISSVO1 = 0;\
TRISSVO2 = 0;\
TRISSVO3 = 0;\
TRISSVO4 = 0;\
}
 
#define clear_all_servo() {\
PINSVO1 = 0;\
PINSVO2 = 0;\
PINSVO3 = 0;\
PINSVO4 = 0;\
} 

#define set_usedservo() {\
if(positions[0]) PINSVO1 = 1;\
if(positions[1]) PINSVO2 = 1;\
if(positions[2]) PINSVO3 = 1;\
if(positions[3]) PINSVO4 = 1;\
}

#define clear_servo(n) {\
switch(n)\
{\
case 0: PINSVO1 = 0; break;\
case 1: PINSVO2 = 0; break;\
case 2: PINSVO3 = 0; break;\
case 3: PINSVO4 = 0; break;\
default: break;\
}}
