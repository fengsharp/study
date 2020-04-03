#ifndef PROTO_H_
#define PROTO_H_

#define NAMESIZE 11
struct _msg_st
{
    uint8_t name[NAMESIZE];
    uint32_t math;
    uint32_t chinese;
}__attribute__((packed));

typedef struct _msg_st msg_st;

#define RCVPOOT 1989
#define MGROUP "224.2.2.2"

#endif