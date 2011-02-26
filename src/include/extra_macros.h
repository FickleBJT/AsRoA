/* Register Settings */


// TIMSK bits

#define TOIE0  0x01
#define OCIE0  0x02
#define TOIE1  0x04
#define OCIE1B 0x08
#define OCIE1A 0x10
#define TICIE1 0x20
#define TOIE2  0x40
#define OCIE2  0x80


// TCCRx bits

#define CS00  0x01
#define CS01  0x02
#define CS02  0x04
#define WGM01 0x08
#define COM00 0x10
#define COM01 0x20
#define WGM00 0x40
#define FOC0  0x80 // Should always be zero
