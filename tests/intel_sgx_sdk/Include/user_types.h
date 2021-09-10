#ifndef _USER_TYPES_H_
#define _USER_TYPES_H_

typedef struct InputBuffer1{
    char *buffer_p;
    size_t size;
} InputBuffer1;

typedef union InputBuffer2{
    char *buffer_p;
    size_t size;
} InputBuffer2;

#endif 