/*-------------------------------------------------------------------------
  common_types.h - Common declarations and types
-------------------------------------------------------------------------*/

#ifndef VASCO_H_
#define VASCO_H_

/* 8 bits */
typedef unsigned char uchar;
typedef signed char schar;
typedef union
{
    uchar uc;
    schar sc;
    struct
    {
        unsigned b0:1;
        unsigned b1:1;
        unsigned b2:1;
        unsigned b3:1;
        unsigned b4:1;
        unsigned b5:1;
        unsigned b6:1;
        unsigned b7:1;
    };
} Char;

/* 16 bits */
typedef unsigned int uint;
typedef signed int sint;
typedef union
{
    uint ui;
    sint si;
    Char c[2];
} Int;


/* 32 bits */
typedef unsigned long ulong;
typedef signed long slong;
typedef union
{
    ulong ul;
    slong sl;
    Int   i[2];
    Char  c[4];
} Long;

#define TRUE 1
#define FALSE 0
#define NULL 0

#endif /*VASCO_H_*/
